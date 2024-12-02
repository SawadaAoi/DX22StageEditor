/* ========================================
	DX22Base/
	------------------------------------
	オブジェクト用ヘッダ
	------------------------------------
	説明：各オブジェクトの基底クラス
	------------------------------------
	ObjectBase.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "vector"
#include "ComponentBase.h"	// コンポーネント基底クラス
#include "TimeManager.h"	// 経過時間をどこでも取得できるようにするため
#include "DebugConsole.h"	// デバッグコンソール出力用
#include <memory>			// std::unique_ptr用
#include "ObjectTag.h"

// =============== 前方宣言 =====================
class SceneBase;	// シーン基底クラス


// =============== 定数定義 =======================

// オブジェクトID取得関数の定義を省略するためのマクロ
// 使い方：クラス定義内でDEFINE_OBJECT_TYPE(クラス名)を記述する
// 例：DEFINE_OBJECT_TYPE(ObjectPlayer);
// 関数のポインタをIDとして扱うため、コンポーネントの種類によって異なるIDを取得できる
#define DEFINE_OBJECT_TYPE(name)												\
    static size_t GetStaticTypeID() {									\
		static const size_t typeID = reinterpret_cast<size_t>(&typeID); \
		return typeID;													\
    }																	\
    size_t GetTypeID() const override {									\
        return GetStaticTypeID();										\
    }																	\
	std::string GetObjClassName() const override {						\
		return #name;													\
	} 

// =============== クラス定義 =====================
class ObjectBase
{
public:

	// オブジェクトの状態
	enum E_State
	{
		STATE_ACTIVE,	// アクティブ
		STATE_PAUSE,	// 一時停止
		STATE_DEAD,		// 死亡

		STATE_MAX,		// 状態の最大数
	};

public:
	ObjectBase() = delete;			// デフォルトコンストラクタは禁止
	ObjectBase(SceneBase* pScene);	// コンストラクタ(シーンを設定)
	virtual ~ObjectBase() {};

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

	virtual void OnCollisionEnter(ObjectBase* pHit);	// 衝突開始処理
	virtual void OnCollisionStay(ObjectBase* pHit);		// 衝突中処理
	virtual void OnCollisionExit(ObjectBase* pHit);		// 衝突終了処理

	// 親子関係
	void SetParentObject(ObjectBase* pParentObj);	// 親オブジェクト設定
	void AddChildObject(ObjectBase* pChildObj);		// 子オブジェクト追加
	void RemoveParentObject();						// 親オブジェクト解除
	void RemoveChildObject(ObjectBase* pChildObj);	// 子オブジェクト解除
	void RemoveAllChildObjects();					// 全子オブジェクト解除
	int GetGenerationCount();						// 世代数の取得

	// ゲッター
	SceneBase* GetOwnerScene() const;					// 所有シーンの取得
	E_State GetState() const;							// 状態の取得
	virtual size_t GetTypeID() const;					// コンポーネントの種類IDの取得
	static size_t GetStaticTypeID();					// コンポーネントの種類IDの取得(静的)
	virtual std::string GetObjClassName() const;		// クラス名の取得
	ObjectBase* GetParentObject() const;				// 親オブジェクトの取得
	std::vector<ObjectBase*> GetChildObjects() const;	// 子オブジェクトの取得
	E_ObjectTag GetTag() const;							// タグの取得
	std::string GetName() const;						// オブジェクト名の取得

	// セッター
	void SetState(E_State eState);		// 状態の設定
	void SetTag(E_ObjectTag eTag);		// タグの設定
	void SetName(std::string sName);	// オブジェクト名の設定

#ifdef _DEBUG
	void Debug();					// デバッグ用の処理(オブジェクト情報ウィンドウに表示)
	virtual void DebugLocal() {};	// 個別デバッグ処理
	void ChangeName();	// 名前変更

	std::string GetListName();	// リスト表示用の名前取得
#endif

	// コンポーネント関連
	template<typename T>
	T* AddComponent();	// コンポーネント追加

	template<typename T>
	T* GetComponent();	// コンポーネント取得

	template<typename T>
	void RemoveComponent();	// コンポーネント削除

private:
	void InitDefaultComponent();	// デフォルトコンポーネント設定

protected:
	SceneBase* m_pOwnerScene;	// 所有シーン
	std::vector<std::unique_ptr<ComponentBase>> m_pComponents;	// コンポーネント一覧

	std::string m_sName;	// オブジェクト名
	E_State		m_eState;	// オブジェクトの状態
	E_ObjectTag m_eTag;		// オブジェクトの分類

	// 親子関係
	ObjectBase*					m_pParentObj;	// 親オブジェクト
	std::vector<ObjectBase*>	m_pChildObjs;	// 子オブジェクト

};

#include "ObjectBase.inl"
