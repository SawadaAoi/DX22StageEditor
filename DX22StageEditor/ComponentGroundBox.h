/* ========================================
	CatRobotGame/
	------------------------------------
	�n�ʃR���|�[�l���g(�{�b�N�X)�p�w�b�_
	------------------------------------
	�����F�L���������n�ʂ̃R���|�[�l���g
		�@�@����O�p�`�̒��_���
		 ��AABB��OBB�̔��̓V�ʂɒn�ʂ�ݒ肷��
		 �@�O�p�`�̒��_���̋��ߕ��݂̂�ύX
	------------------------------------
	ComponentGroundBox.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentGround.h"

// =============== �N���X��` ===================
class ComponentGroundBox :
    public ComponentGround
{
public:
	ComponentGroundBox(ObjectBase* pOwner);

	DEFINE_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�

private:
	void UpdateTriangleVertex() override;	// �O�p�`�̒��_���X�V

};

