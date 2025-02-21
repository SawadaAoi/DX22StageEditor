/* ========================================
	CatRobotGame/
	------------------------------------
	�R���|�[�l���g(�ۉe)�p�w�b�_
	------------------------------------
	�����F�I�u�W�F�N�g(�v���C���[��G)��
		�@���ɕ\������ۉe
	------------------------------------
	ComponentShadow.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentBase.h"


// =============== �N���X��` =====================
class ComponentShadow :
    public ComponentBase
{
public:
	ComponentShadow(ObjectBase* pOwner);
	virtual void Init() override;
	virtual void Uninit() override;
	virtual void Update() override;

	// �Q�b�^�[
	Vector3<float> GetPosition() const;
	float GetCircleSize() const;

	// �Z�b�^�[
	void SetCircleSize(float range);
	void SetPosOffset(Vector3<float> vOffset);

	DEFINE_COMPONENT_TYPE
private:
	float	m_fCircleSize;			// �ۉe�̑傫��
	Vector3<float> m_vPosOffset;	// �e�̊���W�̒����l
};

