/* ========================================
	DX22Base/
	------------------------------------
	デプスステンシルビュー用ヘッダ
	------------------------------------
	説明：デプスステンシルビューを扱うクラス
		　(深度テストやステンシルテストを行うために必要なビュー)
		　深度テスト: 近いオブジェクトを優先して表示し、隠れた部分を隠すことで、よりリアルな描画を実現します。
		　ステンシルテスト: 特定の領域に対してのみ描画を行うなど、より高度な描画制御を可能にします。
	------------------------------------
	DepthStencil.h
========================================== */
#pragma once


// =============== インクルード ===================
#include "Texture.h"


// =============== クラス定義 =====================
class DepthStencil : public Texture
{
public:
	DepthStencil();
	~DepthStencil();
	void Clear();
	HRESULT Create(UINT width, UINT height, bool useStencil);
	ID3D11DepthStencilView* GetView() const;

protected:
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData = nullptr);

private:
	ID3D11DepthStencilView* m_pDSV;
};

