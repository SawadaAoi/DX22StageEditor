/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(敵キャラ)用ヘッダ
	------------------------------------
	説明：敵キャラクターの基底クラス
	------------------------------------
	ObjectEnemy.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectBase.h"

// =============== 前方宣言 =======================
class ComponentCollisionSphere;
class ComponentRigidbody;
class ComponentGroundRaycast;
class ComponentModelAnime;
class ComponentEnemyState;

// =============== クラス定義 =====================
class ObjectEnemy :
    public ObjectBase
{
public:
	ObjectEnemy(SceneBase* pScene);
	void InitLocal();
	void UpdateLocal();

	void OnCollisionStay(ObjectBase* pHit) override;

	DEFINE_OBJECT_TYPE(ObjectEnemy)	// オブジェクトの種類ID取得関数

private:
	void Damage();
private:
	ComponentCollisionSphere*	m_pCompColSphere;		// 衝突判定用コンポーネント
	ComponentRigidbody*			m_pCompRigidbody;		// 物理演算用コンポーネント
	ComponentGroundRaycast*		m_pCompGroundRaycast;	// 地面判定用コンポーネント
	ComponentModelAnime*		m_pCompModelAnime;		// モデルアニメーション用コンポーネント
	ComponentEnemyState*		m_pCompEnemyState;		// 敵キャラ状態管理コンポーネント

	// HP
	int m_nHp;
	int m_nMaxHp;
};

