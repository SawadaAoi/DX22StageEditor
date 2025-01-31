/* ========================================
	DX22Base/
	------------------------------------
	ビルボード用ヘッダ
	------------------------------------
	説明：常にカメラの方向を向く画像をワールド空間上に描画する
	------------------------------------
	ComponentBillboard.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentBase.h"
#include "Vector2.h"
#include "Vector3.h"
#include <memory>

// =============== 前方宣言 =======================
class ComponentTransform;	// トランスフォームコンポーネント
class Texture;				// テクスチャ
class VertexShader;			// 頂点シェーダ
class PixelShader;			// ピクセルシェーダ
class MeshBuffer;			// メッシュバッファ


// =============== クラス =======================
class ComponentBillboard :
    public ComponentBase
{
public:
	ComponentBillboard(ObjectBase* pOwner);
	virtual void Init() override;
	virtual void Draw() override;

	// ゲッター
	Texture* GetTexture();
	Vector3<float> GetPosOffset();
	Vector2<float> GetUvPos();
	Vector2<float> GetUvScale();
	Vector3<float> GetColor();
	float GetAlpha();

	// セッター
	void SetTexture(Texture* pTexture);
	void SetPosOffset(Vector3<float> offset);
	void SetUvPos(Vector2<float> uvPos);
	void SetUvScale(Vector2<float> uvScale);
	void SetColor(Vector3<float> color);
	void SetAlpha(float alpha);

	DEFINE_COMPONENT_TYPE

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	DebugUI::Item* InitTextureList();
#endif // _DEBUG
protected:
	void InitMesh();
	DirectX::XMFLOAT4X4 CreateWorldMatrix();
protected:
	ComponentTransform*			m_pCompTran;	// トランスフォームコンポーネント
	Texture*					m_pTexture;		// テクスチャ
	std::shared_ptr<MeshBuffer> m_pMesh;		// メッシュバッファ

	VertexShader*		m_pVS;		// 頂点シェーダ
	PixelShader*		m_pPS;		// ピクセルシェーダ

	Vector3<float>		m_vPosOffset;	// オフセット


	Vector2<float>		m_vUvPos;	// UV座標
	Vector2<float>		m_vUvScale;	// UVスケール
	Vector3<float>		m_vColor;	// 色
	float				m_fAlpha;	// 透過度
};

