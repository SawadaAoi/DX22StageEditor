/* ========================================
	DX22Base/
	------------------------------------
	図形描画用cpp
	------------------------------------
	Shape.cpp
========================================== */

// =============== インクルード ===================
#include "ShapeBase.h"
#include "ShaderManager.h"
#include "CameraManager.h"
#include "DirectXManager.h"
#include "TextureManager.h"	

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：無し
=========================================== */
ShapeBase::ShapeBase()
	: m_pMeshBuffer{ nullptr, nullptr }
	, m_pVS(nullptr)
	, m_pPS(nullptr)
	, m_WVP()
	, m_vPosition(0.0f, 0.0f, 0.0f)
	, m_vScale(1.0f, 1.0f, 1.0f)
	, m_qRotation({0.0f,0.0f,0.0f}, 1.0f)
	, m_fColor(1.0f, 1.0f, 1.0f)
	, m_eDrawMode(E_DrawMode::DRAW_NORMAL)
	, m_pTexture(nullptr)
	, m_bIsTex(false)
{
	// 頂点シェーダー読み込み
	m_pVS = GET_VS_DATA(VS_KEY::VS_SHAPE);
	// ピクセルシェーダー読み込み
	m_pPS = GET_PS_DATA(PS_KEY::PS_SHAPE);

	//m_pTexture = GET_TEXTURE_DATA(TEX_KEY::TEST);	// デフォルトテクスチャ
}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：解放処理
=========================================== */
ShapeBase::~ShapeBase()
{
	SAFE_DELETE(m_pMeshBuffer[E_DrawMode::DRAW_NORMAL]);
	SAFE_DELETE(m_pMeshBuffer[E_DrawMode::DRAW_WIRE_FRAME]);
}


/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
=========================================== */
void ShapeBase::Draw()
{
	DirectXManager::OnOffDepthStencilView(true);	// 深度バッファの有効化

	SetWVPMatrix();	// WVP行列の設定

	// 定数バッファ(ワールド、ビュー、プロジェクション行列)の書き込み
	m_pVS->WriteBuffer(0, m_WVP);

	// 定数バッファ(色、表示モード、テクスチャ使用フラグ)の書き込み
	float	fData[3] = { m_fColor.x, m_fColor.y, m_fColor.z };	// 色
	int		nData[2] = { m_eDrawMode, m_bIsTex };				// 表示モード、テクスチャ使用フラグ
	m_pPS->WriteBuffer(0, fData);
	m_pPS->WriteBuffer(1, nData);
	m_pPS->SetTexture(0, m_pTexture);

	// シェーダーのバインド(シェーダーの設定を GPU に送る)
	m_pVS->Bind();
	m_pPS->Bind();
	
	// 頂点バッファ、インデックスバッファをGPUに設定し、描画
	m_pMeshBuffer[m_eDrawMode]->Draw();
}


/* ========================================
	WVP行列設定関数
	-------------------------------------
	内容：ワールド、ビュー、プロジェクション行列を設定する
=========================================== */
void ShapeBase::SetWVPMatrix()
{
	// ワールド行列の計算
	DirectX::XMStoreFloat4x4(&m_WVP[0],												// Float4x4に変換して格納
		DirectX::XMMatrixTranspose(													// 転地
			DirectX::XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z) *			// 拡大縮小
			m_qRotation.ToDirectXMatrix() *											// クォータニオン回転
			DirectX::XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z)				// 移動
		));

	m_WVP[1] = CAMERA_MNG_INST.GetActiveCamera()->GetViewMatrix();
	m_WVP[2] = CAMERA_MNG_INST.GetActiveCamera()->GetProjectionMatrix();

}


/* ========================================
	ゲッター(座標)関数
	-------------------------------------
	戻値：Vector3<float> fPos
=========================================== */
Vector3<float> ShapeBase::GetPosition()
{
	return m_vPosition;
}

/* ========================================
	ゲッター(サイズ)関数
	-------------------------------------
	戻値：Vector3<float> fSize
=========================================== */
Vector3<float> ShapeBase::GetScale()
{
	return m_vScale;
}

/* ========================================
	ゲッター(回転)関数
	-------------------------------------
	戻値：Quaternion m_fRotation
=========================================== */
Quaternion ShapeBase::GetRotation()
{
	return m_qRotation;
}

/* ========================================
	ゲッター(表示モード)関数
	-------------------------------------
	戻値：E_DrawMode eMode
=========================================== */
Vector3<float> ShapeBase::GetColor()
{
	return m_fColor;
}

/* ========================================
	ゲッター(表示モード)関数
	-------------------------------------
	戻値：E_DrawMode eMode
=========================================== */
ShapeBase::E_DrawMode ShapeBase::GetDrawMode()
{
	return m_eDrawMode;
}



/* ========================================
	セッター(座標)関数
	-------------------------------------
	引数：Vector3<float> fPos
=========================================== */
void ShapeBase::SetPosition(Vector3<float> fPos)
{
	m_vPosition = fPos;
}

/* ========================================
	セッター(サイズ)関数
	-------------------------------------
	引数：Vector3<float> fSize
=========================================== */
void ShapeBase::SetScale(Vector3<float> fSize)
{
	m_vScale = fSize;
}

/* ========================================
	セッター(回転)関数
	-------------------------------------
	引数：Quaternion fRot
=========================================== */
void ShapeBase::SetRotation(Quaternion fRot)
{
	m_qRotation = fRot;
}

/* ========================================
	セッター(色)関数
	-------------------------------------
	引数：Vector3<float> fColor
=========================================== */
void ShapeBase::SetColor(Vector3<float> fColor)
{
	m_fColor = fColor;
}

/* ========================================
	セッター(表示モード)関数
	-------------------------------------
	引数：E_DrawMode eMode
=========================================== */
void ShapeBase::SetDrawMode(E_DrawMode eMode)
{
	m_eDrawMode = eMode;
}

/* ========================================
	セッター(テクスチャ)関数
	-------------------------------------
	引数：Texture* pTexture
=========================================== */
void ShapeBase::SetTexture(Texture* pTexture)
{
	m_pTexture = pTexture;
}

/* ========================================
	セッター(テクスチャ使用フラグ)関数
	-------------------------------------
	引数：int bIsTex
=========================================== */
void ShapeBase::SetUseTexture(int bIsTex)
{
	m_bIsTex = bIsTex;
}
