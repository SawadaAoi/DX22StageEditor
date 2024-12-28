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

// トランスフォームエディタ用
#include "Input.h"	
#include "ComponentTransform.h"

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
#ifdef _DEBUG
	, m_pSelectObj(nullptr)	// 選択中のオブジェクト
	, m_nObjectListSelectNo(-1)
	, m_nTransEditMode(0)
#endif // _DEBUG
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
	WIN_CAMERA_INFO["CameraList"].RemoveListItemAll();

#endif // _DEBUG
	UninitLocal();	// 個別終了処理

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

}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void SceneBase::Update()
{
	UpdateObject();	// オブジェクト更新
	UpdateLocal();	// 個別更新処理

#ifdef _DEBUG
	// 変形エディタ更新
	UpdateTransformEdit();	
	// リスト未選択時は選択オブジェクトをクリア
	if (m_nObjectListSelectNo == -1) m_pSelectObj = nullptr;	
	// オブジェクトリスト再読み込み
	ReloadDebugObjectList();	
#endif // _DEBUG

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
	オブジェクト更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void SceneBase::UpdateObject()
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
		m_pObjects.emplace_back(std::move(pObject));	// オブジェクト配列に移動
	}
	m_pStandbyObjects.clear();	// クリア


	UpdateCollision();	// 各当たり判定更新(死亡したオブジェクトをm_bColStatesMapから削除するためにこの位置に記述)
	RemoveDeadObjects();// 死亡状態のオブジェクトを削除
}

/* ========================================
	オブジェクト削除関数
	-------------------------------------
	内容：死亡状態のオブジェクトを削除
=========================================== */
void SceneBase::RemoveDeadObjects()
{
	// ↓のループ内でオブジェクトの状態が変わるので、一時保存用にコピー
	std::map<ObjectBase*, ObjectBase::E_State> pObjectStateMap;
	for (auto& pObject : m_pObjects)
		pObjectStateMap.insert(std::make_pair(pObject.get(), pObject->GetState()));


	// 死亡状態のオブジェクトを削除
	for (auto it = m_pObjects.begin(); it != m_pObjects.end();)
	{
		ObjectBase* pObject = it->get();
		// 死亡状態かどうか
		if (pObjectStateMap.at(pObject) == ObjectBase::E_State::STATE_DEAD)
		{
#ifdef _DEBUG
			// 削除対象オブジェクトが一覧で選択中の場合
			if (m_nObjectListSelectNo == ITEM_OBJ_LIST.GetListNo(pObject->GetListName().c_str()))
			{
				WIN_OBJ_INFO.Clear();	// オブジェクト情報ウィンドウクリア

				m_nObjectListSelectNo = -1;								// 選択番号をリセット
				ITEM_OBJ_LIST.SetListNo(-1);	// 選択番号をリセット
			}
#endif

			// 子オブジェクトがある場合
			if (pObject->GetChildObjects().size() > 0)
			{
				for (auto& pChild : pObject->GetChildObjects())
				{
					this->RemoveSceneObject(pChild);	// 子オブジェクトを削除
				}
			}
			// 親オブジェクトがある場合
			if (pObject->GetParentObject())
			{
				pObject->GetParentObject()->RemoveChildObject(pObject);	// 親オブジェクトから削除
			}

			pObject->Uninit();			// 終了処理
			it = m_pObjects.erase(it);	// 削除

		}
		else
		{
			++it;	// 次の要素へ
		}
	}

}

/* ========================================
	オブジェクト追加関数
	-------------------------------------
	内容：シーンにオブジェクトを追加
		　※ファイルデータからオブジェクトを追加する場合に使用
	-------------------------------------
	引数：追加するオブジェクトポインタ
=========================================== */
void SceneBase::AddSceneObjectBase(ObjectBase* pObject)
{
	// シーンが更新中かどうかをチェックします
	if (m_bIsUpdating)
	{
		// 一時保存用の配列にユニークポインタを移動します
		m_pStandbyObjects.push_back(std::unique_ptr<ObjectBase>(pObject));
	}
	else
	{
		// シーンのオブジェクト配列にユニークポインタを移動します
		m_pObjects.push_back(std::unique_ptr<ObjectBase>(pObject));
	}
}

