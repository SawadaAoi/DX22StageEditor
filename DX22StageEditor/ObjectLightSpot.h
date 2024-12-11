/* ========================================
	DX22Base/
	------------------------------------
	ライトオブジェクト(スポットライト)用ヘッダ
	------------------------------------
	説明：ライトオブジェクトの処理
	------------------------------------
	ObjectLightSpot.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectLight.h"

// =============== クラス定義 =====================
class ObjectLightSpot :
    public ObjectLight
{
public:
	ObjectLightSpot(SceneBase* pScene);
	void InitLocal() override;

	DEFINE_OBJECT_TYPE(ObjectLightSpot)
};

