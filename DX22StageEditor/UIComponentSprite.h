/* ========================================
	DX22Base/
	------------------------------------
	UI画像用ヘッダ
	------------------------------------
	説明：UI画像を描画するためのコンポーネント
	------------------------------------
	UIComponentSprite.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "UIComponentBase.h"
#include <memory>

#include "Texture.h"

#include "Vector2.h"
#include "Vector3.h"


// =============== 前方宣言 =======================
class UIComponentTransform;
class VertexShader;
class PixelShader;
class MeshBuffer;

// =============== クラス =======================
class UIComponentSprite :
    public UIComponentBase
{
public:
	UIComponentSprite(UIObjectBase* owner);

	void Init();
	void Draw();

	// ゲッター
	Texture* GetTexture();
	bool GetIsVisible();
	Vector2<float> GetOffset();
	Vector2<float> GetUvPos();
	Vector2<float> GetUvScale();
	Vector3<float> GetColor();
	float GetAlpha();

	// セッター
	void SetTexture(Texture* pTexture);
	void SetIsVisible(bool visible);
	void SetOffset(Vector2<float> offset);
	void SetUvPos(Vector2<float> uvPos);
	void SetUvScale(Vector2<float> uvScale);
	void SetColor(Vector3<float> color);
	void SetAlpha(float alpha);

	DEFINE_UI_COMPONENT_TYPE

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	DebugUI::Item* InitTextureList();
#endif // _DEBUG
private:
	void InitMesh();
	DirectX::XMFLOAT4X4 CreateWorldMatrix();
private:
	UIComponentTransform* m_pCompTran;

	std::shared_ptr<MeshBuffer> m_pMesh;	// メッシュバッファ
	Texture* m_pTexture;	// テクスチャ

	VertexShader*	m_pVS;
	PixelShader*	m_pPS;

	bool				m_bIsVisible;	// 表示フラグ

	Vector2<float>		m_vOffset;	// オフセット
	Vector2<float>		m_vUvPos;	// UV座標
	Vector2<float>		m_vUvScale;	// UVスケール
	Vector3<float>		m_vColor;	// 色
	float				m_fAlpha;	// 透過度

};

