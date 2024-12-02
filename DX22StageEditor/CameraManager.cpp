/* ========================================
	DX22Base/
	------------------------------------
	カメラ管理用cpp
	------------------------------------
	CameraManager.cpp
========================================== */

// =============== インクルード ===================
#include "CameraManager.h"	
#include "SceneBase.h"		// シーン基底クラス
#include "DebugMenu.h"

// =============== 定数定義 =======================
const std::string DEFAULT_CAMERA_NAME = "DefaultCamera";	// デフォルトカメラ名

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：無し
=========================================== */
CCameraManager::CCameraManager()
	: m_pScene(nullptr)	// シーンクラスのポインタを初期化
	, m_pCameraList()	// カメラリストを初期化
	, m_pActiveCamera(nullptr)	// アクティブカメラを初期化
{
}

/* ========================================
	インスタンス(シングルトン)取得関数
	-------------------------------------
	内容：自クラスのインスタンスを取得
	-------------------------------------
	戻値：自クラスのインスタンス
=========================================== */
CCameraManager& CCameraManager::GetInstance()
{
	static CCameraManager instance;
	return instance;
}



/* ========================================
	カメラシーン初期化関数
	-------------------------------------
	内容：シーン開始時にカメラを再取得
	-------------------------------------
	引数1：シーンクラスのポインタ
=========================================== */
void CCameraManager::Init(SceneBase* pScene)
{
	m_pScene = pScene;	// シーンクラスのポインタを取得

	// カメラ一覧をクリア
	m_pCameraList.clear();

	// シーンからカメラリストを取得
	m_pCameraList = m_pScene->GetSceneObjects<ObjectCamera>();

	// カメラが存在する場合
	if (m_pCameraList.size() > 0)
	{
		SwitchCamera(0);												// アクティブにする
	}
	// カメラが存在しない場合
	else
	{
		m_pScene->AddSceneObject<ObjectCamera>(DEFAULT_CAMERA_NAME);				// カメラ追加
		SwitchCamera(0);												// アクティブにする
	}
}


/* ========================================
	カメラ切り替え関数
	-------------------------------------
	内容：カメラの切り替え
	-------------------------------------
	引数1：カメラ番号(0～)
=========================================== */
void CCameraManager::SwitchCamera(int num)
{
	// カメラリストのサイズが指定数より小さい場合
	// カメラ配列番号(0～)、カメラ番号(1～)のため、サイズより大きい場合は処理しない
	if (num >= m_pCameraList.size()) return;

	// カメラリストを全て非アクティブにする
	ResetActiveCamera();

	m_pCameraList.at(num)->SetActive(true);		// 指定番号のカメラをアクティブにする
	m_pActiveCamera = m_pCameraList.at(num);	// アクティブカメラを設定

#ifdef _DEBUG
	// オブジェクト一覧に追加
	WIN_CAMERA_INFO["ActiveCamera"].SetText(m_pActiveCamera->GetName().c_str());

#endif // _DEBUG
}

/* ========================================
	カメラ追加関数
	-------------------------------------
	内容：カメラを一覧に追加
	-------------------------------------
	引数：カメラのポインタ
=========================================== */
void CCameraManager::AddCamera(ObjectCamera* pCamera)
{
	m_pCameraList.push_back(pCamera);
#ifdef _DEBUG
	// オブジェクト一覧に追加
	WIN_CAMERA_INFO["CameraList"].AddListItem(pCamera->GetName().c_str());

#endif // _DEBUG

}

/* ========================================
	カメラ削除関数
	-------------------------------------
	内容：カメラを一覧から削除
	-------------------------------------
	引数：カメラのポインタ
=========================================== */
void CCameraManager::RemoveCamera(ObjectCamera* pCamera)
{
	// カメラリストから削除
	m_pCameraList.erase(std::remove(m_pCameraList.begin(), m_pCameraList.end(), pCamera), m_pCameraList.end());
#ifdef _DEBUG
	if (pCamera == nullptr) return;
	// オブジェクト一覧に追加
	WIN_CAMERA_INFO["CameraList"].RemoveListItem(pCamera->GetName().c_str());

#endif // _DEBUG
}

/* ========================================
	アクティブカメラリセット関数
	-------------------------------------
	内容：アクティブカメラをリセット
=========================================== */
void CCameraManager::ResetActiveCamera()
{
	// カメラリストを全て非アクティブにする
	for (auto pCamera : m_pCameraList)
	{
		pCamera->SetActive(false);
	}
}


/* ========================================
	有効カメラ取得関数
	-------------------------------------
	内容：現在有効なカメラの取得
	-------------------------------------
	戻値：ObjectCamera*		有効カメラのポインタ
============================================ */
ObjectCamera* CCameraManager::GetActiveCamera()
{
	return m_pActiveCamera;
}

/* ========================================
	カメラ数取得関数
	-------------------------------------
	内容：シーン上のカメラ数を取得
	-------------------------------------
	戻値：int		カメラ数
============================================ */
int CCameraManager::GetCameraNum()
{
	return m_pCameraList.size();
}