/* ========================================
	DX22Base/
	------------------------------------
	ライトオブジェクト(ポイントライト)用ヘッダ
	------------------------------------
	説明：ライトオブジェクトの処理
	------------------------------------
	ObjectLightPoint.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectLight.h"

// =============== クラス定義 =====================
class ObjectLightPoint :
    public ObjectLight
{
public:
	ObjectLightPoint(SceneBase* pScene);
	void InitLocal() override;

	DEFINE_OBJECT_TYPE(ObjectLightPoint)
};

