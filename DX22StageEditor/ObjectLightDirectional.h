/* ========================================
	DX22Base/
	------------------------------------
	ライトオブジェクト(方向ライト)用ヘッダ
	------------------------------------
	説明：ライトオブジェクトの処理
	------------------------------------
	ObjectLightDirectional.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectLight.h"

// =============== クラス定義 =====================
class ObjectLightDirectional :
    public ObjectLight
{
public:
	ObjectLightDirectional(SceneBase* pScene);
	void InitLocal() override;

	DEFINE_OBJECT_TYPE(ObjectLightDirectional)

};