/* ========================================
	オブジェクト検索関数
	-------------------------------------
	内容：シーンに所属するオブジェクトを検索
	-------------------------------------
	引数：sName	オブジェクト名
	-------------------------------------
	戻値：取得オブジェクトポインタ
=========================================== */
ObjectBase* SceneBase::FindSceneObject(std::string sName)
{
	// 名前が一致するオブジェクトを検索
	for (auto& pObject : m_pObjects)
	{
		if (pObject->GetName() == sName)
		{
			return pObject.get();
		}
	}

	return nullptr;
}

/* ========================================
	オブジェクト削除関数
	-------------------------------------
	内容：シーンに所属するオブジェクトを削除
		　※死亡状態のオブジェクトを削除する場合に使用(子オブジェクトの削除時)
	-------------------------------------
	引数：pObject	削除するオブジェクトポインタ
=========================================== */
void SceneBase::RemoveSceneObject(ObjectBase* pObject)
{
	if (pObject->GetChildObjects().size() > 0)
	{
		for (auto& pChild : pObject->GetChildObjects())
		{
			this->RemoveSceneObject(pChild);	// 子オブジェクトを削除
		}
	}

	pObject->RemoveParentObject();						// 親オブジェクトから削除
	pObject->SetState(ObjectBase::E_State::STATE_DEAD);	// 死亡状態に設定

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

	// 衝突状態マップ更新(上記の処理で衝突状態が変わったため)
	for (size_t i = 0; i < m_pObjectCollision.size(); ++i) {
		for (size_t j = i + 1; j < m_pObjectCollision.size(); ++j) {

			// 衝突判定を行うコンポーネントを取得
			ComponentCollisionBase* collisionA = m_pObjectCollision.at(i);
			ComponentCollisionBase* collisionB = m_pObjectCollision.at(j);

			collisionA->UpdateCollisionMap(collisionB);
			collisionB->UpdateCollisionMap(collisionA);
		}
	}
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
		return sName = CreateUniqueName(sName);	// オブジェクト名と重複チェック
	}
	else
	{
		return sName;
	}
}

/* ========================================
	全オブジェクト取得関数
	-------------------------------------
	内容：シーンに所属する全てのオブジェクトを取得
	-------------------------------------
	戻値：取得したオブジェクトのポインタ配列
=========================================== */
std::vector<ObjectBase*> SceneBase::GetAllSceneObjects()
{
	std::vector<ObjectBase*> objects;

	for (const auto& pObject : m_pObjects)
	{
		objects.push_back(pObject.get());
	}

	return objects;
}

/* ========================================
	タグ別オブジェクト収集関数
	-------------------------------------
	内容：シーンに所属する特定のタグのオブジェクト
		　を全て収集する
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
	タグ別オブジェクト収集関数
	-------------------------------------
	内容：シーンに所属する特定のタグのオブジェクト
		　を一番最初に見つかったものを取得する
	-------------------------------------
	戻値：取得したオブジェクトのポインタ
========================================== */
ObjectBase* SceneBase::GetSceneObjectTag(E_ObjectTag tag)
{
	// シーンに所属するオブジェクトを検索
	for (const auto& pObject : m_pObjects)
	{
		// 一番最初に見つかったオブジェクトを返す
		if (pObject->GetTag() == tag)
		{
			return pObject.get();
		}
	}

	return nullptr;
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
	using namespace DebugUI;

	// オブジェクト削除ボタン
	WIN_OBJ_LIST.AddItem(Item::CreateCallBack("Remove", Item::Kind::Command, [this](bool isWrite, void* arg)
	{
		// 選択されていない場合は処理しない
		if (m_nObjectListSelectNo == -1) return;					
		// シーン上のカメラが1つの場合、カメラオブジェクトは削除不可
		if (dynamic_cast<ObjectCamera*>(m_pSelectObj) != nullptr && CAMERA_MNG_INST.GetCameraNum() == 1) return;
		// カメラオブジェクトがアクティブの場合、削除不可
		if (CAMERA_MNG_INST.GetActiveCamera() == m_pSelectObj)	return;

		m_pSelectObj->SetState(ObjectBase::E_State::STATE_DEAD);	// 死亡状態に設定

	}));

	// オブジェクトフォーカスボタン
	WIN_OBJ_LIST.AddItem(Item::CreateCallBack("Focus", Item::Kind::Command, [this](bool isWrite, void* arg)
	{
		// 選択されていない場合は処理しない
		if (m_nObjectListSelectNo == -1) return;					
		// アクティブカメラはフォーカス移動不可
		if (m_pSelectObj == CAMERA_MNG_INST.GetActiveCamera()) return; 

		CAMERA_MNG_INST.FocusMoveCamera(m_pSelectObj);	// カメラを指定オブジェクトにフォーカス移動

	}, false, true));

	Item::ConstCallback  FuncListClick = [this](const void* arg) {
		// クリックされたオブジェクトの情報を表示

		std::string sObjName = reinterpret_cast<const char*>(arg);
		m_nObjectListSelectNo = ITEM_OBJ_LIST.GetListNo(sObjName.c_str());	// 選択番号を取得

		// 名前に"L"が含まれている場合(子オブジェクトの場合)
		if (sObjName.find(CHILD_HEAD_TEXT) != std::string::npos)
		{
			// "L"を除去した名前に変換
			int nHeadTextCnt = sObjName.find(CHILD_HEAD_TEXT);
			sObjName = sObjName.substr(nHeadTextCnt + CHILD_HEAD_TEXT.size());
		}

		InitObjectInfo(sObjName);

	};

	Item* pList = Item::CreateList(ITEM_OBJ_LIST_NAME.c_str(), FuncListClick, false);
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
			m_pSelectObj = pObject.get();	// 選択中のオブジェクトを保持
			break;
		}
	}
}

