/* ========================================
	DX22Base/
	------------------------------------
	DirectX管理用cpp
	------------------------------------
	DirectXManager.cpp
========================================== */

// =============== インクルード ===================
#include "DirectXManager.h"		// DirectX管理用ヘッダ
#include "Texture.h"			// テクスチャ管理用ヘッダ
#include "WindowAPI.h"
#include <map>
#include "RenderTarget.h"
#include "DepthStencil.h"

#include "DebugMenu.h"	// デバッグメニュー


// =============== 定数定義 =======================
const float SCREEN_RESET_COLOR[4] = { 0.8f, 0.9f, 1.0f, 1.0f };	// 画面クリアカラー


// カリング設定
D3D11_CULL_MODE CULLING_MODE[DirectXManager::CullMode::CULL_MAX] = {
	D3D11_CULL_NONE,	// カリング無し
	D3D11_CULL_FRONT,	// 前面カリング
	D3D11_CULL_BACK		// 背面カリング
};

// ブレンドモード
D3D11_BLEND BLEND_MODE[DirectXManager::BlendMode::BLEND_MAX][2] = {
		{D3D11_BLEND_ONE,				D3D11_BLEND_ZERO			},	// ブレンド無し
		{D3D11_BLEND_SRC_ALPHA,			D3D11_BLEND_INV_SRC_ALPHA	},	// アルファブレンド
		{D3D11_BLEND_ONE,				D3D11_BLEND_ONE				},	// 加算ブレンド
		{D3D11_BLEND_SRC_ALPHA,			D3D11_BLEND_ONE				},	// 加算アルファブレンド
		{D3D11_BLEND_ZERO,				D3D11_BLEND_INV_SRC_COLOR	},	// 減算ブレンド
		{D3D11_BLEND_INV_DEST_COLOR,	D3D11_BLEND_ONE				},	// スクリーンブレンド
};

// 深度ステート設定
std::pair< D3D11_DEPTH_WRITE_MASK, bool> DEPTH_MASK_PATTERN[DirectXManager::DepthState::DEPTH_MAX] =
{
	{D3D11_DEPTH_WRITE_MASK_ALL,	 true},	// 深度書き込みとテスト有効(デフォルト)
	{D3D11_DEPTH_WRITE_MASK_ZERO,	 true},	// 深度書き込み無しテスト有効(透過オブジェクト、影用)
	{D3D11_DEPTH_WRITE_MASK_ZERO,	false},	// 深度描き込み無しテスト無し(2D描画用)
};

// サンプラーステート用フィルタリング
D3D11_FILTER SAMPLER_FILTER[] = {
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,	// ポイントフィルタリング(テクスチャの最も近いピクセルの色を使用します。エッジがはっきりとしており、ぼやけた印象がありません)
		D3D11_FILTER_MIN_MAG_MIP_POINT,		// 縦横方向にリニアフィルタリングを使用し、ミップマップレベルはポイントフィルタリングを使用します。
};

// 初期値
const DirectXManager::CullMode			DEFAULT_CULLING_MODE	= DirectXManager::CULL_BACK;				// カリングモード
const DirectXManager::BlendMode			EFAULT_BLEND_MODE		= DirectXManager::BLEND_ALPHA;				// ブレンドモード
const DirectXManager::DepthState		DEFAULT_DEPTH_STATE		= DirectXManager::DEPTH_ENABLE_WRITE_TEST;	// 深度ステート
const DirectXManager::SamplerState		DEFAULT_SAMPLER_STATE	= DirectXManager::SAMPLER_LINEAR;			// サンプラーステート


