/* ========================================
	DX22Base/
	------------------------------------
	テクスチャ用cpp
	------------------------------------
	Texture.cpp
========================================== */

// =============== インクルード ===================
#include "Texture.h"
#include "DirectXManager.h"	// DirectXManager



/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
Texture::Texture()
	: m_width(0), m_height(0)
	, m_pTex(nullptr)
	, m_pSRV(nullptr)
{
}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：解放
=========================================== */
Texture::~Texture()
{
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pTex);
}

/* ========================================
	コピーコンストラクタ
	-------------------------------------
	内容：コピーコンストラクタ
		　※アニメモデルのディープコピー時用
=========================================== */
Texture::Texture(const Texture& other)
	: m_width(0)
	, m_height(0)
	, m_pTex(nullptr)
	, m_pSRV(nullptr)
{
	if (this == &other) return;

	m_width = other.m_width;
	m_height = other.m_height;

	// テクスチャが存在する場合
	if (other.m_pTex)
	{
		// テクスチャの設定を取得
		D3D11_TEXTURE2D_DESC desc;
		other.m_pTex->GetDesc(&desc);

		// テクスチャ作成
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = nullptr;
		data.SysMemPitch = desc.Width * 4;

		// テクスチャ生成
		DirectXManager::GetDevice()->CreateTexture2D(&desc, nullptr, &m_pTex);

	}
	// シェーダリソースビューが存在する場合
	if (other.m_pSRV) {
		ID3D11Resource* resource = nullptr;

		// 元の ShaderResourceView からリソースを取得
		other.m_pSRV->GetResource(&resource);

		// 元のビューの設定を取得
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		other.m_pSRV->GetDesc(&srvDesc);

		// 新しい ShaderResourceView を作成
		ID3D11Device* device = DirectXManager::GetDevice();
		if (device) {
			device->CreateShaderResourceView(resource, &srvDesc, &m_pSRV);
		}

		// リソースの解放
		if (resource) {
			resource->Release();
		}
	}
}


/* ========================================
	テクスチャ作成関数
	-------------------------------------
	内容：画像ファイルからテクスチャを作成
	-------------------------------------
	引数：fileName		ファイルパス
	-------------------------------------
	戻り値：HRESULT		結果
=========================================== */
HRESULT Texture::Create(const char* fileName)
{
	HRESULT hr = S_OK;

	// 文字変換
	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;
	MultiByteToWideChar(0, 0, fileName, -1, wPath, MAX_PATH);	// ワイド文字に変換

	// ファイル別読み込み
	DirectX::TexMetadata	mdata;	// メタデータ(画像情報)
	DirectX::ScratchImage	image;	// 画像データ

	// TGAファイル
	if (strstr(fileName, ".tga"))	
	{
		hr = DirectX::LoadFromTGAFile(wPath, &mdata, image);
	}
	// それ以外
	else
	{
		hr = DirectX::LoadFromWICFile(wPath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &mdata, image);

	}
	// 読み込み失敗
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// シェーダリソースビュー生成
	hr = CreateShaderResourceView(
		DirectXManager::GetDevice(),	// デバイス
		image.GetImages(),				// 画像データ
		image.GetImageCount(),			// 画像数
		mdata,							// メタデータ
		&m_pSRV);						// シェーダリソースビュー

	// 読み込み失敗
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// 横幅、縦幅取得
	m_width		= (UINT)mdata.width;
	m_height	= (UINT)mdata.height;


	return hr;
}


/* ========================================
	テクスチャ作成関数
	-------------------------------------
	内容：フォーマットを指定してテクスチャを作成
	-------------------------------------
	引数1：format		フォーマット
	引数2：width		横幅
	引数3：height		縦幅
	引数4：pData		初期データ
	-------------------------------------
	戻り値：HRESULT		結果
=========================================== */
HRESULT Texture::Create(DXGI_FORMAT format, UINT width, UINT height, const void* pData)
{
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);	// テクスチャ設定
	return CreateResource(desc, pData);
}


/* ========================================
	テクスチャ構造体作成関数
	-------------------------------------
	内容：テクスチャ構造体を作成
	-------------------------------------
	引数1：format		フォーマット
	引数2：width		横幅
	引数3：height		縦幅
	-------------------------------------
	戻り値：HRESULT		結果
=========================================== */
D3D11_TEXTURE2D_DESC Texture::MakeTexDesc(DXGI_FORMAT format, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); 

	desc.Width				= width;						// テクスチャの幅（ピクセル単位）
	desc.Height				= height;						// テクスチャの高さ（ピクセル単位）
	desc.MipLevels			= 1;							// ミップマップのレベル数（1に設定するとミップマップなし）
	desc.ArraySize			= 1;							// テクスチャ配列のサイズ（1で通常のテクスチャ）
	desc.Format				= format;						// テクスチャのピクセルフォーマット（例: RGBAなど）
	desc.SampleDesc.Count	= 1;							// マルチサンプリングの数（1に設定するとサンプリングなし）
	desc.SampleDesc.Quality = 0;							// マルチサンプリングの品質（0に設定するとサンプリングなし）
	desc.Usage				= D3D11_USAGE_DEFAULT;			// GPUによるデフォルトの読み書きが可能
	desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;	// シェーダからアクセスできるようにバインド
	desc.CPUAccessFlags		= 0;							// CPUからのアクセスは不要
	desc.MiscFlags			= 0;							// その他のフラグ
	return desc;
}

/* ========================================
	シェーダーリソースビュー作成関数
	-------------------------------------
	内容：テクスチャリソースを作成
	-------------------------------------
	引数1：desc			テクスチャ設定
	引数2：pData		初期データ
	-------------------------------------
	戻り値：HRESULT		結果
=========================================== */
HRESULT Texture::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	HRESULT hr = E_FAIL;

	// テクスチャ作成
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem		= pData;			// テクスチャデータ
	data.SysMemPitch	= desc.Width * 4;	// 1行のバイト数

	// テクスチャ生成
	hr = DirectXManager::GetDevice()->CreateTexture2D(&desc, pData ? &data : nullptr, &m_pTex);
	if (FAILED(hr)) { return hr; }

	// シェーダーリソースビュー設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	// フォーマット変換(シェーダがそのデータを正しく扱える為に)
	switch (desc.Format)
	{
	default:							srvDesc.Format = desc.Format;			break;	// そのまま
	case	DXGI_FORMAT_R32_TYPELESS: 	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;	break;	// 32bit浮動小数点
	}
	srvDesc.ViewDimension		= D3D11_SRV_DIMENSION_TEXTURE2D;	// 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;								// ミップマップレベル数

	
	// シェーダリソースビュー生成
	hr = DirectXManager::GetDevice()->CreateShaderResourceView(m_pTex, &srvDesc, &m_pSRV);

	// 読み込み失敗
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// 横幅、縦幅取得
	m_width		= desc.Width;
	m_height	= desc.Height;
	
	
	return hr;
}



/* ========================================
	ゲッター(横幅)関数
	-------------------------------------
	戻値：UINT		横幅
=========================================== */
UINT Texture::GetWidth() const
{
	return m_width;
}

/* ========================================
	ゲッター(縦幅)関数
	-------------------------------------
	戻値：UINT		縦幅
=========================================== */
UINT Texture::GetHeight() const
{
	return m_height;
}

/* ========================================
	ゲッター(シェーダリソースビュー)関数
	-------------------------------------
	戻値：ID3D11ShaderResourceView*		シェーダリソースビュー
=========================================== */
ID3D11ShaderResourceView* Texture::GetResource() const
{
	return m_pSRV;
}
