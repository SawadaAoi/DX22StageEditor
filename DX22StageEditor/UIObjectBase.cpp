/* ========================================
	DX22Base/
	------------------------------------
	UIオブジェクト基底用cpp
	------------------------------------
	UIObjectBase.cpp
========================================== */
#pragma once

// =============== インクルード =====================
#include "UIObjectBase.h"
#include "SceneBase.h"			// シーン基底クラス
#include "UIComponentBase.h"	// UIコンポーネント基底クラス
#include <windows.h>
#include "UIComponentTransform.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーン
=========================================== */
UIObjectBase::UIObjectBase(SceneBase* pScene)
	: m_pOwnerScene(pScene)
	, m_pComponents()
	, m_sName("NoName")
	, m_pParentObj(nullptr)
	, m_pChildObjs()
{
	// 所有シーンがnullptrの場合はエラーを出力
	if (pScene == nullptr)
	{
		OutputDebugStringA("UIObjectBase::UIObjectBase() : 所有シーンがnullptrです。\n");
	}

}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
	-------------------------------------
	引数：オブジェクト名
=========================================== */
void UIObjectBase::Init(std::string sName)
{
	m_pComponents.clear();	// コンポーネントリストをクリア
	SetName(sName);			// オブジェクト名を設定
	InitDefaultComponent();	// デフォルトコンポーネントを追加

	InitLocal();			// 個別初期化処理
}