/* ========================================
	DirectX初期化関数
	-------------------------------------
	内容：DirectXの初期化を行う
	-------------------------------------
	戻値：HRESULT型の戻り値
=========================================== */
HRESULT DirectXManager::InitDirectX()
{
	

	HRESULT	hr = E_FAIL;	// 戻り値

	// デバイスとスワップチェインの初期化
	if (FAILED(hr = InitDeviceAndSwapChain()))
	{
		MessageBox(NULL, "デバイスとスワップチェインの初期化に失敗しました", "エラー", MB_OK);

		return hr;
	}

	// レンダーターゲットビューの初期化
	if (FAILED(hr = InitRenderTargetView()))
	{
		MessageBox(NULL, "レンダーターゲットビューの初期化に失敗しました", "エラー", MB_OK);

		return hr;
	}
	
	// 深度ステンシルビューの初期化
	if (FAILED(hr = InitDepthStencilView()))
	{
		MessageBox(NULL, "深度ステンシルビューの初期化に失敗しました", "エラー", MB_OK);

		return hr;
	}

	// ラスタライザステートの初期化
	if (FAILED(hr = InitRasterizerState()))
	{
		MessageBox(NULL, "ラスタライザステートの初期化に失敗しました", "エラー", MB_OK);

		return hr;
	}

	// ビューポートの初期化
	InitViewPort();

	// 深度ステンシルステートの初期化
	if (FAILED(hr = InitDepthStencilState()))
	{
		MessageBox(NULL, "深度ステンシルステートの初期化に失敗しました", "エラー", MB_OK);

		return hr;
	}

	// ブレンドステートの初期化
	if (FAILED(hr = InitBlendState()))
	{
		MessageBox(NULL, "ブレンドステートの初期化に失敗しました", "エラー", MB_OK);

		return hr;
	}

	// サンプラーステートの初期化
	if (FAILED(hr = InitSamplerState()))
	{
		MessageBox(NULL, "サンプラーステートの初期化に失敗しました", "エラー", MB_OK);

		return hr;
	}

	return S_OK;
}

/* ========================================
	DirectX解放関数
	-------------------------------------
	内容：DirectXの各変数の解放を行う
=========================================== */
void DirectXManager::UninitDirectX()
{
	SAFE_DELETE(m_pDSV);
	SAFE_DELETE(m_pRTV);

	for (int i = 0; i < SAMPLER_MAX; ++i)
		SAFE_RELEASE(m_pSamplerState[i]);

	for (int i = 0; i < BLEND_MAX; ++i)
		SAFE_RELEASE(m_pBlendState[i]);

	for (int i = 0; i < CULL_MAX; ++i)
		SAFE_RELEASE(m_pRasterizerStates[i]);

	for (int i = 0; i < DEPTH_MAX; ++i)
		SAFE_RELEASE(m_pDepthStencilState[i]);

	if (m_pContext)
		m_pContext->ClearState();
	SAFE_RELEASE(m_pContext);
	if (m_pSwapChain)
		m_pSwapChain->SetFullscreenState(false, NULL);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDevice);
}

/* ========================================
	デバイス・スワップチェイン初期化関数
	-------------------------------------
	内容：デバイスとスワップチェインの初期化を行う
	-------------------------------------
	戻値：HRESULT型の戻り値
=========================================== */
HRESULT DirectXManager::InitDeviceAndSwapChain()
{
	HRESULT	hr = E_FAIL;	// 戻り値

	// バックバッファの設定
	DXGI_MODE_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Width					= WINDOW_INST.GetScreenSize().nWidth;	// ウィンドウの幅
	bd.Height					= WINDOW_INST.GetScreenSize().nHeight;	// ウィンドウの高さ
	bd.RefreshRate.Numerator	= WINDOW_INST.GetRefreshRate();			// リフレッシュレート_分子
	bd.RefreshRate.Denominator	= 1;									// リフレッシュレート_分母
	bd.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;			// ピクセルフォーマットフォーマット
	bd.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// スキャンラインオーダー
	bd.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;		// スケーリング

	// バックバッファのマルチサンプリングに関する設定
	DXGI_SAMPLE_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Count	= 1;	// サンプリング数
	sd.Quality	= 0;	// サンプリング品質

	// スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferDesc		= bd;										// バックバッファの設定
	scd.SampleDesc		= sd;										// マルチサンプルの数
	scd.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;			// バックバッファの使用方法
	scd.BufferCount		= 1;										// バックバッファの数
	scd.OutputWindow	= WINDOW_INST.GetHwnd();					// 関連付けるウインドウ
	scd.Windowed		= TRUE;										// ウィンドウモード
	scd.SwapEffect		= DXGI_SWAP_EFFECT_DISCARD;					// バッファの交換方法(フリップ後のバッファを破棄)
	scd.Flags			= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// フラグ	


	// ドライバの種類
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,   // GPUで描画
		D3D_DRIVER_TYPE_WARP,       // 高精度(低速
		D3D_DRIVER_TYPE_REFERENCE   // CPUで描画
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes); // ドライバの数

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; // デバッグモード
#endif

	// 機能レベル(アプリケーションはGPUの機能に応じて適切なレベルで動作させる)
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,     // DirectX11.1対応GPUレベル
		D3D_FEATURE_LEVEL_11_0,     // DirectX11対応GPUレベル
		D3D_FEATURE_LEVEL_10_1,     // DirectX10.1対応GPUレベル
		D3D_FEATURE_LEVEL_10_0,     // DirectX10対応GPUレベル
		D3D_FEATURE_LEVEL_9_3,      // DirectX9.3対応GPUレベル
		D3D_FEATURE_LEVEL_9_2,      // DirectX9.2対応GPUレベル
		D3D_FEATURE_LEVEL_9_1       // Direct9.1対応GPUレベル
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);	// 機能レベルの数


	// 高機能なドライバを優先してデバイスを生成
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
	{
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,							// ディスプレイデバイスのアダプタ（NULLの場合最初に見つかったアダプタ）
			driverTypes[driverTypeIndex],	// デバイスドライバのタイプ
			NULL,							// ソフトウェアラスタライザを使用する場合に指定する
			createDeviceFlags,				// デバイスフラグ
			featureLevels,					// 機能レベル
			numFeatureLevels,				// 機能レベル数
			D3D11_SDK_VERSION,				// SDKバージョン
			&scd,							// スワップチェインの設定
			&m_pSwapChain,					// IDXGIDwapChainインタフェース    
			&m_pDevice,						// ID3D11Deviceインタフェース
			&m_FeatureLevel,				// サポートされている機能レベル
			&m_pContext						// デバイスコンテキスト
		);
		// デバイスの生成に成功したらループを抜ける
		if (SUCCEEDED(hr)) { break; }
	}

	return hr;
}

