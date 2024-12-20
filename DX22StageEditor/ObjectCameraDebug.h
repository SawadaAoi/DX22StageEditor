/* ========================================
	CatRobotGame/
	------------------------------------
	カメラオブジェクト(デバッグ)用ヘッダ
	------------------------------------
	説明：マウス、キーで自由に動かせるカメラ
	------------------------------------
	ObjectCameraDebug.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectCamera.h"

// =============== クラスの定義 =====================
class ObjectCameraDebug :
    public ObjectCamera
{
public:
	ObjectCameraDebug(SceneBase* pScene);
	void InitLocal() override;

	DEFINE_OBJECT_TYPE(ObjectCameraDebug)	// オブジェクトの種類ID取得関数
};

