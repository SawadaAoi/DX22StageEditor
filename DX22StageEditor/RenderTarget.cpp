/* ========================================
	DX22Base/
	------------------------------------
	レンダーターゲット用cpp
	------------------------------------
	RenderTarget.cpp
========================================== */

// =============== インクルード ===================
#include "RenderTarget.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
========================================== */
RenderTarget::RenderTarget()
	: m_pRTV(nullptr)
{
}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：解放
========================================== */
RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(m_pRTV);
}

/* ========================================
	クリア関数
	-------------------------------------
	内容：レンダーターゲットをクリア
========================================== */
void RenderTarget::Clear()
{
	static float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Clear(color);
}

/* ========================================
	クリア関数
	-------------------------------------
	内容：レンダーターゲットをクリア
	-------------------------------------
	引数：color		クリア色
========================================== */
void RenderTarget::Clear(const float* color)
{
	DirectXManager::GetContext()->ClearRenderTargetView(m_pRTV, color);
}


/* ========================================
	レンダーターゲット作成関数
	-------------------------------------
	内容：フォーマットを指定して
		　レンダーターゲットを作成
	-------------------------------------
	引数1：format		フォーマット
	引数2：width		横幅
	引数3：height		縦幅
	-------------------------------------
	戻値：HRESULT		結果
=========================================== */
HRESULT RenderTarget::Create(DXGI_FORMAT format, UINT width, UINT height)
{
	// テクスチャ構造体作成
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);	

	// レンダーターゲットとして使用
	desc.BindFlags	|= D3D11_BIND_RENDER_TARGET;	// テクスチャの使用先				

	return CreateResource(desc);	// リソース作成
}

/* ========================================
	レンダーターゲット作成関数
	-------------------------------------
	内容：スワップチェインからレンダーターゲットを作成
	-------------------------------------
	戻値：HRESULT		結果
=========================================== */
HRESULT RenderTarget::CreateFromScreen()
{
	HRESULT hr;	

	// スワップチェインのバックバッファのポインタを取得(m_pTexに設定)
	hr = DirectXManager::GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pTex);

	// 失敗
	if (FAILED(hr)) { return hr; }


	// レンダーターゲットビューの詳細設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;		// RTV が関連付けられるリソースのフォーマット
	rtvDesc.ViewDimension	= D3D11_RTV_DIMENSION_TEXTURE2D;	// ビューの次元 (1D, 2D, 3D)。

	// レンダーターゲットビューを作成
	hr = DirectXManager::GetDevice()->CreateRenderTargetView(m_pTex, &rtvDesc, &m_pRTV);

	if (FAILED(hr)) { return hr; }

	// テクスチャの情報を取得し、幅と高さを設定
	D3D11_TEXTURE2D_DESC desc;
	m_pTex->GetDesc(&desc);
	m_width = desc.Width;
	m_height = desc.Height;
	
	return hr;
}

/* ========================================
	リソース作成関数
	-------------------------------------
	内容：リソース作成
	-------------------------------------
	引数1：desc		テクスチャ構造体
	引数2：pData		初期データ
	-------------------------------------
	戻値：HRESULT		結果
=========================================== */
HRESULT RenderTarget::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
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
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format			= desc.Format;						// フォーマット
	rtvDesc.ViewDimension	= D3D11_RTV_DIMENSION_TEXTURE2D;	// ビューの次元 (1D, 2D, 3D)。

	// レンダーターゲットビュー生成
	hr = DirectXManager::GetDevice()->CreateRenderTargetView(m_pTex, &rtvDesc, &m_pRTV);
	if (FAILED(hr)) { return hr; }

	// 幅と高さを設定
	m_width		= desc.Width;
	m_height	= desc.Height;

	return hr;

}


/* ========================================
	ゲッター(ビュー取得)
	-------------------------------------
	戻値：ID3D11RenderTargetView*	レンダーターゲットビュー
=========================================== */
ID3D11RenderTargetView* RenderTarget::GetView() const
{
	return m_pRTV;
}