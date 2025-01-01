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
class ObjectBase;

// =============== 定数定義 =====================
#define CAMERA_MNG_INST CameraManager::GetInstance()	// instance取得


// =============== クラス定義 =====================
class CameraManager
{
public:
	static CameraManager& GetInstance();	// インスタンス取得
	void Init(SceneBase* pScene);	// シーン開始時にカメラ初期化
	
	void SwitchCamera(int num);	// カメラ切り替え
	void SwitchCamera(ObjectCamera* pCamera);	// カメラ切り替え

	void AddCamera(ObjectCamera* pCamera);	// カメラ追加
	void RemoveCamera(ObjectCamera* pCamera);	// カメラ削除

	void FocusMoveCamera(ObjectBase* pObj);	// カメラを指定オブジェクトにフォーカス移動

	// ゲッター
	ObjectCamera* GetActiveCamera();
	int GetCameraNum();

private:
	CameraManager();	// コンストラクタ	
	void ResetActiveCamera();	// アクティブカメラをリセット

	std::vector<ObjectCamera*> m_pCameraList;	// カメラリスト
	SceneBase* m_pScene;	// シーンクラスのポインタ

	ObjectCamera* m_pActiveCamera;	// アクティブカメラ
};

