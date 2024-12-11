/* ========================================
	DX22Base/
	------------------------------------
	ライト基本コンポーネント用ヘッダ
	------------------------------------
	説明：ライトの基本機能を提供する
	------------------------------------
	ComponentLightBase.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentBase.h"
#include <memory>

// =============== 前方宣言 =======================
class ComponentTransform;
class ShapeLine;
class ShapeSphere;

// =============== クラス定義 =====================
class ComponentLightBase :
    public ComponentBase
{
public:
	enum E_LightType
	{
		NONE,			// なし
		DIRECTIONAL,	// ディレクショナルライト
		POINT,			// ポイントライト
		SPOT,			// スポットライト
	};
public:
    ComponentLightBase(ObjectBase* pOwner);

    virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() override;

	// ゲッター
	E_LightType GetLightType() const;
	Vector3<float> GetColor() const;
	float GetRange() const;
	float GetAngle() const;

	// セッター
	void SetLightType(E_LightType type);
	void SetColor(const Vector3<float>& color);
	void SetRange(float range);
	void SetAngle(float angle);

	DEFINE_COMPONENT_TYPE
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	ComponentTransform* m_pCompTransform;	// トランスフォームコンポーネント
	E_LightType			m_eLightType;		// ライトの種類
	Vector3<float>		m_vColor;			// ディフューズカラー
	float				m_fRange;			// ライトの有効範囲
	float				m_fAngle;			// スポットライトの角度

	bool m_bDispLine;						// ラインの表示フラグ
	std::unique_ptr<ShapeLine>	m_pDirLine;	// ライトの向きの描画用
	std::unique_ptr<ShapeSphere> m_pSphere;	// ライトの有効範囲の描画用
};