/* ========================================
	レンダーターゲットビュー初期化関数
	-------------------------------------
	内容：レンダーターゲットビューの初期化を行う
	-------------------------------------
	戻値：HRESULT型の戻り値
=========================================== */
HRESULT DirectXManager::InitRenderTargetView()
{
	HRESULT	hr;

	// レンダーターゲットクラス作成
	m_pRTV = new RenderTarget();
	if (FAILED(hr = m_pRTV->CreateFromScreen()))
	{
		return hr;	// 失敗
	}

	SetRenderTargets(1, &m_pRTV, nullptr);	// レンダーターゲットの設定

	return hr;
}


/* ========================================
	深度ステンシルビュー初期化関数
	-------------------------------------
	内容：深度ステンシルビューの初期化を行う
	-------------------------------------
	戻値：HRESULT型の戻り値
=========================================== */
HRESULT DirectXManager::InitDepthStencilView()
{
	HRESULT	hr;

	// 深度ステンシルビュークラス作成
	m_pDSV = new DepthStencil();

	// 深度ステンシルビューの作成
	hr = m_pDSV->Create(m_pRTV->GetWidth(), m_pRTV->GetHeight(), false);
	
	
	return hr;
}

/* ========================================
	ビューポート初期化関数
	-------------------------------------
	内容：ビューポートの初期化を行う
=========================================== */
void DirectXManager::InitViewPort()
{
	// ビューポートの設定
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width	= (float)WINDOW_INST.GetScreenSize().nWidth;
	vp.Height	= (float)WINDOW_INST.GetScreenSize().nHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_pContext->RSSetViewports(1, &vp);
}

