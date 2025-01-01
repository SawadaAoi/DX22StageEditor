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

// オブジェクトフォーカスに使用
#include "ObjectBase.h"
#include "ComponentTransform.h"

// =============== 定数定義 =======================
const std::string DEFAULT_CAMERA_NAME	= "DefaultCamera";	// デフォルトカメラ名
const Vector3<float> FOCUS_OFFSET		= Vector3<float>(0.0f, 2.0f, -5.0f);	// フォーカス時のカメラ位置


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
CameraManager::CameraManager()
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
CameraManager& CameraManager::GetInstance()
{
	static CameraManager instance;
	return instance;
}



/* ========================================
	カメラシーン初期化関数
	-------------------------------------
	内容：シーン開始時にカメラを再取得
	-------------------------------------
	引数1：シーンクラスのポインタ
=========================================== */
void CameraManager::Init(SceneBase* pScene)
{
	m_pScene = pScene;	// シーンクラスのポインタを取得

	// カメラ一覧をクリア
	m_pCameraList.clear();	

	// シーンからカメラリストを取得
	for (ObjectBase* pObj : m_pScene->GetSceneObjectsTag(E_ObjectTag::Camera))
	{
		AddCamera(static_cast<ObjectCamera*>(pObj));
	}

	// カメラが存在する場合
	if (m_pCameraList.size() > 0)
	{
		SwitchCamera(0);												// アクティブにする
	}
	// カメラが存在しない場合
	else
	{
		m_pScene->AddSceneObject<ObjectCamera>(DEFAULT_CAMERA_NAME);	// カメラ追加
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
void CameraManager::SwitchCamera(int num)
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
	カメラ切り替え関数
	-------------------------------------
	内容：カメラの切り替え
	-------------------------------------
	引数1：カメラのポインタ
=========================================== */
void CameraManager::SwitchCamera(ObjectCamera* pCamera)
{
	if (pCamera == nullptr) return;

	// カメラリストを全て非アクティブにする
	ResetActiveCamera();

	pCamera->SetActive(true);		// 指定のカメラをアクティブにする
	m_pActiveCamera = pCamera;	// アクティブカメラを設定

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
void CameraManager::AddCamera(ObjectCamera* pCamera)
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
void CameraManager::RemoveCamera(ObjectCamera* pCamera)
{
	// カメラリストから削除
	m_pCameraList.erase(std::remove(m_pCameraList.begin(), m_pCameraList.end(), pCamera), m_pCameraList.end());
#ifdef _DEBUG
	// カメラリストから削除
	WIN_CAMERA_INFO["CameraList"].RemoveListItem(pCamera->GetName().c_str());

#endif // _DEBUG
}

/* ========================================
	カメラフォーカス移動関数
	-------------------------------------
	内容：カメラを指定オブジェクトにフォーカス移動
	-------------------------------------
	引数：オブジェクトのポインタ
=========================================== */
void CameraManager::FocusMoveCamera(ObjectBase* pObj)
{
	ComponentTransform* pCameraTrans = GetActiveCamera()->GetComponent<ComponentTransform>();	// カメラのトランスフォーム取得
	ComponentTransform* pTargetTrans = pObj->GetComponent<ComponentTransform>();				// ターゲットのトランスフォーム取得

	// トランスフォームが取得できない場合は処理しない
	if (pCameraTrans == nullptr || pTargetTrans == nullptr) return;

	Vector3<float> vTargetPos = pTargetTrans->GetWorldPosition();
	vTargetPos += FOCUS_OFFSET;	// 斜め後ろから見下ろすように位置を調整

	// カメラの位置と向きを設定
	pCameraTrans->SetLocalPosition(vTargetPos);
	pCameraTrans->LookAt(pTargetTrans->GetWorldPosition());

}

/* ========================================
	アクティブカメラリセット関数
	-------------------------------------
	内容：アクティブカメラをリセット
=========================================== */
void CameraManager::ResetActiveCamera()
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
ObjectCamera* CameraManager::GetActiveCamera()
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
int CameraManager::GetCameraNum()
{
	return m_pCameraList.size();
}
