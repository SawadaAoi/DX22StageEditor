/* ========================================
	DX22Base/
	------------------------------------
	UIオブジェクト基底用ヘッダ
	------------------------------------
	説明：UIオブジェクトの基底クラス
	------------------------------------
	UIObjectBase.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "vector"
#include <string>
#include "UIComponentBase.h"	// UIコンポーネント基底クラス
#include <memory>			// std::unique_ptr用
#include "TimeManager.h"	// 経過時間をどこでも取得できるようにするため


// =============== 前方宣言 =====================
class SceneBase;	// シーン基底クラス

// =============== 定数定義 =======================
// オブジェクトID取得関数の定義を省略するためのマクロ
// 使い方：クラス定義内でDEFINE_OBJECT_TYPE(クラス名)を記述する
// 例：DEFINE_OBJECT_TYPE(ObjectPlayer);
// 関数のポインタをIDとして扱うため、コンポーネントの種類によって異なるIDを取得できる
#define DEFINE_UI_OBJECT_TYPE											\
    static size_t GetStaticTypeID() {									\
		static const size_t typeID = reinterpret_cast<size_t>(&typeID); \
        return typeID;													\
    }																	\
    size_t GetTypeID() const override {									\
        return GetStaticTypeID();										\
    }


// =============== クラス定義 =====================
class UIObjectBase
{
public:
	UIObjectBase() = delete;
	UIObjectBase(SceneBase* pScene);
	virtual ~UIObjectBase() {};

	// 処理が変わらない関数
	void Init(std::string sName);
	void Uninit();
	void Update();
	void Draw();

	// 子クラスで実装する関数
	virtual void InitLocal() {};	// 個別初期化処理
	virtual void UninitLocal() {};	// 個別終了処理
	virtual void UpdateLocal() {};	// 個別更新処理
	virtual void DrawLocal() {};	// 個別描画処理

	// 親子関係
	void SetParentObject(UIObjectBase* pParentObj);		// 親オブジェクトの設定
	void AddChildObject(UIObjectBase* pChildObj);		// 子オブジェクトの追加
	void RemoveParentObject();							// 親オブジェクトの削除
	void RemoveChildObject(UIObjectBase* pChildObj);	// 子オブジェクトの削除
	void RemoveAllChildObjects();						// 全ての子オブジェクトの削除
	int GetGenerationCount();							// 子オブジェクトの数を取得

	// ゲッター
	SceneBase* GetOwnerScene() const;					// 所有シーンの取得
	virtual size_t GetTypeID() const;					// コンポーネントの種類IDの取得
	static size_t GetStaticTypeID();					// コンポーネントの種類IDの取得
	std::string GetName() const;						// オブジェクト名の取得
	UIObjectBase* GetParentObject() const;				// 親オブジェクトの取得
	std::vector<UIObjectBase*> GetChildObjects() const;	// 子オブジェクトの取得

	// セッター
	void SetName(std::string sName);	// オブジェクト名の設定

	// コンポーネント関連
	template<typename T>
	T* AddComponent();	// コンポーネント追加

	template<typename T>
	T* GetComponent();	// コンポーネント取得

	template<typename T>
	void RemoveComponent();	// コンポーネント削除

#ifdef _DEBUG
	void Debug();					// デバッグ用の処理(オブジェクト情報ウィンドウに表示)
	virtual void DebugLocal() {};	// 個別デバッグ処理

	std::string GetListName();	// リスト表示用の名前取得
#endif

private:
	void InitDefaultComponent();	// デフォルトコンポーネント設定

protected:
	SceneBase*										m_pOwnerScene;	// 所有シーン
	std::vector<std::unique_ptr<UIComponentBase>>	m_pComponents;	// コンポーネント一覧

	std::string m_sName;		// オブジェクト名
	
	// 親子関係
	UIObjectBase*				m_pParentObj;	// 親オブジェクト
	std::vector<UIObjectBase*>	m_pChildObjs;	// 子オブジェクト
};

#include "UIObjectBase.inl"	// コンポーネント関連のテンプレート関数の実装