/* ========================================
	CatRobotGame/
	------------------------------------
	�G�L�����U���R���|�[�l���g(�ːi)�p�w�b�_
	------------------------------------
	�����F�G�L�����̓ːi�U�������N���X
	------------------------------------
	ComponentEnemyAttackCharge.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentEnemyAttackBase.h"
#include <memory>

// =============== �O���錾 =======================
class ObjectBase;
class ShapeSphere;

// =============== �N���X��` ===================
class ComponentEnemyAttackCharge :
    public ComponentEnemyAttackBase
{
public:
	// �ːi���
	enum E_ChargeState
	{
		CS_IDLE,	// �ҋ@
		CS_ATTACK,	// �ːi
	};
public:
	ComponentEnemyAttackCharge(ObjectBase* pOwner);
	void Init() override;
	void Update() override;
	void Draw() override;

	// �Q�b�^�[
	E_ChargeState GetChargeState();
	float GetChargeDist();
	float GetChargeSpeed();

	// �Z�b�^�[
	void SetChargeState(E_ChargeState eState);
	void SetChargeDist(float fDist);
	void SetChargeSpeed(float fSpeed);

	DEFINE_COMPONENT_TYPE
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	void Idle();
	void Attack();
private:
	ObjectBase*		m_pTarget;		// �^�[�Q�b�g
	E_ChargeState	m_eChargeState;	// �ːi���

	float m_fChargeDist;	// �ːi����
	float m_fChargeSpeed;	// �ːi���x

	Vector3<float> m_vChargeEndPos;		// �ːi�I���ʒu

	bool m_bDispChargeEndSphere;	// �ړI���W�\���t���O
	std::unique_ptr<ShapeSphere> m_pChargeEndSphere;	// �ړI���W�`��p

};

