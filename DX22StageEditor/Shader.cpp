/* ========================================
	DX22Base/
	------------------------------------
	シェーダー用cpp
	------------------------------------
	Shader.cpp
========================================== */

// =============== インクルード ===================
#include "Shader.h"
#include <d3dcompiler.h>	// シェーダーのコンパイル
#include <stdio.h>			// ファイル読み込み

// =============== リンク =======================
#pragma comment(lib, "d3dcompiler.lib")	// シェーダーのコンパイル


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
Shader::Shader(Kind kind)
	: m_kind(kind)
{
}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：解放処理
=========================================== */
Shader::~Shader()
{
	std::vector<ID3D11Buffer*>::iterator it = m_pBuffers.begin();
	while (it != m_pBuffers.end())
	{
		SAFE_RELEASE((*it));
		++it;
	}
}

/* ========================================
	シェーダーファイル(*.cso)読込関数
	-------------------------------------
	内容：
	-------------------------------------
	引数：pFileName	ファイル名
	-------------------------------------
	戻り値：HRESULT	結果
=========================================== */
HRESULT Shader::Load(const char* pFileName)
{
	HRESULT hr = E_FAIL;

	// ファイルを読み込む
	FILE* fp;
	fopen_s(&fp, pFileName, "rb");	// バイナリモードで読み込み
	if (!fp) { return hr; }			// ファイルが開けなかったら終了

	// ファイルのサイズを調べる
	int fileSize = 0;
	fseek(fp, 0, SEEK_END);		// ファイルの最後まで移動
	fileSize = ftell(fp);		// ファイルの位置を取得(=ファイルサイズ)

	// メモリに読み込み
	fseek(fp, 0, SEEK_SET);				// ファイルの先頭に移動
	char* pData = new char[fileSize];	// ファイルサイズ分のメモリを確保
	fread(pData, fileSize, 1, fp);		// ファイルを読み込む
	fclose(fp);							// ファイルを閉じる

	// シェーダー作成
	hr = Make(pData, fileSize);
	
	// 終了処理
	if (pData) { delete[] pData; }
	return hr;
}

/* ========================================
	シェーダーコンパイル関数
	-------------------------------------
	内容：
	-------------------------------------
	引数：pCode	コード
	-------------------------------------
	戻り値：HRESULT	結果
=========================================== */
HRESULT Shader::Compile(const char *pCode)
{
    // ターゲットリスト
    static const char *pTargetList[] = 
    {
        "vs_5_0",  // 頂点シェーダーのコンパイルターゲット
        "ps_5_0"   // ピクセルシェーダーのコンパイルターゲット
    };

    HRESULT hr;
    ID3DBlob *pBlob = nullptr;  // コンパイルされたシェーダーコードを格納
    ID3DBlob *error = nullptr;  // エラーメッセージを格納

    UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;  // デバッグフラグを使用(デバッグ情報/最適化をスキップ)

    hr = D3DCompile(pCode, strlen(pCode), nullptr, nullptr, nullptr,
        "main", pTargetList[m_kind], compileFlag, 0, &pBlob, &error);
    
    // コンパイルが失敗した場合、エラーハンドリング
    if (FAILED(hr)) { return hr; }

    // シェーダ作成
    hr = Make(pBlob->GetBufferPointer(), (UINT)pBlob->GetBufferSize());

    // リソースを解放
    SAFE_RELEASE(pBlob);
    SAFE_RELEASE(error);
    
    return hr;
}


/* ========================================
	定数バッファ書込関数
	-------------------------------------
	内容：シェーダーに渡すデータを
		　定数バッファに書き込み、GPU に送る
	-------------------------------------
	引数1：slot		バッファ番号
	引数2：pData	書き込むデータ
=========================================== */
void Shader::WriteBuffer(UINT slot, const void* pData)
{
	// バッファのサイズを超えていないかチェック
	if (slot < m_pBuffers.size())
	{
		DirectXManager::GetContext()->UpdateSubresource(m_pBuffers[slot], 0, nullptr, pData, 0, 0);
	}
}

/* ========================================
	テクスチャ設定関数
	-------------------------------------
	内容：テクスチャを設定
	-------------------------------------
	引数1：slot		テクスチャを設定するシェーダースロット番号
	引数2：tex		テクスチャ
=========================================== */
void Shader::SetTexture(UINT slot, const Texture* tex)
{
	// テクスチャがないか、スロット番号が不正なら終了
	if (!tex || slot >= m_pTextures.size()) { return; }
	
	// テクスチtextureのリソースを取得(=GPU がテクスチャをシェーダーで使用できるようにするため)
	ID3D11ShaderResourceView* pTex = tex->GetResource();

	m_pTextures[slot] = pTex;	// テクスチャを設定

	// シェーダーにテクスチャを設定
	switch (m_kind)
	{
	case Vertex:	DirectXManager::GetContext()->VSSetShaderResources(slot, 1, &pTex); break;
	case Pixel:		DirectXManager::GetContext()->PSSetShaderResources(slot, 1, &pTex); break;
	}
}

/* ========================================
	シェーダー作成関数
	-------------------------------------
	内容：シェーダーファイルのデータを受け取って、
		　その中身を解析し、シェーダーに必要なリソース
		 （定数バッファやテクスチャリソース）を準備
	-------------------------------------
	引数1：pData		データ
	引数2：size		サイズ
	-------------------------------------
	戻り値：HRESULT	結果
=========================================== */
HRESULT Shader::Make(void* pData, UINT size)
{
	HRESULT hr;
	ID3D11Device* pDevice = DirectXManager::GetDevice();

	// 解析用のリフレクション作成
	ID3D11ShaderReflection* pReflection;
	hr = D3DReflect(pData, size, IID_PPV_ARGS(&pReflection));
	if (FAILED(hr)) { return hr; }

	// 定数バッファ作成
	D3D11_SHADER_DESC shaderDesc;
	pReflection->GetDesc(&shaderDesc);	// シェーダーの基本情報を取得

	m_pBuffers.resize(shaderDesc.ConstantBuffers, nullptr);	// 定数バッファの数だけバッファを作成

	// 定数バッファの作成
	for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
	{
		// 定数バッファの情報取得
		D3D11_SHADER_BUFFER_DESC shaderBufDesc;														
		ID3D11ShaderReflectionConstantBuffer* cbuf = pReflection->GetConstantBufferByIndex(i);
		cbuf->GetDesc(&shaderBufDesc);

		// 作成するバッファの情報
		D3D11_BUFFER_DESC bufDesc;
		ZeroMemory(&bufDesc, sizeof(bufDesc));
		bufDesc.ByteWidth	= shaderBufDesc.Size;			// バッファのサイズ
		bufDesc.Usage		= D3D11_USAGE_DEFAULT;		// バッファの使用方法
		bufDesc.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;	// バッファのバインドフラグ

		// バッファの作成
		hr = pDevice->CreateBuffer(&bufDesc, nullptr, &m_pBuffers[i]);
		if (FAILED(hr)) { return hr; }
	}

	// テクスチャ領域作成
	m_pTextures.resize(shaderDesc.TextureNormalInstructions, nullptr);

	return MakeShader(pData, size);	// シェーダー作成
}


