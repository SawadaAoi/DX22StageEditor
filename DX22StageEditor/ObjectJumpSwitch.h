/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�W�����v�o�l�X�C�b�`)�p�w�b�_
	------------------------------------
	�����F�W�����v�o�l�̃W�����v�͂��ω����铖���蔻��p
		�@���W�����v�o�l�̓V�ʂɔz�u
	------------------------------------
	ObjectJumpSwitch.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBase.h"

// =============== �O���錾 =======================
class ComponentCollisionAABB;
class ObjectJumpPad;

// =============== �N���X��` ===================
class ObjectJumpSwitch :
    public ObjectBase
{
public:
	ObjectJumpSwitch(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;
	void OnCollisionStay(ObjectBase* pHit) override;
	void OnCollisionExit(ObjectBase* pHit) override;
private:
	ObjectJumpPad*				m_pJumpPad;			// �W�����v�p�b�h(�e)
	ComponentCollisionAABB*		m_pCompColAABB;		// �Փ˔���
	float						m_fObjJumpPower;	// �W�����v��

	
};

