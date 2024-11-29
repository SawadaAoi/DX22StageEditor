/* ========================================
	DX22Base/
	------------------------------------
	シーン用cpp
	------------------------------------
	SceneBase.cpp
========================================== */

// =============== インクルード =====================
#include "SceneBase.h"
#include "ObjectBase.h"
#include "ObjectCamera.h"
#include "CameraManager.h"
#include "ComponentCollisionBase.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
========================================== */
SceneBase::SceneBase()
	: m_bIsUpdating(false)	// 更新中フラグを初期化
	, m_pObjects()			// シーンに所属するオブジェクト一覧
	, m_pStandbyObjects()	// オブジェクトを一時的に保存しておく配列
	, m_pObjectCollision()	// 各オブジェクトが持つ衝突判定コンポーネント
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
=========================================== */
void SceneBase::Init()
{
#ifdef _DEBUG
	InitObjectList();
#endif // _DEBUG

	// クリア
	m_pObjects.clear();			// シーンに所属するオブジェクト一覧
	m_pStandbyObjects.clear();	// オブジェクトを一時的に保存しておく配列
	InitLocal();				// 個別初期化処理

	CAMERA_MNG_INST.Init(this);	// シーンのカメラを初期化
}

/* ========================================
	終了関数
	-------------------------------------
	内容：終了処理
=========================================== */
void SceneBase::Uninit()
{
#ifdef _DEBUG
	WIN_OBJ_LIST.Clear();
	WIN_OBJ_INFO.Clear();
	WIN_UI_LIST.Clear();
	WIN_UI_INFO.Clear();
	WIN_CAMERA_INFO["CameraList"].RemoveListItemAll();

#endif // _DEBUG


	// 所持オブジェクト配列の全要素を削除
	for (auto& pObject : m_pObjects)
	{
		pObject->Uninit();
	}
	m_pObjects.clear();	// クリア

	// 一時保存していたオブジェクト配列の全要素を削除
	for (auto& pObject : m_pStandbyObjects)
	{
		pObject->Uninit();
	}
	m_pStandbyObjects.clear();	// クリア

	UninitLocal();	// 個別終了処理
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void SceneBase::Update()
{
	m_bIsUpdating = true;	// 更新中フラグを立てる

	// 所持オブジェクト配列の全要素を更新
	for (auto& pObject : m_pObjects)
	{
		pObject->Update();
	}

	m_bIsUpdating = false;	// 更新中フラグを解除

	// 一時保存オブジェクト配列
	for (auto& pObject : m_pStandbyObjects)
	{
		pObject->Update();
		m_pObjects.emplace_back(std::move(pObject));	// シーンに追加
	}
	m_pStandbyObjects.clear();	// クリア

	// 各当たり判定更新(死亡したオブジェクトをm_bColStatesMapから削除するためにこの位置に記述)
	UpdateCollision();	

	// 死亡状態のオブジェクトを削除
	for (auto it = m_pObjects.begin(); it != m_pObjects.end();)
	{
		// 死亡状態かどうか
		if ((*it)->GetState() == ObjectBase::E_State::STATE_DEAD)
		{
#ifdef _DEBUG
			// オブジェクト一覧リストから削除
			ITEM_OBJ_LIST.RemoveListItem((*it)->GetName().c_str());
#endif
			(*it)->Uninit();			// 終了処理
			it = m_pObjects.erase(it);	// 削除
		}
		else
		{
			++it;	// 次の要素へ
		}
	}


	UpdateLocal();	// 個別更新処理
}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
=========================================== */
void SceneBase::Draw()
{
	// 所持オブジェクト配列の全要素を描画
	for (auto& pObject : m_pObjects)
	{
		pObject->Draw();	
	}

	DrawLocal();	// 個別描画処理
}

/* ========================================
	衝突判定配列追加関数
	-------------------------------------
	内容：衝突判定を管理する配列に追加
		　衝突判定コンポーネントの初期化で呼ばれる
	-------------------------------------
	引数1：衝突判定コンポーネント
=========================================== */
void SceneBase::AddObjectCollision(ComponentCollisionBase* pCollision)
{
	m_pObjectCollision.emplace_back(pCollision);	// emplace_backで追加(コピーを避ける)
}

/* ========================================
	衝突判定配列削除関数
	-------------------------------------
	内容：衝突判定を管理する配列から削除
		　衝突判定コンポーネントの終了処理で呼ばれる
	-------------------------------------
	引数1：衝突判定コンポーネント
=========================================== */
void SceneBase::RemoveObjectCollision(ComponentCollisionBase* pCollision)
{
	// 衝突判定を管理する配列から削除
	m_pObjectCollision.erase(
		std::remove(m_pObjectCollision.begin(), m_pObjectCollision.end(), pCollision), m_pObjectCollision.end());
}

/* ========================================
	当たり判定配列更新関数
	-------------------------------------
	内容：各オブジェクトが持つ当たり判定
		　コンポーネントを更新する
=========================================== */
void SceneBase::UpdateCollision()
{
	// 例：0と1、0と2、1と2、1と3、2と3、と全ての組み合わせで衝突判定を行う
	for (size_t i = 0; i < m_pObjectCollision.size(); ++i) {
		for (size_t j = i + 1; j < m_pObjectCollision.size(); ++j) {

			// 衝突判定を行うコンポーネントを取得
			ComponentCollisionBase* collisionA = m_pObjectCollision.at(i);
			ComponentCollisionBase* collisionB = m_pObjectCollision.at(j);

			collisionA->UpdateCollision(collisionB);
			collisionB->UpdateCollision(collisionA); 
		}
	}
}

/* ========================================
	タグ別オブジェクト収集関数
	-------------------------------------
	内容：シーンに所属する特定のタグのオブジェクト
		　を全て収集する
		 ※テンプレートではないが、似たような処理の為、ここに記載。
	-------------------------------------
	戻値：取得したオブジェクトのポインタ配列
========================================== */
std::vector<ObjectBase*> SceneBase::GetSceneObjectsTag(E_ObjectTag tag)
{
	// 取得したオブジェクトを格納する配列
	std::vector<ObjectBase*> objects;

	// シーンに所属するオブジェクトを検索
	for (const auto& pObject : m_pObjects)
	{
		// オブジェクトの型が一致したら配列に追加
		if (pObject->GetTag() == tag)
		{
			objects.push_back(pObject.get());
		}
	}

	return objects;	// 取得したオブジェクトの配列を返す
}

/* ========================================
	ユニーク名前生成関数
	-------------------------------------
	内容：名前が重複している場合に、
		　連番をつけた名前を生成する
	-------------------------------------
	引数：sName	オブジェクト名
	-------------------------------------
	戻値：調整後の名前
=========================================== */
std::string SceneBase::CreateUniqueName(std::string sName)
{
	// 名前が重複している場合は連番を付ける
	int nDupCnt = 0;	// 重複回数
	for (auto& pObject : m_pObjects)
	{
		// 名前が含まれている場合(既に連番をつけている場合を想定して)
		if (pObject->GetName().find(sName) != std::string::npos)
		{
			nDupCnt++;
		}
	}

	if (nDupCnt > 0)
	{
		sName += "_" + std::to_string(nDupCnt);
	}

	return sName;
}


// デバッグ用 ========================================================
#ifdef _DEBUG

/* ========================================
	ウィンドウ初期化(オブジェクト一覧)関数
	-------------------------------------
	内容：オブジェクト一覧の初期化を行う
=========================================== */
void SceneBase::InitObjectList()
{
	DebugUI::Item::ConstCallback  FuncListClick = [this](const void* arg) {
		// クリックされたオブジェクトの情報を表示

		std::string sObjName = reinterpret_cast<const char*>(arg);

		// 名前に"L"が含まれている場合(子オブジェクトの場合)
		if (sObjName.find(DebugUI::CHILD_HEAD_TEXT) != std::string::npos) 
		{
			// "L"を除去した名前に変換
			int nHeadTextCnt = sObjName.find(DebugUI::CHILD_HEAD_TEXT);					
			sObjName = sObjName.substr(nHeadTextCnt + DebugUI::CHILD_HEAD_TEXT.size());	
		}


		InitObjectInfo(sObjName);
	};

	DebugUI::Item* pList = DebugUI::Item::CreateList(ITEM_OBJ_LIST_NAME.c_str(), FuncListClick, false);
	WIN_OBJ_LIST.AddItem(pList);
}

/* ========================================
	ウィンドウ初期化(オブジェクト情報)関数
	-------------------------------------
	内容：オブジェクト情報の初期化を行う
		　※一覧のオブジェクト名をクリックする度呼ばれる
	-------------------------------------
	引数：string オブジェクト名
=========================================== */
void SceneBase::InitObjectInfo(std::string sName)
{
	using namespace DebugUI;

	WIN_OBJ_INFO.Clear();	// 表示リセット

	// 名前が一致するオブジェクトを検索
	for (auto& pObject : m_pObjects)
	{
		if (pObject->GetName() == sName)
		{
			// オブジェクト情報を表示
			pObject->Debug();
			break;
		}
	}
}



#endif