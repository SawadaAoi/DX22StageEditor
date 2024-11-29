/* ========================================
	DX22Base/
	------------------------------------
	頂点シェーダー用cpp
	------------------------------------
	VertexShader.cpp
========================================== */

// =============== インクルード ===================
#include "VertexShader.h"
#include <d3dcompiler.h>	// シェーダーのコンパイル


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
VertexShader::VertexShader()
	: Shader(Shader::Vertex)	// シェーダーの種類を頂点シェーダに設定
	, m_pVS(nullptr)
	, m_pInputLayout(nullptr)
{
}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：解放処理
=========================================== */
VertexShader::~VertexShader()
{
	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pVS);
}

/* ========================================
	バインド関数
	-------------------------------------
	内容：頂点シェーダーを GPU にバインドし、
		　描画処理で使用できる状態にする
=========================================== */
void VertexShader::Bind(void)
{
	// デバイスコンテキストの取得
	ID3D11DeviceContext* pContext = DirectXManager::GetContext();

	pContext->VSSetShader(m_pVS, NULL, 0);		// 頂点シェーダーをバインド
	pContext->IASetInputLayout(m_pInputLayout);	// インプットレイアウトをバインド

	// 定数バッファの数だけバインド
	for (int i = 0; i < m_pBuffers.size(); ++i)
	{
		pContext->VSSetConstantBuffers(i, 1, &m_pBuffers[i]);
	}
	// テクスチャの数だけバインド
	for (int i = 0; i < m_pTextures.size(); ++i)
	{
		pContext->VSSetShaderResources(i, 1, &m_pTextures[i]);
	}
}

/* ========================================
	シェーダー作成関数
	-------------------------------------
	内容：シェーダーのバイトコードから頂点シェーダーを作成。
		  頂点バッファの入力データ形式を定義するインプットレイアウトも作成
	-------------------------------------
	引数1：pData		データ
	引数2：size		サイズ
	-------------------------------------
	戻り値：エラーコード
=========================================== */
HRESULT VertexShader::MakeShader(void* pData, UINT size)
{
	HRESULT hr;
	ID3D11Device* pDevice = DirectXManager::GetDevice();	// デバイスの取得

	// シェーダー作成
	hr = pDevice->CreateVertexShader(pData, size, NULL, &m_pVS);
	if (FAILED(hr)) { return hr; }

	/*
	シェーダ作成時にシェーダリフレクションを通してインプットレイアウトを取得
	セマンティクスの配置などから識別子を作成
	識別子が登録済→再利用、なければ新規作成
	https://blog.techlab-xe.net/dxc-shader-reflection/
	*/

	ID3D11ShaderReflection*			pReflection;	// シェーダリフレクション
	D3D11_SHADER_DESC				shaderDesc;		// シェーダーの情報
	D3D11_INPUT_ELEMENT_DESC*		pInputDesc;		// インプットレイアウト
	D3D11_SIGNATURE_PARAMETER_DESC	sigDesc;		// シグネチャ

	DXGI_FORMAT formats[][4] =
	{
		{
			DXGI_FORMAT_R32_UINT,			// 1つの32ビット符号なし整数
			DXGI_FORMAT_R32G32_UINT,		// 2つの32ビット符号なし整数
			DXGI_FORMAT_R32G32B32_UINT,		// 3つの32ビット符号なし整数
			DXGI_FORMAT_R32G32B32A32_UINT	// 4つの32ビット符号なし整数
		},
		{
			DXGI_FORMAT_R32_SINT,
			DXGI_FORMAT_R32G32_SINT,
			DXGI_FORMAT_R32G32B32_SINT,
			DXGI_FORMAT_R32G32B32A32_SINT,
		}, 
		{
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_R32G32B32_FLOAT,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
		},
	};

	// シェーダリフレクションの作成
	hr = D3DReflect(pData, size, IID_PPV_ARGS(&pReflection));	// シェーダーバイトコードを解析して、その情報を反映する関数
	if (FAILED(hr)) { return hr; }

	pReflection->GetDesc(&shaderDesc);	// シェーダーの詳細情報（例えば、入力パラメータの数や構造）を取得
	pInputDesc = new D3D11_INPUT_ELEMENT_DESC[shaderDesc.InputParameters];	// InputParameters: 入力パラメータの数

	// インプットレイアウトの作成
	for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
	{
		// シェーダーが期待する各入力パラメータのセマンティクス（POSITION, TEXCOORD, NORMAL など）やそのインデックスを取得
		pReflection->GetInputParameterDesc(i, &sigDesc);

		// セマンティクス名、セマンティクスインデックスを取得
		pInputDesc[i].SemanticName	= sigDesc.SemanticName;		// セマンティクス名
		pInputDesc[i].SemanticIndex = sigDesc.SemanticIndex;	// セマンティクスインデックス

		// 頂点フォーマットの決定
		// http://marupeke296.com/TIPS_No17_Bit.html
		BYTE elementCount = sigDesc.Mask;
		// コンポーネント数を計算してフォーマットを決定
		elementCount = (elementCount & 0x05) + ((elementCount >> 1) & 0x05);	// 0x05 = 0101
		elementCount = (elementCount & 0x03) + ((elementCount >> 2) & 0x03);	// 0x03 = 0011

		// コンポーネントタイプによってフォーマットを決定
		switch (sigDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			pInputDesc[i].Format = formats[0][elementCount - 1];
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			pInputDesc[i].Format = formats[1][elementCount - 1];
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			pInputDesc[i].Format = formats[2][elementCount - 1];
			break;
		}
		pInputDesc[i].InputSlot = 0;
		pInputDesc[i].AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
		pInputDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pInputDesc[i].InstanceDataStepRate = 0;
	}

	// インプットレイアウトの作成
	hr = pDevice->CreateInputLayout(
		pInputDesc, shaderDesc.InputParameters,
		pData, size, &m_pInputLayout
	);

	delete[] pInputDesc;
	return hr;
}
