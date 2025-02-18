/* ========================================
	CatRobotGame/
	------------------------------------
　　オブジェクト(ステージ見本親)用h
	------------------------------------
	説明：ステージセレクト画面で表示する
		　各ステージ見本の親オブジェクト
	------------------------------------
	ObjectStageSampleParent.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBase.h"

// =============== クラスの定義 ===================
class ObjectStageSampleParent :
    public ObjectBase
{
public:
	ObjectStageSampleParent(SceneBase* pScene);
	void UpdateLocal() override;

	DEFINE_OBJECT_TYPE(ObjectStageSampleParent)
#ifdef _DEBUG
	void DebugLocal(DebugUI::Window& window);
#endif // DEBUG
private:
	float	m_fRotateSpeed;
	bool	m_bIsRotate;
};

