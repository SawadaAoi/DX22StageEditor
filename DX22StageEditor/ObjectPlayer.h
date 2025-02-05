/* ========================================
	DX22Base/
	------------------------------------
	オブジェクト(プレイヤー)用ヘッダ
	------------------------------------
	説明：プレイヤー用
	------------------------------------
	ObjectPlayer.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectBase.h"
#include <memory>

// =============== 前方宣言 =====================
class ComponentTransform;
class ComponentGroundRaycast;
class ComponentRigidbody;
class ComponentModelStatic;
class ComponentModelAnime;
class ComponentPlayerController;

// =============== クラス定義 =====================
class ObjectPlayer :
    public ObjectBase
{
public:
	// プレイヤーの状態
	// クリアアニメーションが他のアニメ(歩き等)に上書きされるのを防ぐため
	enum E_PlayerState
	{
		PS_Normal,
		PS_Dead,
		PS_GameClear,
	};
public:
	ObjectPlayer(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	void GameClear();
	void Dead();

	void OnCollisionStay(ObjectBase* pHit) override;

	// ゲッター
	int GetHp();
	int GetMaxHp();
	bool GetInvincible();

	// セッター
	void SetHp(int hp);
	void SetMaxHp(int maxHp);
	void SetInvincible(bool bInvincible);

	DEFINE_OBJECT_TYPE(ObjectPlayer)	// オブジェクトの種類ID取得関数

private:
	void CheckGround();
	void Damage();
	void InvincibleUpdate();
private:
	ComponentGroundRaycast*		m_pCompGroundRaycast;
	ComponentRigidbody*			m_pCompRigidbody;
	ComponentModelStatic*		m_pCompModelStatic;
	ComponentModelAnime*		m_pCompModelAnime;
	ComponentPlayerController*	m_pCompPlayerController;

	// HP
	int m_nHp;
	int m_nMaxHp;

	// 無敵時間
	bool	m_bInvincible;	// 無敵時間フラグ
	float	m_fInvCnt;		// 無敵時間カウント
	float	m_fInvFlashCnt;	// 無敵時間点滅カウント

	E_PlayerState m_ePlayerState;
};

