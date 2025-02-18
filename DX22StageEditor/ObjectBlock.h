/* ========================================
	CatRobotGame/
	------------------------------------
	ブロックオブジェクト用ヘッダ
	------------------------------------
	説明：ブロックオブジェクトの基底クラス
	------------------------------------
	ObjectBlock.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBase.h"

// =============== 前方宣言 =======================
class ComponentGeometry;
class ComponentCollisionBase;

// =============== クラス定義 ===================
class ObjectBlock
	: public ObjectBase
{
public:
	enum E_COL_TYPE
	{
		COL_OBB,
		COL_AABB,
	};

	struct S_SaveData
	{
		int nTextureID[6];			// テクスチャID
		bool bUseTex;				// テクスチャ使用フラグ
		E_COL_TYPE eColType;		// 衝突判定形状
		Vector2<float> vUvScale[3];	// テクスチャスケール
		Vector2<float> vUvOffset[3];	// テクスチャオフセット
	};
public:
	ObjectBlock(SceneBase* pScene);

	void InitLocal() override;

	void CopyLocal(ObjectBase* pObject) override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectBlock)	// オブジェクトの種類ID取得関数
	
protected:
	ComponentGeometry*		m_pCompGeometry;
	ComponentCollisionBase*	m_pCompColBase;

	E_COL_TYPE m_eColType;	// 衝突判定形状
};