/* ========================================
	ビューポート初期化関数
	-------------------------------------
	内容：ビューポートの初期化を行う
	-------------------------------------
	引数1：int型の幅
	引数2：int型の高さ
=========================================== */
void DirectXManager::InitViewport(int width, int height)
{
	// ビューポートの設定
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width	= (float)width;
	vp.Height	= (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_pContext->RSSetViewports(1, &vp);
}


/* ========================================
	ラスタライザステート初期化関数
	-------------------------------------
	内容：ラスタライザステートの初期化を行う
	-------------------------------------
	戻値：HRESULT型の戻り値
=========================================== */
HRESULT DirectXManager::InitRasterizerState()
{
	HRESULT	hr;

	// ラスタライザステートの設定(カリング設定以外は先に設定しておく)
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode					= D3D11_FILL_SOLID;	// ポリゴンの塗りつぶし
	rd.FrontCounterClockwise	= FALSE;			// フロントフェイスの時計回り
	rd.DepthBias				= 0;				// 深度バイアス
	rd.DepthBiasClamp			= 0.0f;				// 深度バイアスクランプ
	rd.SlopeScaledDepthBias		= 0.0f;				// スロープスケール深度バイアス
	rd.DepthClipEnable			= TRUE;				// 深度クリッピング有効
	rd.ScissorEnable			= FALSE;			// シザー矩形有効
	rd.MultisampleEnable		= FALSE;			// マルチサンプリング有効
	rd.AntialiasedLineEnable	= FALSE;			// アンチエイリアス有効



	// カリングモードの種類分作成
	for (int i = 0; i < CullMode::CULL_MAX; ++i)
	{
		// カリングモードを設定
		rd.CullMode = CULLING_MODE[i];		
		// ラスタライザステートの作成
		hr = m_pDevice->CreateRasterizerState(&rd, &m_pRasterizerStates[i]);

		// 作成に失敗したら終了
		if (FAILED(hr)) { return hr; }
	}

	// ラスタライザステートを設定
	SetCullingMode(DEFAULT_CULLING_MODE);	// デフォルトは背面カリング

	return hr;
}


/* ========================================
	ブレンドステート初期化関数
	-------------------------------------
	内容：ブレンドステートの初期化を行う
	-------------------------------------
	戻値：HRESULT型の戻り値
=========================================== */
HRESULT DirectXManager::InitBlendState()
{
	HRESULT	hr;

	//--- アルファブレンディング
	// https://pgming-ctrl.com/directx11/blend/

	// レンダーターゲットごとのブレンドステートの設定
	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));
	rtbd.BlendEnable			= TRUE;							// ブレンド有効
	rtbd.BlendOp				= D3D11_BLEND_OP_ADD;			// ブレンド演算
	rtbd.SrcBlendAlpha			= D3D11_BLEND_ONE;				// ソースブレンドアルファ
	rtbd.DestBlendAlpha			= D3D11_BLEND_ZERO;				// デスティネーションブレンドアルファ
	rtbd.BlendOpAlpha			= D3D11_BLEND_OP_ADD;			// ブレンド演算アルファ
	rtbd.RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;	// レンダーターゲットライトマスク

	// ブレンドステートの設定
	D3D11_BLEND_DESC bd = {};
	ZeroMemory(&bd, sizeof(bd));
	bd.AlphaToCoverageEnable	= TRUE;		// アルファ・カバレッジ(透過画像の重なり等)　←重要
	bd.IndependentBlendEnable	= FALSE;	// 独立ブレンド有効(先頭の設定を全てのレンダーターゲットに適用)
	

	// ブレンドモードの種類分作成
	for (int i = 0; i < BLEND_MAX; ++i)
	{
		rtbd.SrcBlend	= BLEND_MODE[i][0];	// ソースブレンド
		rtbd.DestBlend	= BLEND_MODE[i][1];	// デスティネーションブレンド
		bd.RenderTarget[0] = rtbd;		// レンダーターゲットを設定

		// ブレンドステートの作成
		hr = m_pDevice->CreateBlendState(&bd, &m_pBlendState[i]);
		
		// 作成に失敗したら終了
		if (FAILED(hr)) { return hr; }
	}
	
	// ブレンドステートを設定
	SetBlendMode(EFAULT_BLEND_MODE);	// デフォルトはアルファブレンド

	return hr;
}


/* ========================================
	深度ステンシルステート初期化関数
	-------------------------------------
	内容：深度ステンシルステートの初期化を行う
	-------------------------------------
	戻値：HRESULT型の戻り値
=========================================== */
HRESULT DirectXManager::InitDepthStencilState()
{
	HRESULT	hr;

	//--- 深度テスト
	// https://tositeru.github.io/ImasaraDX11/part/ZBuffer-and-depth-stencil
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthFunc						= D3D11_COMPARISON_LESS_EQUAL;		// 深度テスト関数(デフォルトは
	dsDesc.StencilReadMask					= D3D11_DEFAULT_STENCIL_READ_MASK;	// ステンシル読み取りマスク
	dsDesc.StencilWriteMask					= D3D11_DEFAULT_STENCIL_WRITE_MASK;	// ステンシル書き込みマスク
	dsDesc.FrontFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;			// ステンシルテスト失敗時の処理
	dsDesc.FrontFace.StencilDepthFailOp		= D3D11_STENCIL_OP_KEEP;			// ステンシルテスト失敗時の深度処理
	dsDesc.FrontFace.StencilPassOp			= D3D11_STENCIL_OP_INCR;			// ステンシルテスト成功時の処理
	dsDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_GREATER_EQUAL;	// ステンシルテスト関数
	dsDesc.BackFace							= dsDesc.FrontFace;					// バックフェイスの設定
	
	for (int i = 0; i < DepthState::DEPTH_MAX; i++)
	{
		dsDesc.DepthWriteMask		= DEPTH_MASK_PATTERN[i].first;		// 深度書き込みマスク設定
		dsDesc.DepthEnable			= DEPTH_MASK_PATTERN[i].second;	// 深度テスト有効
		dsDesc.StencilEnable		= DEPTH_MASK_PATTERN[i].second;	// ステンシルテスト有効

		// 深度ステートの作成
		hr = m_pDevice->CreateDepthStencilState(&dsDesc, &m_pDepthStencilState[i]);

		// 作成に失敗したら終了
		if (FAILED(hr)) { return hr; }
	}

	// 深度ステートを設定
	SetDepthTest(DEFAULT_DEPTH_STATE);	// デフォルトは深度書き込みとテスト有効


	return hr;
}

