/* ========================================
	DX22Base/
	------------------------------------
	地面オブジェクト用ヘッダ
	------------------------------------
	説明：プレイヤーが立つ地面オブジェクト
	------------------------------------
	ObjectGround.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectBase.h"
#include "Vector3.h"
#include <memory>
#include "ShapeLine.h"

// =============== クラス定義 =====================
class ObjectGround :
    public ObjectBase
{
public:
	ObjectGround(SceneBase* pScene);
	~ObjectGround() {};

	void InitLocal() override;
	void UninitLocal() override;
	void UpdateLocal() override;
	void DrawLocal() override;

	DEFINE_OBJECT_TYPE(ObjectGround)	// オブジェクトの種類ID取得関数

private:
	class ComponentTransform* m_pTransform;	// 座標、回転、スケール
	class ComponentGeometry* m_pGeometry;	// 形状
};

