/* ========================================
	DX22Base/
	------------------------------------
	DirectX管理用ヘッダ
	------------------------------------
	説明:DirextXの管理を行う
	------------------------------------
	DirectXManager.h
========================================== */
#pragma once

// =============== インクルード ===================
#include <d3d11.h>					// DirectX11のヘッダ
#pragma comment(lib, "d3d11.lib")	// DirectX11のライブラリ

// =============== 定数定義 =======================
#define SAFE_DELETE(p) do{if(p){delete p; p = nullptr;}}while(0)			// メモリ解放マクロ
#define SAFE_DELETE_ARRAY(p) do{if(p){delete[] p; p = nullptr;}}while(0)	// 配列メモリ解放マクロ
#define SAFE_RELEASE(p) do{if(p){p->Release(); p = nullptr;}}while(0)		// リソース解放マクロ

class RenderTarget;
class DepthStencil;

// =============== クラス定義 =====================
class DirectXManager
{
public:
	// ブレンドモード
	enum BlendMode
	{
		BLEND_NONE,			// ブレンド無し
		BLEND_ALPHA,		// アルファブレンド
		BLEND_ADD,			// 加算ブレンド
		BLEND_ADDALPHA,		// 加算アルファブレンド
		BLEND_SUB,			// 減算ブレンド
		BLEND_SCREEN,		// スクリーンブレンド
		BLEND_MAX
	};

	// カリングモード
	enum CullMode
	{
		CULL_NONE,		// カリング無し
		CULL_FRONT,		// 前面カリング
		CULL_BACK,		// 裏面カリング
		CULL_MAX
	};

	// サンプラーステート
	enum SamplerState
	{
		SAMPLER_LINEAR,	// 線形補間
		SAMPLER_POINT,	// 最近傍補間
		SAMPLER_MAX
	};

	// 深度ステート
	enum DepthState
	{
		DEPTH_ENABLE_WRITE_TEST,	// 深度書き込みとテスト有効
		DEPTH_ENABLE_TEST,			// 深度テストのみ有効
		DEPTH_DISABLE,				// 深度無効
		DEPTH_MAX
	};

public:

	static HRESULT InitDirectX();
	static void UninitDirectX();
	static void BeginDrawDirectX();
	static void BeginDrawDirectX(float* pClearColor);
	static void EndDrawDirectX();

	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetContext();
	static IDXGISwapChain* GetSwapChain();
	static RenderTarget* GetDefaultRTV();
	static DepthStencil* GetDefaultDSV();
	static void SetRenderTargets(UINT num, RenderTarget** ppViews, DepthStencil* pView);
	static void OnOffDepthStencilView(bool enable);

	static void SetCullingMode(CullMode cull);
	static void SetDepthTest(DepthState state);
	static void SetBlendMode(BlendMode blend);
	static void SetSamplerState(SamplerState state);

private:
	static HRESULT	InitDeviceAndSwapChain();
	static HRESULT	InitRenderTargetView();
	static HRESULT	InitDepthStencilView();
	static void		InitViewPort();
	static void		InitViewport(int width, int height);
	static HRESULT	InitRasterizerState();
	static HRESULT	InitBlendState();
	static HRESULT	InitDepthStencilState();
	static HRESULT	InitSamplerState();

	static inline ID3D11Device*				m_pDevice;		// デバイス
	static inline ID3D11DeviceContext*		m_pContext;		// デバイスコンテキスト
	static inline IDXGISwapChain*			m_pSwapChain;	// スワップチェイン
	static inline D3D_FEATURE_LEVEL			m_FeatureLevel;	// フィーチャーレベル

	static inline RenderTarget*				m_pRTV;			// レンダーターゲット
	static inline DepthStencil*				m_pDSV;			// 深度ステンシル

	static inline ID3D11RasterizerState*				m_pRasterizerStates[CULL_MAX];			// ラスタライザステート(配列)
	static inline ID3D11BlendState*						m_pBlendState[BLEND_MAX];		// ブレンドステート
	static inline ID3D11SamplerState*					m_pSamplerState[SAMPLER_MAX];	// サンプラーステート

	static inline ID3D11DepthStencilState*				m_pDepthStencilState[DEPTH_MAX];	// 深度ステート

	static inline D3D11_VIEWPORT						m_ViewPort;		// ビューポート

};