/* ========================================
	DX22Base/
	------------------------------------
	レンダーターゲットビュー用ヘッダ
	------------------------------------
	説明：レンダーターゲットを扱うクラス
		　(描画結果を保存するためのバッファ)
	------------------------------------
	RenderTarget.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "Texture.h"	// 親クラス

// =============== クラス定義 =====================
class RenderTarget : public Texture
{
public:
	RenderTarget();
	~RenderTarget();
	void Clear();
	void Clear(const float* color);
	HRESULT Create(DXGI_FORMAT format, UINT width, UINT height);
	HRESULT CreateFromScreen();
	ID3D11RenderTargetView* GetView() const;

protected:
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData = nullptr);

private:
	ID3D11RenderTargetView* m_pRTV; // レンダーターゲットビュー
};

