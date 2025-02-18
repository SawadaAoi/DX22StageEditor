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
#include <fstream>			// ファイル入出力用

// =============== 前方宣言 =====================
class SceneBase;	// シーン基底クラス
class ComponentTransform;	// トランスフォームコンポーネント

// =============== 定数定義 =======================

// オブジェクトID取得関数の定義を省略するためのマクロ
// 使い方：クラス定義内でDEFINE_OBJECT_TYPE(クラス名)を記述する
// 例：DEFINE_OBJECT_TYPE(ObjectPlayer);
// 関数のポインタをIDとして扱うため、コンポーネントの種類によって異なるIDを取得できる
#define DEFINE_OBJECT_TYPE(name)										\
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

	// ライトパラメータ
	struct T_LightParam
	{
		float fDiffuse;		// 拡散反射値
		float fSpecular;	// 鏡面反射値
		float fAmbient;		// 環境光値
		bool  bLightUse;	// ライト使用フラグ
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

	// オブジェクトの破棄
	void Destroy(float nTime = 0.0f);

	// 衝突処理
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
	bool CheckIsDescendant(ObjectBase* pObject);	// 子孫かどうかの判定

	// コピー関数
	ObjectBase* Copy();								// オブジェクトのコピー
	virtual void CopyLocal(ObjectBase* pObject);	// オブジェクトのコピー(個別処理)

	virtual void OutPutLocalData(std::ofstream& file);	// オブジェクトのデータ出力
	virtual void InputLocalData(std::ifstream& file);	// オブジェクトのデータ入力

	// ゲッター
	ComponentTransform* GetTransform() const;			// トランスフォームコンポーネントの取得
	SceneBase* GetOwnerScene() const;					// 所有シーンの取得
	E_State GetState() const;							// 状態の取得
	virtual size_t GetTypeID() const;					// コンポーネントの種類IDの取得
	static size_t GetStaticTypeID();					// コンポーネントの種類IDの取得(静的)
	virtual std::string GetObjClassName() const;		// クラス名の取得
	ObjectBase* GetParentObject() const;				// 親オブジェクトの取得
	std::vector<ObjectBase*> GetChildObjects() const;	// 子オブジェクトの取得
	E_ObjectTag GetTag() const;							// タグの取得
	std::string GetName() const;						// オブジェクト名の取得
	T_LightParam GetLightMaterial() const;				// ライトパラメータの取得
	bool GetIsSave() const;								// セーブするかどうかの取得
	bool GetIsFold() const;								// オブジェクト一覧折りたたみフラグの取得

	// セッター
	void SetState(E_State eState);		// 状態の設定
	void SetTag(E_ObjectTag eTag);		// タグの設定
	void SetName(std::string sName);	// オブジェクト名の設定
	void SetLightMaterial(float fDiffuse = 1.0f, float fSpecular = 0.0f, float fAmbient = 0.3f);	// ライトパラメータの設定
	void SetLightUse(bool bUse);		// ライト使用の設定
	void SetIsSave(bool bIsSave);		// セーブするかどうかの設定
	void SetIsFold(bool bIsFold);		// オブジェクト一覧折りたたみフラグの設定


	// コンポーネント関連
	template<typename T>
	T* AddComponent();	// コンポーネント追加

	template<typename T>
	T* GetComponent();	// コンポーネント取得

	template<typename T>
	void RemoveComponent();	// コンポーネント削除

#ifdef _DEBUG
	void Debug();					// デバッグ用の処理(オブジェクト情報ウィンドウに表示)
	virtual void DebugLocal(DebugUI::Window& window) {};	// 個別デバッグ処理

	DebugUI::Item* InitParentList();	// 親リストの初期化

	void ChangeName();			// 名前変更
	void ChangeParentList(std::string sParentName);		// 親変更

	std::string GetListName();	// リスト表示用の名前取得

#endif
private:
	void InitDefaultComponent();	// デフォルトコンポーネント設定

protected:
	SceneBase* m_pOwnerScene;	// 所有シーン
	std::vector<std::unique_ptr<ComponentBase>> m_pComponents;	// コンポーネント一覧

	ComponentTransform* m_pCompTransform;	// トランスフォームコンポーネント

	std::string m_sName;	// オブジェクト名
	E_State		m_eState;	// オブジェクトの状態
	E_ObjectTag m_eTag;		// オブジェクトの分類

	// 親子関係
	ObjectBase*					m_pParentObj;	// 親オブジェクト
	std::vector<ObjectBase*>	m_pChildObjs;	// 子オブジェクト

	T_LightParam m_tLightParam;	// ライトパラメータ

	// セーブするかどうか(実行中に自身の子クラスを作成したオブジェクトの保存時に、子クラスを保存しないようにするため)
	bool m_bIsSave;	

	// オブジェクト一覧折りたたみフラグ(true:折りたたむ, false:展開)
	bool m_bIsFold;

	bool m_bIsDestroy;			// オブジェクト破棄フラグ(true:破棄, false:未破棄)
	float m_fDestroyTime;		// 破棄時間
	float m_fDestroyTimeCnt;	// 破棄時間カウント
};

#include "ObjectBase.inl"