/* ========================================
	変形エディタモード設定関数
	-------------------------------------
	内容：変形エディタのモードを設定
	-------------------------------------
	引数：int モード(0:なし, 1:移動, 2:回転, 3:拡大)
=========================================== */
void SceneBase::SetTransformEditMode(int nMode)
{
	m_nTransEditMode = nMode;
}

/* ========================================
	変形エディタリセット関数
	-------------------------------------
	内容：変形エディタのリセットボタンが押された時の処理
=========================================== */
void SceneBase::ResetTransformEdit()
{
	if (m_nObjectListSelectNo == -1) return;	// 選択されていない場合は処理しない

	// 選択中のオブジェクトのトランスフォームを取得
	ComponentTransform* pTrans = m_pSelectObj->GetComponent<ComponentTransform>();

	// 変形エディタのモードによってリセット処理を変更
	switch (m_nTransEditMode)
	{
	// Noneの場合は全てリセット
	case 0:
		pTrans->SetLocalPosition(Vector3<float>(0.0f, 0.0f, 0.0f));
		pTrans->SetLocalRotationEuler(Vector3<float>(0.0f, 0.0f, 0.0f));
		pTrans->SetLocalScale(Vector3<float>(1.0f, 1.0f, 1.0f));
		break;
	// 移動の場合は位置リセット
	case 1:
		pTrans->SetLocalPosition(Vector3<float>(0.0f, 0.0f, 0.0f));
		break;
	// 回転の場合は回転リセット
	case 2:
		pTrans->SetLocalRotationEuler(Vector3<float>(0.0f, 0.0f, 0.0f));
		break;
	// 拡大の場合は拡大リセット
	case 3:
		pTrans->SetLocalScale(Vector3<float>(1.0f, 1.0f, 1.0f));
		break;
	}
}

