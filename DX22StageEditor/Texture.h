/* ========================================
	DX22Base/
	------------------------------------
	テクスチャ用ヘッダ
	------------------------------------
	説明：テクスチャを扱うクラス
	------------------------------------
	Texture.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "DirectXManager.h"	// DirectXManager
#include "DirectXTex/TextureLoad.h"


// =============== クラス定義 =====================
class Texture
{
public:
	Texture();
	virtual ~Texture();
	// コピーコンストラクタ
	Texture(const Texture& other);


	HRESULT Create(const char* fileName);
	HRESULT Create(DXGI_FORMAT format, UINT width, UINT height, const void* pData = nullptr);


	// ゲッター
	UINT GetWidth() const;
	UINT GetHeight() const;
	ID3D11ShaderResourceView* GetResource() const;

protected:
	D3D11_TEXTURE2D_DESC MakeTexDesc(DXGI_FORMAT format, UINT width, UINT height);
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData);	// 継承先で処理を変更するため、仮想関数にする

protected:
	UINT						m_width;		// 横幅
	UINT						m_height;		// 縦幅
	ID3D11Texture2D*			m_pTex;			// テクスチャ(レンダーターゲット、深度ステンシルで使用)

private:
	ID3D11ShaderResourceView* m_pSRV;			// シェーダリソースビュー(シェーダーに渡すためのビュー)

};

