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

	// オブジェクト関連
	template<typename T>
	T* AddSceneObject(std::string sName);	// オブジェクト追加
	template<typename T>
	T* GetSceneObject(std::string sName);	// オブジェクト取得
	template<typename T>
	std::vector<T*> GetSceneObjects();		// オブジェクト取得

	void AddSceneObjectBase(ObjectBase* pObject);	// オブジェクト追加
	ObjectBase* FindSceneObject(std::string sName);	// オブジェクト検索

	// 衝突判定関連
	void AddObjectCollision(ComponentCollisionBase* pCollision);	// 衝突判定コンポーネント追加
	void RemoveObjectCollision(ComponentCollisionBase* pCollision);	// 衝突判定コンポーネント削除


	// ゲッター
	std::vector<ObjectBase*> GetAllSceneObjects();					// シーン上に存在する全てのオブジェクトを取得 
	std::vector<ObjectBase*> GetSceneObjectsTag(E_ObjectTag tag);	// タグに対応するオブジェクトを取得
private:
	void UpdateCollision();	// 衝突判定更新関数

#ifdef _DEBUG
	void InitObjectList();
	void InitObjectInfo(std::string sName);
#endif // _DEBUG
private:
	std::string CreateUniqueName(std::string sName);	// 名前が重複している場合は連番を付ける
private:
	std::vector<std::unique_ptr<ObjectBase>>	m_pObjects;			// シーンに所属するオブジェクト一覧
	std::vector<std::unique_ptr<ObjectBase>>	m_pStandbyObjects;	// オブジェクトを一時的に保存しておく配列
	std::vector<ComponentCollisionBase*>		m_pObjectCollision;	// 各オブジェクトが持つ衝突判定コンポーネント

	bool m_bIsUpdating;	// 更新中かどうか

};

#include "SceneBase.inl"