/* ========================================
	変形エディタ更新関数
	-------------------------------------
	内容：変形エディタの更新処理
=========================================== */
void SceneBase::UpdateTransformEdit()
{
	if (m_nObjectListSelectNo == -1) return;	// 選択されていない場合は処理しない
	if (Input::IsKeyPress(VK_SHIFT)) return;	// Shiftキーが押されている場合は処理しない

	// 選択中のオブジェクトのトランスフォームを取得
	ComponentTransform* pTrans = m_pSelectObj->GetComponent<ComponentTransform>();
	// デバッグメニューのモードテキストを取得
	DebugUI::Item* pModeText = &WIN_TRANSFORM_EDIT["Mode"];
	pModeText->SetText("None");	// 選択なしはNone

	// 変形エディタのモードによって処理を変更
	switch (m_nTransEditMode)
	{
	case 1:	// 移動
	{
		pModeText->SetText("Position");
		float fChangeVal = WIN_TRANSFORM_EDIT["ValuePos"].GetFloat() * DELTA_TIME;	// 変化値

		// 移動処理
		if (Input::IsKeyPress('W')) pTrans->TranslateZ(fChangeVal);
		if (Input::IsKeyPress('S')) pTrans->TranslateZ(-fChangeVal);
		if (Input::IsKeyPress('A')) pTrans->TranslateX(-fChangeVal);
		if (Input::IsKeyPress('D')) pTrans->TranslateX(fChangeVal);
		if (Input::IsKeyPress('E')) pTrans->TranslateY(fChangeVal);
		if (Input::IsKeyPress('Q')) pTrans->TranslateY(-fChangeVal);
		break;
	}
	case 2:	// 回転
	{
		pModeText->SetText("Rotation");
		float fChangeVal = WIN_TRANSFORM_EDIT["ValueRot"].GetFloat() * DELTA_TIME;	// 変化値

		// 回転処理
		if (Input::IsKeyPress('W')) pTrans->RotateX(fChangeVal);
		if (Input::IsKeyPress('S')) pTrans->RotateX(-fChangeVal);
		if (Input::IsKeyPress('A')) pTrans->RotateY(-fChangeVal);
		if (Input::IsKeyPress('D')) pTrans->RotateY(fChangeVal);
		if (Input::IsKeyPress('E')) pTrans->RotateZ(fChangeVal);
		if (Input::IsKeyPress('Q')) pTrans->RotateZ(-fChangeVal);
		break;
	}
	case 3:	// 拡大
	{
		pModeText->SetText("Scale");
		float fChangeVal = WIN_TRANSFORM_EDIT["ValueScale"].GetFloat() * DELTA_TIME;	// 変化値

		// 拡大率
		float fChangPlus	= 1.0f + fChangeVal;
		float fChangMinus	= 1.0f - fChangeVal;

		// 拡大処理
		if (Input::IsKeyPress('W')) pTrans->ScaleZ(fChangPlus);
		if (Input::IsKeyPress('S')) pTrans->ScaleZ(fChangMinus);
		if (Input::IsKeyPress('A')) pTrans->ScaleX(fChangMinus);
		if (Input::IsKeyPress('D')) pTrans->ScaleX(fChangPlus);
		if (Input::IsKeyPress('E')) pTrans->ScaleY(fChangPlus);
		if (Input::IsKeyPress('Q')) pTrans->ScaleY(fChangMinus);
		break;
	}
	}
}

/* ========================================
	デバッグ用オブジェクト一覧再読込関数
	-------------------------------------
	内容：オブジェクト一覧を再読み込みする
=========================================== */
void SceneBase::ReloadDebugObjectList()
{
	// オブジェクト一覧をクリア
	ITEM_OBJ_LIST.RemoveListItemAll();

	// シーンに所属する全てのオブジェクトを取得
	for (const auto& pObject : m_pObjects)
	{
		if (pObject->GetParentObject()) continue;	// 親オブジェクトがある場合は飛ばす
		// オブジェクト一覧に追加
		ITEM_OBJ_LIST.AddListItem(pObject->GetName().c_str());
		AddObjectListChild(pObject.get());
	}
}


/* ========================================
	デバッグ用オブジェクト一覧子オブジェクト追加関数
	-------------------------------------
	内容：オブジェクト一覧に子オブジェクトを追加
		　※子がある限り再帰的に呼び出される
	-------------------------------------
	引数：ObjectBase* 親オブジェクト
=========================================== */
void SceneBase::AddObjectListChild(ObjectBase* pObject)
{
	// 子オブジェクトがある場合
	if (pObject->GetChildObjects().size() > 0)
	{
		for (auto& pChild : pObject->GetChildObjects())
		{
			// 挿入位置
			int nInsertNo = ITEM_OBJ_LIST.GetListNo(pObject->GetListName().c_str());
			// オブジェクト一覧に追加
			ITEM_OBJ_LIST.InsertListItem(pChild->GetListName().c_str(), nInsertNo + 1);
			// 子オブジェクトを追加
			AddObjectListChild(pChild);
		}
	}
	else
	{
		return;
	}
}


#endif