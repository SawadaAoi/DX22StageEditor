/* ========================================
	DX22Base/
	------------------------------------
	図形表示コンポーネント用ヘッダ
	------------------------------------
	説明：指定された図形を表示する
	------------------------------------
	ComponentGeometry.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentBase.h"	// コンポーネント基底クラス
#include "ShapeBase.h"		// 図形基底クラス
#include <memory>

// =============== クラス定義 ===================
class ComponentGeometry :
	public ComponentBase
{
public:
	enum E_ShapeType
	{
		TYPE_NONE = 0,		// なし
		TYPE_BOX,			// 立方体
		TYPE_SPHERE,		// 球
		TYPE_PLANE,			// 平面

		TYPE_MAX,			// 図形の最大数
	};
public:
	ComponentGeometry(ObjectBase* pOwner);

	void Init() override;
	void Update() override;
	void Draw() override;

	// ゲッター
	void SetShapeType(E_ShapeType eType);	// 図形の種類を設定	

	DEFINE_COMPONENT_TYPE	// コンポーネントの種類ID取得関数

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	DebugUI::Item* InitShapeTypeList();
	DebugUI::Item* InitTextureList();
#endif // _DEBUG
private:
	void CreateShape();	// 図形オブジェクトを生成

	std::shared_ptr<ShapeBase> m_pShape;	// 図形オブジェクト
	class ComponentTransform* m_pTransform;	// 座標コンポーネント

	Texture*	m_pTexture;	// テクスチャ
	bool		m_bIsTex;	// テクスチャ使用フラグ

	E_ShapeType m_eType;	// 図形の種類

};

