/* ========================================
	DX22Base/
	------------------------------------
	シーン用ヘッダ
	------------------------------------
	説明：各シーンの基底クラス
	------------------------------------
	SceneBase.h
========================================== */
#pragma once

// =============== インクルード =====================
#include <vector>
#include "ObjectBase.h"
#include <memory>
#include "DebugMenu.h"
#include "UIObjectBase.h"

// =============== 前方宣言 =======================
class ComponentCollisionBase;

// =============== 定数定義 =======================
// E_Stateの呼び出しを省略
#define OBJ_ACTIVE	ObjectBase::E_State::STATE_ACTIVE
#define OBJ_DEAD	ObjectBase::E_State::STATE_DEAD
#define OBJ_PAUSE	ObjectBase::E_State::STATE_PAUSE

// シーン名取得関数の定義を省略するためのマクロ
#define DEFINE_SCENE_TYPE(name)				\
	std::string GetSceneName() override {	\
		return #name;						\
	} 

// =============== クラス定義 =====================
class SceneBase
{
public:
	SceneBase();
	virtual ~SceneBase() {};

	void Init();	// 初期化処理
	void Uninit();	// 終了処理
	void Update();	// 更新処理
	void Draw();	// 描画処理

	virtual void InitLocal() {};	// 個別初期化処理
	virtual void UninitLocal() {};	// 個別終了処理
	virtual void UpdateLocal() {};	// 個別更新処理
	virtual void DrawLocal() {};	// 個別描画処理

	void UpdateObject();
	void RemoveDeadObjects();	// 死亡状態オブジェクトを削除

	// オブジェクト関連
	template<typename T>
	T* AddSceneObject(std::string sName);	// オブジェクト追加
	template<typename T>
	T* GetSceneObject(std::string sName);	// オブジェクト取得(名前指定)
	template<typename T>
	T* GetSceneObject();					// オブジェクト取得(型検索(最初に見つかったもの))
	template<typename T>
	std::vector<T*> GetSceneObjects();		// オブジェクト取得(型検索(全て))

	void AddSceneObjectBase(ObjectBase* pObject);	// オブジェクト追加
	ObjectBase* FindSceneObject(std::string sName);	// オブジェクト検索

	void RemoveSceneObject(ObjectBase* pObject);	// オブジェクト削除


	// 衝突判定関連
	void AddObjectCollision(ComponentCollisionBase* pCollision);	// 衝突判定コンポーネント追加
	void RemoveObjectCollision(ComponentCollisionBase* pCollision);	// 衝突判定コンポーネント削除

	// 名前連番付与関数(重複チェック)
	std::string CreateUniqueName(std::string sName);	

	// ゲッター
	virtual std::string GetSceneName();								// シーン名取得
	std::vector<ObjectBase*> GetAllSceneObjects();					// シーン上に存在する全てのオブジェクトを取得 
	std::vector<ObjectBase*> GetSceneObjectsTag(E_ObjectTag tag);	// タグに対応するオブジェクトを取得
	ObjectBase* GetSceneObjectTag(E_ObjectTag tag);					// タグに対応するオブジェクトを取得(一番最初に見つかったもの)

#ifdef _DEBUG
	void SetTransformEditMode(int nMode);	// 変形エディタのモードを設定
	void ResetTransformEdit();				// 変形エディタのリセット
	void ReloadDebugObjectList();			// オブジェクトリスト再読み込み
#endif // _DEBUG
private:
	void UpdateCollision();	// 衝突判定更新関数
	bool CheckUniqueName(std::string sName, std::vector<ObjectBase*> pObjects);	// 名前重複チェック

#ifdef _DEBUG
	void InitObjectList();
	void InitObjectInfo(std::string sName);

	void UpdateTransformEdit();	// 変形エディタの更新
	void AddObjectListChild(ObjectBase* pObject);

#endif // _DEBUG
private:
	std::vector<std::unique_ptr<ObjectBase>>	m_pObjects;			// シーンに所属するオブジェクト一覧
	std::vector<std::unique_ptr<ObjectBase>>	m_pStandbyObjects;	// オブジェクトを一時的に保存しておく配列
	std::vector<ComponentCollisionBase*>		m_pObjectCollision;	// 各オブジェクトが持つ衝突判定コンポーネント

	bool m_bIsUpdating;	// 更新中かどうか
#ifdef _DEBUG
	ObjectBase* m_pSelectObj;	// 一覧で選択中のオブジェクト
	int m_nObjectListSelectNo;	// オブジェクトリストの番号
	int m_nTransEditMode;		// 変形エディタのモード(0:なし, 1:移動, 2:回転, 3:拡大)
#endif // _DEBUG
};

#include "SceneBase.inl"