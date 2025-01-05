/* ========================================
	DX22Base/
	------------------------------------
	AABBブロックオブジェクト(地面判定有り)用ヘッダ
	------------------------------------
	説明：回転しない、地面判定有りのブロック
	------------------------------------
	ObjectBlockGroundAABB.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBlock.h"

// =============== クラス定義 ===================
class ObjectBlockGroundAABB :
    public ObjectBlock
{
public:
	ObjectBlockGroundAABB(SceneBase* pScene);
	void InitLocal() override;
	DEFINE_OBJECT_TYPE(ObjectBlockGroundAABB)
};

