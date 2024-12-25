/* ========================================
	DX22Base/
	------------------------------------
	オブジェクト用cpp
	------------------------------------
	ObjectBase.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectBase.h"
#include "ComponentBase.h"
#include <windows.h>
#include "ComponentTransform.h"
#include "DebugMenu.h"
#include "SceneManager.h"

// =============== 定数定義 =======================
const float DEFAULT_LIGHT_DIFFUSE = 1.0f;	// デフォルト拡散光
const float DEFAULT_LIGHT_SPECULAR = 0.0f;	// デフォルト鏡面光
const float DEFAULT_LIGHT_AMBIENT = 0.3f;	// デフォルト環境光

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーン
=========================================== */
ObjectBase::ObjectBase(SceneBase* pScene)
	: m_pOwnerScene(pScene)				// 所有シーンを設定
	, m_eState(E_State::STATE_ACTIVE)	// 状態をアクティブに設定
	, m_pParentObj(nullptr)				// 親オブジェクトをnullptrに設定
	, m_pChildObjs()					// 子オブジェクト配列を初期化
	, m_pComponents()					// コンポーネント配列を初期化
	, m_eTag(E_ObjectTag::None)			// タグをNoneに設定
	, m_sName("NoName")					// オブジェクト名をNoNameに設定
	, m_tLightParam{ DEFAULT_LIGHT_DIFFUSE, DEFAULT_LIGHT_SPECULAR, DEFAULT_LIGHT_AMBIENT, true}	// ライトパラメータ初期化
{
	// 所有者オブジェクトがnullptrの場合はエラーを出力
	if (pScene == nullptr)
	{
		OutputDebugStringA("ObjectBase::ObjectBase() : 所有シーンがnullptrです。\n");
	}

}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
	-------------------------------------
	引数：オブジェクト名
=========================================== */
void ObjectBase::Init(std::string sName)
{
	m_pComponents.clear();	// クリア
	SetName(sName);			// オブジェクト名を設定
	InitDefaultComponent();	// デフォルトコンポーネント追加

	InitLocal();	// 個別初期化処理
}