/* ========================================
	終了関数
	-------------------------------------
	内容：終了処理
=========================================== */
void UIObjectBase::Uninit()
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
void UIObjectBase::Update()
{
	// 所持コンポーネント配列の全要素を更新
	for (auto& pComponent : m_pComponents)
	{
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
void UIObjectBase::Draw()
{
	// 所持コンポーネント配列の全要素を描画
	for (auto& pComponent : m_pComponents)
	{
		pComponent->Draw();
	}
	DrawLocal();	// 個別描画処理

}

/* ========================================
	親オブジェクト設定関数
	-------------------------------------
	内容：親オブジェクトを設定する
	-------------------------------------
	引数1：親に設定するオブジェクト
=========================================== */
void UIObjectBase::SetParentObject(UIObjectBase* pParentObj)
{
	// 既に親オブジェクトが設定されている場合
	if (m_pParentObj)
	{
		// 親オブジェクトから自身を削除
		m_pParentObj->RemoveChildObject(this);
	}

#ifdef _DEBUG

	// 削除するオブジェクト名をリストから削除
	ITEM_UI_LIST.RemoveListItem(this->GetName().c_str(), DebugUI::CHILD_HEAD_TEXT);

	m_pParentObj = pParentObj;	// 自オブジェクトの更新

	// 親オブジェクトのリスト番号取得
	int nParentNum = ITEM_UI_LIST.GetListNo(m_pParentObj->GetListName().c_str()) + 1;

	// オブジェクト一覧リストで親オブジェクトの下に追加
	ITEM_UI_LIST.InsertListItem(this->GetListName().c_str(), nParentNum);

	// 子オブジェクトを持っている場合
	for (auto& pChild : m_pChildObjs)
	{
		// 子オブジェクトの親を更新(リストの表示を更新するため)
		pChild->SetParentObject(this);
	}
#else
	m_pParentObj = pParentObj;	// 自オブジェクトの更新

#endif // _DEBUG

	// 登録した親オブジェクトの子オブジェクトリストを確認
	for (auto& pChild : m_pParentObj->m_pChildObjs)
	{
		// 既に親オブジェクトに自身が登録されている場合は処理しない
		if (pChild == this)
		{
			return;
		}
	}

	m_pParentObj->AddChildObject(this);	// 親オブジェクトに自身を登録
}

/* ========================================
	子オブジェクト追加関数
	-------------------------------------
	内容：子オブジェクトを追加する
	-------------------------------------
	引数1：子オブジェクトにするオブジェクト
=========================================== */
void UIObjectBase::AddChildObject(UIObjectBase* pChildObj)
{
	// 既に登録されているか確認
	for (auto& pChild : m_pChildObjs)
	{
		if (pChild == pChildObj)
		{
			return;
		}
	}

	m_pChildObjs.push_back(pChildObj);	// 子オブジェクトリストに追加

	// 既に子オブジェクトが更新済みかチェック
	// ※親オブジェクト設定関数から呼び出された場合
	if (pChildObj->GetParentObject() != this)
	{
		pChildObj->SetParentObject(this);	// 子オブジェクトの更新
	}

}

/* ========================================
	親オブジェクト解除関数
	-------------------------------------
	内容：親オブジェクトを解除する
=========================================== */
void UIObjectBase::RemoveParentObject()
{
	// 親オブジェクトがない場合は処理しない
	if (m_pParentObj == nullptr) return;

	m_pParentObj->RemoveChildObject(this);	// 親オブジェクトから自身を削除
	m_pParentObj = nullptr;					// 親オブジェクトを空に設定

	// 自身のTransformコンポーネントの親解除処理
	this->GetComponent<UIComponentTransform>()->ClearParent();	

#ifdef _DEBUG

	ITEM_UI_LIST.RemoveListItem(this->GetName().c_str(), DebugUI::CHILD_HEAD_TEXT);

	// オブジェクト一覧の一番下に追加
	ITEM_UI_LIST.AddListItem(this->GetListName().c_str());

#endif
}

/* ========================================
	子オブジェクト削除関数
	-------------------------------------
	内容：子オブジェクトを削除する
	-------------------------------------
	引数1：削除する子オブジェクト
=========================================== */
void UIObjectBase::RemoveChildObject(UIObjectBase* pChildObj)
{
	// 子オブジェクトを検索して配列から削除
	m_pChildObjs.erase(
		std::remove(m_pChildObjs.begin(), m_pChildObjs.end(), pChildObj), m_pChildObjs.end());


	pChildObj->m_pParentObj = nullptr;								// 親オブジェクトを空に設定
	pChildObj->GetComponent<UIComponentTransform>()->ClearParent();	// Transformコンポーネントの親解除処理

#ifdef _DEBUG

	ITEM_UI_LIST.RemoveListItem(pChildObj->GetName().c_str(), DebugUI::CHILD_HEAD_TEXT);

	// オブジェクト一覧の一番下に追加
	ITEM_UI_LIST.AddListItem(pChildObj->GetListName().c_str());

#endif
}

/* ========================================
	全子オブジェクト削除関数
	-------------------------------------
	内容：全子オブジェクトを削除する
=========================================== */
void UIObjectBase::RemoveAllChildObjects()
{
	// 子オブジェクトを全て配列から削除
	for (auto pChild : m_pChildObjs)
	{
		pChild->m_pParentObj = nullptr;									// 親オブジェクトを空に設定
		pChild->GetComponent<UIComponentTransform>()->ClearParent();	// Transformコンポーネントの親解除処理

#ifdef _DEBUG

		ITEM_UI_LIST.RemoveListItem(pChild->GetName().c_str(), DebugUI::CHILD_HEAD_TEXT);

		// オブジェクト一覧の一番下に追加
		ITEM_UI_LIST.AddListItem(pChild->GetListName().c_str());

#endif
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
int UIObjectBase::GetGenerationCount()
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
	デフォルトコンポーネント追加関数
	-------------------------------------
	内容：オブジェクトにデフォルトで持たせる
		　コンポーネントを追加する
=========================================== */
void UIObjectBase::InitDefaultComponent()
{
	AddComponent<UIComponentTransform>();	// Transformコンポーネントを追加	
}

/* ========================================
	ゲッター(所持シーン)関数
	-------------------------------------
	戻値：所有シーンのポインタ
=========================================== */
SceneBase* UIObjectBase::GetOwnerScene() const
{
	return m_pOwnerScene;
}

/* ========================================
	ゲッター(オブジェクトID)関数
	-------------------------------------
	戻値：オブジェクトID
=========================================== */
size_t UIObjectBase::GetTypeID() const
{
	return GetStaticTypeID();
}

/* ========================================
	ゲッター(静的オブジェクトID)関数
	-------------------------------------
	戻値：静的オブジェクトID
=========================================== */
size_t UIObjectBase::GetStaticTypeID()
{
	static size_t nID = reinterpret_cast<size_t>(&GetStaticTypeID); 
	return nID;													
}

/* ========================================
	ゲッター(オブジェクト名)関数
	-------------------------------------
	戻値：オブジェクト名
=========================================== */
std::string UIObjectBase::GetName() const
{
	return m_sName;
}

/* ========================================
	ゲッター(親オブジェクト)関数
	-------------------------------------
	戻値：親オブジェクト
=========================================== */
UIObjectBase* UIObjectBase::GetParentObject() const
{
	return m_pParentObj;
}

/* ========================================
	ゲッター(子オブジェクト)関数
	-------------------------------------
	戻値：子オブジェクト
=========================================== */
std::vector<UIObjectBase*> UIObjectBase::GetChildObjects() const
{
	return m_pChildObjs;
}

/* ========================================
	セッター(オブジェクト名)関数
	-------------------------------------
	引数1：オブジェクト名
=========================================== */
void UIObjectBase::SetName(std::string sName)
{
	m_sName = sName;
}

#ifdef _DEBUG
/* ========================================
	デバッグ用関数
	-------------------------------------
	内容：デバッグ用の処理
		　例：UI情報(所持コンポーネント等)の表示
=========================================== */
void UIObjectBase::Debug()
{
	DebugUI::Window& pObjInfo = WIN_UI_INFO;

	// オブジェクト詳細情報を更新
	pObjInfo.AddItem(DebugUI::Item::CreateValue("UIName", DebugUI::Item::Text, false));	// 名前
	pObjInfo["UIName"].SetText(this->GetName().c_str());							// オブジェクト名を設定

	// 各コンポーネント情報をオブジェクト情報ウィンドウに表示
	auto it = m_pComponents.begin();
	while (it != m_pComponents.end())
	{
		(*it)->Debug(pObjInfo);
		++it;
	}
}

/* ========================================
	リスト表示名取得関数
	-------------------------------------
	内容：リスト表示用の名前を取得する
	-------------------------------------
	戻り値：リスト表示用の名前
=========================================== */
std::string UIObjectBase::GetListName()
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