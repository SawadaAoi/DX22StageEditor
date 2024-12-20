/* ========================================
	CatRobotGame/
	------------------------------------
	カメラオブジェクト(プレイヤー)用ヘッダ
	------------------------------------
	説明：プレイヤーに追従するカメラ
		　プレイヤーを中心に横回転、縦回転が可能
	------------------------------------
	ObjectCameraPlayer.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectCamera.h"

// =============== クラスの定義 =====================
class ObjectCameraPlayer :
    public ObjectCamera
{
public:
	ObjectCameraPlayer(SceneBase* pScene);

	void InitLocal() override;
	void UpdateLocal() override;

	// ゲッター
	float GetDistance();	// カメラとプレイヤーの距離取得関数
	float GetRadY();		// カメラのY軸回転角度取得関数
	float GetRadXZ();		// カメラのXZ平面回転角度取得関数

	// セッター
	void SetDistance(float fDistance);	// カメラとプレイヤーの距離設定関数
	void SetRadY(float fRadY);			// カメラのY軸回転角度設定関数
	void SetRadXZ(float fRadXZ);		// カメラのXZ平面回転角度設定関数

	DEFINE_OBJECT_TYPE(ObjectCameraPlayer)	// オブジェクトの種類ID取得関数
private:
	void CheckLimitRadY();	// Y軸回転角度制限関数
private:
	ObjectBase* m_pPlayer;

	float m_fRotSpeed;	// 回転速度
	float m_fDistance;	// カメラとプレイヤーの距離
	float m_fRadY;		// カメラのY軸回転角度
	float m_fRadXZ;		// カメラのXZ平面回転角度
};

