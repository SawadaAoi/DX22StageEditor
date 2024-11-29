/* ========================================
	DX22Base/
	------------------------------------
	プレイヤーオブジェクト用ヘッダ
	------------------------------------
	説明：プレイヤーオブジェクトの処理を管理する
	------------------------------------
	ObjectPlayer.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectBase.h"
#include "ComponentTransform.h"
#include <memory>

// =============== クラス定義 =====================
class ObjectPlayer :
    public ObjectBase
{
public:
	ObjectPlayer(SceneBase* pScene);
	void InitLocal();
	void UpdateLocal();
	void DrawLocal();

	// ゲッター
	DEFINE_OBJECT_TYPE	// オブジェクトの種類ID取得関数

private:
	ComponentTransform* m_pTransform;
	class ComponentGeometry* m_pGeometry;
	std::unique_ptr<class ShapeLine> m_pLine;
	class ComponentGroundRaycast* m_pGroundRaycast;

};

