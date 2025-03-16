/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�W�����v�o�l)�p�w�b�_
	------------------------------------
	�����FRigidbody�����I�u�W�F�N�g����ɏ���
		  �W�����v�͂�^����
		  ObjectJumpPad
		  L m_pSpringModelObj
		   L m_pJumpSwitchObj
	------------------------------------
	ObjectJumpPad.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBase.h"

// =============== �O���錾 =======================
class ComponentModelStatic;
class ComponentCollisionAABB;
class ObjectJumpSwitch;

// =============== �N���X��` ===================
class ObjectJumpPad :
    public ObjectBase
{
public:
	enum E_SpringState
	{
		Stay,		// ���̂܂�
		Stretch,	// �L�т�
		Shrink,		// �k��
	};
public:
	ObjectJumpPad(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	// �Q�b�^�[
	float GetStretchRate();

	// �Z�b�^�[
	void SetSpringState(E_SpringState eState);

	DEFINE_OBJECT_TYPE(ObjectJumpPad)
private:
	void InitSpringModel();
	void InitJumpSwitch();
private:
	ObjectBase*			m_pSpringModelObj;	// �o�l�I�u�W�F�N�g
	ObjectJumpSwitch*	m_pJumpSwitchObj;	// �W�����v�X�C�b�`

	E_SpringState	m_eSpringState;			// �o�l�̏��
	float			m_fStretchTime;			// �L�т鎞��

	Vector3<float> m_vBasePos;				// �x�[�X�|�W�V����

};

