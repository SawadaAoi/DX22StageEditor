/* ========================================
	DX22Base/
	------------------------------------
	�I�u�W�F�N�g(�v���C���[)�p�w�b�_
	------------------------------------
	�����F�v���C���[�p
	------------------------------------
	ObjectPlayer.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectBase.h"
#include <memory>

// =============== �O���錾 =====================
class ComponentTransform;
class ComponentGroundRaycast;
class ComponentRigidbody;
class ComponentModelStatic;
class ComponentModelAnime;
class ComponentPlayerController;

// =============== �N���X��` =====================
class ObjectPlayer :
    public ObjectBase
{
public:
	// �v���C���[�̏��
	// �N���A�A�j���[�V���������̃A�j��(������)�ɏ㏑�������̂�h������
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

	// �Q�b�^�[
	int GetHp();
	int GetMaxHp();
	bool GetInvincible();

	// �Z�b�^�[
	void SetHp(int hp);
	void SetMaxHp(int maxHp);
	void SetInvincible(bool bInvincible);

	DEFINE_OBJECT_TYPE(ObjectPlayer)	// �I�u�W�F�N�g�̎��ID�擾�֐�

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

	// ���G����
	bool	m_bInvincible;	// ���G���ԃt���O
	float	m_fInvCnt;		// ���G���ԃJ�E���g
	float	m_fInvFlashCnt;	// ���G���ԓ_�ŃJ�E���g

	E_PlayerState m_ePlayerState;
};

