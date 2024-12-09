/* ========================================
	DX22Base/
	------------------------------------
	地面オブジェクト(草原)用ヘッダ
	------------------------------------
	説明：レイが当たる地面オブジェクト
	------------------------------------
	ObjectGroundGrass.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectGround.h"

// =============== クラス定義 =====================
class ObjectGroundGrass :
    public ObjectGround
{
public:
	ObjectGroundGrass(SceneBase* pScene);
	void InitLocal() override;

	DEFINE_OBJECT_TYPE(ObjectGroundGrass)	// オブジェクトの種類ID取得関数
};

