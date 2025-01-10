/* ========================================
	CatRobotGame/
	------------------------------------
	壁オブジェクト用ヘッダ
	------------------------------------
	説明：壁(プレイヤーが落ちない為)
	------------------------------------
	ObjectWall.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBase.h"
#include <vector>

// =============== 前方宣言 =======================
class ComponentCollisionOBB;
class ComponentGeometry;		// 見た目

// =============== クラス定義 =====================
class ObjectWall :
	public ObjectBase
{
public:
	ObjectWall(SceneBase* pScene);

	void InitLocal() override;
	void UpdateLocal() override;

	DEFINE_OBJECT_TYPE(ObjectWall)
private:
	ComponentCollisionOBB*	m_pCompColOBB;
	ComponentGeometry*		m_pCompGeometry;

};