/* ========================================
	サンプラーステート初期化関数
	-------------------------------------
	内容：サンプラーステートの初期化を行う
	-------------------------------------
	戻値：HRESULT型の戻り値
=========================================== */
HRESULT DirectXManager::InitSamplerState()
{
	HRESULT	hr;

	// サンプラー
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;		// アドレスU
	samplerDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;		// アドレスV
	samplerDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;		// アドレスW
	samplerDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;			// 比較関数
	samplerDesc.MinLOD			= 0;								// 最小LOD
	samplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;				// 最大LOD
	samplerDesc.MaxAnisotropy	= 1;								// 最大異方性
	samplerDesc.BorderColor[0]	= 0;								// ボーダーカラー
	samplerDesc.BorderColor[1]	= 0;								// ボーダーカラー
	samplerDesc.BorderColor[2]	= 0;								// ボーダーカラー
	samplerDesc.BorderColor[3]	= 0;								// ボーダーカラー

	for (int i = 0; i < SAMPLER_MAX; ++i)
	{
		// フィルタリング設定
		samplerDesc.Filter = SAMPLER_FILTER[i];	

		// サンプラーステートの作成
		hr = m_pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState[i]);

		// 作成に失敗したら終了
		if (FAILED(hr)) { return hr; }
	}

	SetSamplerState(DEFAULT_SAMPLER_STATE);	// デフォルトは線形補間

	return hr;
}


/* ========================================
	DirectX描画開始関数
	-------------------------------------
	内容：描画開始処理を行う
=========================================== */
void DirectXManager::BeginDrawDirectX()
{
	float fClearColor[4];

#ifdef _DEBUG
	DirectX::XMFLOAT4 color = WIN_BASIC_SETTING["BackColor"].GetColor();
	fClearColor[0] = color.x;
	fClearColor[1] = color.y;
	fClearColor[2] = color.z;
	fClearColor[3] = color.w;
#else
	fClearColor[0] = SCREEN_RESET_COLOR[0];
	fClearColor[1] = SCREEN_RESET_COLOR[1];
	fClearColor[2] = SCREEN_RESET_COLOR[2];
	fClearColor[3] = SCREEN_RESET_COLOR[3];
#endif // _DEBUG

	m_pRTV->Clear(fClearColor);
	m_pDSV->Clear();

}

/* ========================================
	DirectX描画開始関数
	-------------------------------------
	内容：描画開始処理を行う
	-------------------------------------
	引数1：float型配列 
=========================================== */
void DirectXManager::BeginDrawDirectX(float* pClearColor)
{
	m_pRTV->Clear(pClearColor);
	m_pDSV->Clear();

}

/* ========================================
	DirectX描画終了関数
	-------------------------------------
	内容：描画終了処理を行う
=========================================== */
void DirectXManager::EndDrawDirectX()
{
	m_pSwapChain->Present(0, 0);
}



/* ========================================
	レンダーターゲット設定関数
	-------------------------------------
	内容：レンダーターゲットを設定する
	-------------------------------------
	引数1：レンダーターゲットの数
	引数2：RenderTarget**型のppViews(配列)
	引数3：DepthStencil*型のpView
=========================================== */
void DirectXManager::SetRenderTargets(UINT num, RenderTarget** ppRTViews, DepthStencil* pDepthStencilView)
{
	static ID3D11RenderTargetView* rtvs[4];

	if (num > 4) num = 4;	// 最大4つまで

	// レンダーターゲットの数だけ取得
	for (UINT i = 0; i < num; ++i)
	{
		rtvs[i] = ppRTViews[i]->GetView();
	}

	// レンダーターゲットの設定
	m_pContext->OMSetRenderTargets(
		num,														// レンダーターゲットの数
		rtvs,														// レンダーターゲット
		pDepthStencilView ? pDepthStencilView->GetView() : nullptr	// 深度ステンシルビュー(ない場合はnullptr)
	);

}

