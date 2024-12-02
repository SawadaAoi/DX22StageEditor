/* ========================================
	DX22Base/
	------------------------------------
	カメラ管理用ヘッダ
	------------------------------------
	説明:カメラを管理するクラス(適宜切り替える)
	------------------------------------
	CameraManager.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectCamera.h"	// カメラオブジェクト

// =============== 前方宣言 =======================
class SceneBase;

// =============== 定数定義 =====================
#define CAMERA_MNG_INST CCameraManager::GetInstance()	// instance取得


// =============== クラス定義 =====================
class CCameraManager
{
public:
	static CCameraManager& GetInstance();	// インスタンス取得
	void Init(SceneBase* pScene);	// シーン開始時にカメラ初期化
	
	void SwitchCamera(int num);	// カメラ切り替え

	void AddCamera(ObjectCamera* pCamera);	// カメラ追加
	void RemoveCamera(ObjectCamera* pCamera);	// カメラ削除

	// ゲッター
	ObjectCamera* GetActiveCamera();
	int GetCameraNum();

private:
	CCameraManager();	// コンストラクタ	
	void ResetActiveCamera();	// アクティブカメラをリセット

	std::vector<ObjectCamera*> m_pCameraList;	// カメラリスト
	SceneBase* m_pScene;	// シーンクラスのポインタ

	ObjectCamera* m_pActiveCamera;	// アクティブカメラ
};

