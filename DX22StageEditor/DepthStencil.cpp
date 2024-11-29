/* ========================================
	DX22Base/
	------------------------------------
	デプスステンシルビュー用cpp
	------------------------------------
	DepthStencil.cpp
========================================== */

// =============== インクルード ===================
#include "DepthStencil.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
========================================== */
DepthStencil::DepthStencil()
	: m_pDSV(nullptr)
{
}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：解放
========================================== */
DepthStencil::~DepthStencil()
{
	SAFE_RELEASE(m_pDSV);
}


/* ========================================
	クリア関数
	-------------------------------------
	内容：デプスステンシルビューをクリア
========================================== */
void DepthStencil::Clear()
{
	DirectXManager::GetContext()->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}


/* ========================================
	作成関数
	-------------------------------------
	内容：デプスステンシルビューを作成
	-------------------------------------
	引数1：width		横幅
	引数2：height		縦幅
	引数3：useStencil	ステンシル使用フラグ
	-------------------------------------
	戻り値：HRESULT	結果
========================================== */
HRESULT DepthStencil::Create(UINT width, UINT height, bool useStencil)
{
	// https://docs.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-depth-stencil#compositing
	// フォーマットを指定して、テクスチャ設定構造体を作成
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(useStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS, width, height);

	// ステンシル使用判定
	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;		// テクスチャの使用先	

	// 生成
	return CreateResource(desc);
}


/* ========================================
	リソース作成関数
	-------------------------------------
	内容：リソースを作成
	-------------------------------------
	引数1：desc		テクスチャ設定
	引数2：pData	初期データ
	-------------------------------------
	戻り値：HRESULT	結果
========================================== */
HRESULT DepthStencil::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	// ステンシル使用判定
	bool useStencil = (desc.Format == DXGI_FORMAT_R24G8_TYPELESS);

	HRESULT hr;

	// テクスチャ作成
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem		= pData;			// テクスチャデータ
	data.SysMemPitch	= desc.Width * 4;	// 1行のバイト数

	// テクスチャ生成
	hr = DirectXManager::GetDevice()->CreateTexture2D(&desc, pData ? &data : nullptr, &m_pTex);
	if (FAILED(hr)) { return hr; }

	// 設定
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format			= useStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;	// フォーマット
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;											// ビューの次元 (1D, 2D, 3D)。

	// デプスステンシルビュー生成
	return DirectXManager::GetDevice()->CreateDepthStencilView(m_pTex, &dsvDesc, &m_pDSV);
}


/* ========================================
	ゲッター(ビュー取得)
	-------------------------------------
	戻値：ID3D11DepthStencilView*		ビュー
=========================================== */
ID3D11DepthStencilView* DepthStencil::GetView() const
{
	return m_pDSV;
}