/* ========================================
	深度ステンシルビューオンオフ関数
	-------------------------------------
	内容：深度ステンシルビューの有効無効を
		　切り替える
	-------------------------------------
	引数1：trueなら有効、falseなら無効
=========================================== */
void DirectXManager::OnOffDepthStencilView(bool enable)
{
	static ID3D11RenderTargetView* pRTV = m_pRTV->GetView();	// レンダーターゲットビュー

	// 深度ステンシルビューの有効無効を切り替える
	if (enable)
	{
		m_pContext->OMSetRenderTargets(1, &pRTV, m_pDSV->GetView());
	}
	else
	{
		m_pContext->OMSetRenderTargets(1, &pRTV, nullptr);
	}
}

/* ========================================
	カリングモード設定関数
	-------------------------------------
	内容：カリングモードを設定する
	-------------------------------------
	引数1：CullMode型のcull
=========================================== */
void DirectXManager::SetCullingMode(CullMode cull)
{
	// カリングモードが範囲外の場合は処理を抜ける
	if (cull < 0 || cull >= CULL_MAX) return;

	// 引数の定数に対応するラスタライザステートを設定
	m_pContext->RSSetState(m_pRasterizerStates[cull]);
}

/* ========================================
	深度テスト設定関数
	-------------------------------------
	内容：深度テストを設定する
	-------------------------------------
	引数1：DepthState型のstate
=========================================== */
void DirectXManager::SetDepthTest(DepthState state)
{
	// 深度ステートが範囲外の場合は処理を抜ける
	if (state < 0 || state >= DEPTH_MAX) return;

	// 深度ステンシルステートの設定
	m_pContext->OMSetDepthStencilState(m_pDepthStencilState[state], 0);
}

/* ========================================
	ブレンドモード設定関数
	-------------------------------------
	内容：ブレンドモードを設定する
	-------------------------------------
	引数1：BlendMode型のblend
=========================================== */
void DirectXManager::SetBlendMode(BlendMode blend)
{
	// ブレンドモードが範囲外の場合は処理を抜ける
	if (blend < 0 || blend >= BLEND_MAX) return;

	// ブレンドファクター(色の合成方法)の設定
	FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };

	// ブレンドステートの設定
	m_pContext->OMSetBlendState(m_pBlendState[blend], blendFactor, 0xffffffff);
}

/* ========================================
	サンプラーステート設定関数
	-------------------------------------
	内容：サンプラーステートを設定する
	-------------------------------------
	引数1：SamplerState型のstate
=========================================== */
void DirectXManager::SetSamplerState(SamplerState state)
{
	// サンプラーステートが範囲外の場合は処理を抜ける
	if (state < 0 || state >= SAMPLER_MAX) return;

	// サンプラーステートの設定
	m_pContext->PSSetSamplers(0, 1, &m_pSamplerState[state]);
}


/* ========================================
	ゲッター(デバイス)関数
	-------------------------------------
	戻値：ID3D11Device*型
=========================================== */
ID3D11Device* DirectXManager::GetDevice()
{
	return m_pDevice;
}

/* ========================================
	ゲッター(デバイスコンテキスト)関数
	-------------------------------------
	戻値：ID3D11DeviceContext*型
=========================================== */
ID3D11DeviceContext* DirectXManager::GetContext()
{
	return m_pContext;
}

/* ========================================
	ゲッター(スワップチェイン)関数
	-------------------------------------
	戻値：IDXGISwapChain*型
=========================================== */
IDXGISwapChain* DirectXManager::GetSwapChain()
{
	return m_pSwapChain;
}

/* ========================================
	ゲッター(レンダーターゲット)関数
	-------------------------------------
	戻値：RenderTarget*型
=========================================== */
RenderTarget* DirectXManager::GetDefaultRTV()
{
	return m_pRTV;
}

/* ========================================
	ゲッター(深度ステンシル)関数
	-------------------------------------
	戻値：DepthStencil*型
=========================================== */
DepthStencil* DirectXManager::GetDefaultDSV()
{
	return m_pDSV;
}