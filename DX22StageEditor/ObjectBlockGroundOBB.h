/* ========================================
	DX22Base/
	------------------------------------
	OBBブロックオブジェクト(地面判定有り)用ヘッダ
	------------------------------------
	説明：回転あり、地面判定有りのブロック
	------------------------------------
	ObjectBlockGroundOBB.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBlock.h"

// =============== クラス定義 ===================
class ObjectBlockGroundOBB :
    public ObjectBlock
{
public:
	ObjectBlockGroundOBB(SceneBase* pScene);
	void InitLocal() override;
	DEFINE_OBJECT_TYPE(ObjectBlockGroundOBB)
};

