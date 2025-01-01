/* ========================================
	CatRobotGame/
	------------------------------------
	カメラオブジェクト(クリア時演出)用ヘッダ
	------------------------------------
	説明：ステージをクリアした際のカメラ演出
		　プレイヤーに近づいてズームアウトする(マリオ64風)
	------------------------------------
	ObjectCameraGameClear.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectCamera.h"

// =============== クラスの定義 =====================
class ObjectCameraGameClear :
    public ObjectCamera
{
public:
	ObjectCameraGameClear(SceneBase* pScene);

	void InitLocal() override;
	void UpdateLocal() override;

	DEFINE_OBJECT_TYPE(ObjectCameraGameClear)	// オブジェクトの種類ID取得関数

private:
	void EventMoveNearPlayer();
	void EventZoomOutStart();
private:
	ObjectBase* m_pTarget;	// ターゲット(プレイヤー

	float	m_fStandByTimeCnt;	// 待機時間カウント
	bool	m_bZoomOutStart;	// ズームアウト開始
};

