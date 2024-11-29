/* ========================================
	DX22Base/
	------------------------------------
	ピクセルシェーダー用cpp
	------------------------------------
	PixelShader.cpp
========================================== */

// =============== インクルード ===================
#include "PixelShader.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
PixelShader::PixelShader()
	: Shader(Shader::Pixel)	// シェーダーの種類をピクセルシェーダに設定
	, m_pPS(nullptr)
{
}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：解放処理
=========================================== */
PixelShader::~PixelShader()
{
	SAFE_RELEASE(m_pPS);
}

/* ========================================
	バインド関数
	-------------------------------------
	内容：ピクセルシェーダーを GPU にバインドし、
		　描画処理で使用できる状態にする
=========================================== */
void PixelShader::Bind(void)
{
	// デバイスコンテキストの取得
	ID3D11DeviceContext* pContext = DirectXManager::GetContext();
	// ピクセルシェーダーをバインド
	pContext->PSSetShader(m_pPS, nullptr, 0);

	// 定数バッファの数だけバインド
	for (int i = 0; i < m_pBuffers.size(); ++i)
	{
		pContext->PSSetConstantBuffers(i, 1, &m_pBuffers[i]);
	}
	// テクスチャの数だけバインド
	for (int i = 0; i < m_pTextures.size(); ++i)
	{
		pContext->PSSetShaderResources(i, 1, &m_pTextures[i]);
	}
}

/* ========================================
	シェーダー作成関数
	-------------------------------------
	内容：シェーダーのバイトコードからピクセルシェーダーを作成。
	-------------------------------------
	引数1：pData	データ
	引数2：size		サイズ
=========================================== */
HRESULT PixelShader::MakeShader(void* pData, UINT size)
{
	return DirectXManager::GetDevice()->CreatePixelShader(pData, size, NULL, &m_pPS);
}