/* ========================================
	終了関数
	-------------------------------------
	内容：終了処理
=========================================== */
void ObjectBase::Uninit()
{
	// 所持コンポーネント配列の全要素を削除
	for (auto& pComponent : m_pComponents)
	{
		pComponent->Uninit();
		pComponent.reset();	// メモリ解放

	}
	m_pComponents.clear();	// クリア

	UninitLocal();	// 個別終了処理
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void ObjectBase::Update()
{
	// 所持コンポーネント配列の全要素を更新
	for (auto& pComponent : m_pComponents)
	{
		if (!pComponent->GetActive()) continue;	// コンポーネントが非アクティブの場合は更新しない
		pComponent->Update();
	}

	UpdateLocal();	// 個別更新処理

	// 子オブジェクトの更新
	for (auto pChildObj : m_pChildObjs)
	{
		pChildObj->Update();
	}
}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
=========================================== */
void ObjectBase::Draw()
{
	// 所持コンポーネント配列の全要素を描画
	for (auto& pComponent : m_pComponents)
	{
		if (!pComponent->GetActive()) continue;	// コンポーネントが非アクティブの場合は更新しない
		pComponent->Draw();
	}

	DrawLocal();	// 個別描画処理
}


/* ========================================
	デフォルトコンポーネント追加関数
	-------------------------------------
	内容：オブジェクトにデフォルトで持たせる
		　コンポーネントを追加する
=========================================== */
void ObjectBase::InitDefaultComponent()
{
	m_pCompTransform = AddComponent<ComponentTransform>();	// Transformコンポーネントを追加	
}

/* ========================================
	衝突開始処理関数
	-------------------------------------
	内容：他オブジェクトと衝突した際に呼ばれる
	-------------------------------------
	引数1：衝突したオブジェクト
=========================================== */
void ObjectBase::OnCollisionEnter(ObjectBase* pHit)
{
	// 継承して各オブジェクトで処理を記述
}

/* ========================================
	衝突中処理関数
	-------------------------------------
	内容：他オブジェクトと衝突している間呼ばれる
	-------------------------------------
	引数1：衝突しているオブジェクト
=========================================== */
void ObjectBase::OnCollisionStay(ObjectBase* pHit)
{
	// 継承して各オブジェクトで処理を記述
}

/* ========================================
	衝突終了処理関数
	-------------------------------------
	内容：他オブジェクトと衝突が終了した際に呼ばれる
	-------------------------------------
	引数1：衝突が終了したオブジェクト
=========================================== */
void ObjectBase::OnCollisionExit(ObjectBase* pHit)
{
	// 継承して各オブジェクトで処理を記述

}

/* ========================================
	親オブジェクト設定関数
	-------------------------------------
	内容：親オブジェクトを設定する
	-------------------------------------
	引数1：親に設定するオブジェクト
=========================================== */
void ObjectBase::SetParentObject(ObjectBase* pParentObj)
{
	// 既に設定されている親オブジェクトから自身を削除
	if (m_pParentObj)
		m_pParentObj->RemoveChildObject(this);

	m_pParentObj = pParentObj;	// 自オブジェクトの更新

	// 既に親オブジェクトが更新済みかチェック
	// ※子オブジェクト追加関数から呼び出された場合
	for (auto& pChild : m_pParentObj->m_pChildObjs)
	{
		if (pChild == this)	return;
	}

	m_pParentObj->AddChildObject(this);	// 親オブジェクトの更新
}

/* ========================================
	子オブジェクト追加関数
	-------------------------------------
	内容：子オブジェクトを追加する
	-------------------------------------
	引数1：子オブジェクトにするオブジェクト
=========================================== */
void ObjectBase::AddChildObject(ObjectBase* pChildObj)
{
	// 既に子オブジェクトに登録されている場合は追加しない
	for (auto& pChild : m_pChildObjs)
	{
		if (pChild == pChildObj)	return;
	}
	m_pChildObjs.push_back(pChildObj);	// 自オブジェクトの更新

	// 既に子オブジェクトが更新済みかチェック
	// ※親オブジェクト設定関数から呼び出された場合
	if (pChildObj->GetParentObject() == this) return;

	pChildObj->SetParentObject(this);	// 子オブジェクトの更新
}

/* ========================================
	親オブジェクト解除関数
	-------------------------------------
	内容：親オブジェクトを解除する
=========================================== */
void ObjectBase::RemoveParentObject()
{
	// 親オブジェクトがない場合は処理しない
	if (m_pParentObj == nullptr) return;

	m_pParentObj->RemoveChildObject(this);	// 親オブジェクトから自身を削除
	m_pParentObj = nullptr;					// 親オブジェクトを空に設定
	GetComponent<ComponentTransform>()->ClearParent();	// Transformコンポーネントの親解除処理

}

/* ========================================
	子オブジェクト削除関数
	-------------------------------------
	内容：子オブジェクトを削除する
	-------------------------------------
	引数1：削除する子オブジェクト
=========================================== */
void ObjectBase::RemoveChildObject(ObjectBase* pChildObj)
{
	// 子オブジェクトを検索して配列から削除
	m_pChildObjs.erase(
		std::remove(m_pChildObjs.begin(), m_pChildObjs.end(), pChildObj), m_pChildObjs.end());

	pChildObj->m_pParentObj = nullptr;								// 親オブジェクトを空に設定
	pChildObj->GetComponent<ComponentTransform>()->ClearParent();	// Transformコンポーネントの親解除処理

}

/* ========================================
	全子オブジェクト削除関数
	-------------------------------------
	内容：全子オブジェクトを削除する
=========================================== */
void ObjectBase::RemoveAllChildObjects()
{
	// 子オブジェクトを全て配列から削除
	for (auto pChild : m_pChildObjs)
	{
		pChild->m_pParentObj = nullptr;								// 親オブジェクトを空に設定
		pChild->GetComponent<ComponentTransform>()->ClearParent();	// Transformコンポーネントの親解除処理

	}
	m_pChildObjs.clear();

}

/* ========================================
	世代数取得関数
	-------------------------------------
	内容：自身の世代数を取得する
	-------------------------------------
	戻り値：世代数
=========================================== */
int ObjectBase::GetGenerationCount()
{
	// 親オブジェクトがない場合
	if (m_pParentObj == nullptr)
	{
		return 0;
	}
	else
	{
		return m_pParentObj->GetGenerationCount() + 1;
	}
}

/* ========================================
	出力データ個別関数
	-------------------------------------
	内容：オブジェクト毎に出力するデータを設定
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectBase::OutPutLocalData(std::ofstream& file)
{
	// 継承して各オブジェクトで処理を記述
}

/* ========================================
	入力データ個別関数
	-------------------------------------
	内容：オブジェクト毎に入力するデータを設定
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectBase::InputLocalData(std::ifstream& file)
{
	// 継承して各オブジェクトで処理を記述
}

/* ========================================
	ゲッター(トランスフォーム)関数
	-------------------------------------
	戻値：ComponentTransform* トランスフォームコンポーネントのポインタ
=========================================== */
ComponentTransform* ObjectBase::GetTransform() const
{
	return m_pCompTransform;
}

/* ========================================
	ゲッター(所持シーン)関数
	-------------------------------------
	戻値：所有シーンのポインタ
=========================================== */
SceneBase* ObjectBase::GetOwnerScene() const
{
	return m_pOwnerScene;
}

/* ========================================
	ゲッター(状態)関数
	-------------------------------------
	戻値：状態
=========================================== */
ObjectBase::E_State ObjectBase::GetState() const
{
	return m_eState;
}

/* ========================================
	ゲッター(オブジェクトID)関数
	-------------------------------------
	戻値：オブジェクトID
=========================================== */
size_t ObjectBase::GetTypeID() const
{
	return GetStaticTypeID();
}

/* ========================================
	ゲッター(静的オブジェクトID)関数
	-------------------------------------
	戻値：静的オブジェクトID
=========================================== */
size_t ObjectBase::GetStaticTypeID()
{
	return reinterpret_cast<size_t>(&GetStaticTypeID);
}

/* ========================================
	ゲッター(オブジェクトクラス名)関数
	-------------------------------------
	戻値：オブジェクトクラス名
=========================================== */
std::string ObjectBase::GetObjClassName() const
{
	return "ObjectBase";
}

/* ========================================
	ゲッター(親オブジェクト)関数
	-------------------------------------
	戻値：親オブジェクトのポインタ
=========================================== */
ObjectBase* ObjectBase::GetParentObject() const
{
	return m_pParentObj;
}


/* ========================================
	ゲッター(子オブジェクト)関数
	-------------------------------------
	戻値：子オブジェクト配列
=========================================== */
std::vector<ObjectBase*> ObjectBase::GetChildObjects() const
{
	return m_pChildObjs;
}

/* ========================================
	ゲッター(タグ)関数
	-------------------------------------
	戻値：タグ
=========================================== */
E_ObjectTag ObjectBase::GetTag() const
{
	return m_eTag;
}

/* ========================================
	ゲッター(オブジェクト名)関数
	-------------------------------------
	戻値：オブジェクト名
=========================================== */
std::string ObjectBase::GetName() const
{
	return m_sName;
}

/* ========================================
	ゲッター(ライトパラメータ)関数
	-------------------------------------
	戻値：ライトパラメータ
=========================================== */
ObjectBase::T_LightParam ObjectBase::GetLightMaterial() const
{
	return  m_tLightParam;
}

/* ========================================
	セッター(状態)関数
	-------------------------------------
	引数1：状態
=========================================== */
void ObjectBase::SetState(E_State eState)
{
	m_eState = eState;
}

/* ========================================
	セッター(タグ)関数
	-------------------------------------
	引数1：タグ
=========================================== */
void ObjectBase::SetTag(E_ObjectTag eTag)
{
	m_eTag = eTag;
}

/* ========================================
	セッター(オブジェクト名)関数
	-------------------------------------
	引数1：オブジェクト名
=========================================== */
void ObjectBase::SetName(std::string sName)
{
	m_sName = sName;
}

/* ========================================
	セッター(ライトパラメータ)関数
	-------------------------------------
	引数1：拡散光
	引数2：鏡面光
	引数3：環境光
=========================================== */
void ObjectBase::SetLightMaterial(float fDiffuse, float fSpecular, float fAmbient)
{
	m_tLightParam.fDiffuse	= fDiffuse;
	m_tLightParam.fSpecular = fSpecular;
	m_tLightParam.fAmbient	= fAmbient;
}

/* ========================================
	セッター(ライト使用)関数
	-------------------------------------
	引数1：ライト使用フラグ
=========================================== */
void ObjectBase::SetLightUse(bool bUse)
{
	m_tLightParam.bLightUse = bUse;
}




#ifdef _DEBUG
/* ========================================
	デバッグ用関数
	-------------------------------------
	内容：デバッグ用の処理
		　例：オブジェクト情報(所持コンポーネント等)の表示
=========================================== */
void ObjectBase::Debug()
{
	using namespace DebugUI;

	Window& pObjInfo = WIN_OBJ_INFO;

	// オブジェクト詳細情報を更新
	pObjInfo.AddItem(Item::CreateValue("ObjectName", Item::Text, false));	// 名前
	pObjInfo["ObjectName"].SetText(this->GetName().c_str());				// オブジェクト名を設定

	Item* pGroupObjectBase = Item::CreateGroup("ObjectBase");	// オブジェクト基本情報グループ

	// オブジェクト名変更
	pGroupObjectBase->AddGroupItem(Item::CreateCallBack("ChangeName", Item::Kind::Command, [this](bool isWrite, void* arg)	// 名前変更ボタン
	{
		ChangeName();
	}));
	pGroupObjectBase->AddGroupItem(Item::CreateValue("ObjectReName", Item::Path, false, true));	// 変更後の名前

	pGroupObjectBase->AddGroupItem(InitParentList());	// 親オブジェクトリスト

	pGroupObjectBase->AddGroupItem(Item::CreateBind("UseLight", Item::Bool, &m_tLightParam.bLightUse, false));	// ライト使用

	pObjInfo.AddItem(pGroupObjectBase);		// グループを追加

	pObjInfo["ObjectBase"]["ObjectReName"].SetPath(this->GetName().c_str());	// 変更後の名前を設定

	// ローカルデバッグ処理
	DebugLocal(pObjInfo);

	// 各コンポーネント情報をオブジェクト情報ウィンドウに表示
	auto it = m_pComponents.begin();
	while (it != m_pComponents.end())
	{
		(*it)->Debug(pObjInfo);
		++it;
	}
}

/* ========================================
	親オブジェクトリスト初期化関数
	-------------------------------------
	内容：親オブジェクトリストの初期化
	-------------------------------------
	戻り値：親オブジェクトリスト
=========================================== */
DebugUI::Item* ObjectBase::InitParentList()
{
	using namespace DebugUI;

	// 親オブジェクトリスト作成
	Item* pParentList = Item::CreateList("ParentObject", [this](const void* arg)
	{
		std::string sParentName = reinterpret_cast<const char*>(arg);
		ChangeParentList(sParentName);	// 親オブジェクト変更

	}, false, true);

	// シーン上のオブジェクトをリストに追加
	int nParentNo = 0;					// 選択中のオブジェクト番号
	pParentList->AddListItem("None");	// リストの先頭は親オブジェクトなし

	// シーン上のオブジェクトをリストに追加
	for (const auto pObj : SceneManager::GetScene()->GetAllSceneObjects())
	{
		if (pObj->GetName() == this->GetName()) continue;	// 自身は追加しない

		pParentList->AddListItem(pObj->GetName().c_str());	// シーン上のオブジェクト名を追加
	}
	// 親オブジェクトがある場合
	if (m_pParentObj)
	{
		nParentNo = pParentList->GetListNo(m_pParentObj->GetName().c_str());	// 親オブジェクトを選択
	}

	pParentList->SetListNo(nParentNo);	// 選択中のオブジェクトを設定

	pParentList->RemoveListItem(this->GetName().c_str());


	return pParentList;
}

/* ========================================
	名前変更関数
	-------------------------------------
	内容：オブジェクト名を変更する
=========================================== */
void ObjectBase::ChangeName()
{
	std::string sReName = WIN_OBJ_INFO["ObjectBase"]["ObjectReName"].GetPath();	// 変更後の名前
	std::string sOldName = this->GetName();										// 変更前の名前

	if (sReName.empty()) return;		// 変更後の名前が空の場合は処理しない
	if (sReName == sOldName) return;	// 同じ名前の場合は処理しない

	sReName = SceneManager::GetScene()->CreateUniqueName(sReName);	// 重複しない名前に変更

	// オブジェクト名変更
	int listNo = ITEM_OBJ_LIST.GetListNo(this->GetListName().c_str());			// オブジェクト一覧の表示位置取得
	ITEM_OBJ_LIST.RemoveListItem(sOldName.c_str(), DebugUI::CHILD_HEAD_TEXT);	// 変更前の名前をリストから削除
	
	this->SetName(sReName);												// 内部の名前変更
	ITEM_OBJ_LIST.InsertListItem(this->GetListName().c_str(), listNo);	// オブジェクト一覧に変更後の名前を追加

	WIN_OBJ_INFO["ObjectName"].SetText(this->GetName().c_str());		// オブジェクト詳細の名前を変更
}


/* ========================================
	親オブジェクトリスト変更関数
	-------------------------------------
	内容：親オブジェクトを変更する
	-------------------------------------
	引数：新しい親オブジェクト名
=========================================== */
void ObjectBase::ChangeParentList(std::string sParentName)
{
	ObjectBase* pParentNew = SceneManager::GetScene()->FindSceneObject(sParentName);	// 親オブジェクト取得

	// 既に設定されている親オブジェクトと同じ場合は処理しない
	if (pParentNew == m_pParentObj) return;

	if (pParentNew)
	{
		this->SetParentObject(pParentNew);	// 新しい親オブジェクトに設定
		// オブジェクト一覧の選択位置を変更
		int nListNo = ITEM_OBJ_LIST.GetListNo(pParentNew->GetListName().c_str());	// オブジェクト一覧の表示位置取得
		ITEM_OBJ_LIST.SetListNo(nListNo + 1);
	}
	else
	{
		this->RemoveParentObject();			// 親オブジェクトがない場合(Noneを選択)は解除
		ITEM_OBJ_LIST.SetListNo(-1);
	}
}

/* ========================================
	リスト表示名取得関数
	-------------------------------------
	内容：リスト表示用の名前を取得する
	-------------------------------------
	戻り値：リスト表示用の名前
=========================================== */
std::string ObjectBase::GetListName()
{
	std::string sName;							// リスト表示名
	int nGeneCnt = this->GetGenerationCount();	// 世代数取得
	
	// 親オブジェクトがある場合
	if (nGeneCnt > 0)
	{
		// 世代数分のスペースを追加(表示を段階的にするため)
		for (int i = 0; i < nGeneCnt; i++)
		{
			sName += DebugUI::CHILD_HEAD_SPACE;
		}
		sName += DebugUI::CHILD_HEAD_TEXT + this->GetName();
	}
	// 無い場合はそのまま表示
	else
	{
		sName = this->GetName();
	}

	return sName;
}


#endif // _DEBUG




