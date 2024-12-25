/* ========================================
	CatRobotGame/
	------------------------------------
	�n�ʃR���|�[�l���g(�{�b�N�X)�pcpp
	------------------------------------
	ComponentGroundBox.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ComponentGroundBox.h"
#include "ComponentTransform.h"
#include <vector>

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�I�u�W�F�N�g
=========================================== */
ComponentGroundBox::ComponentGroundBox(ObjectBase* pOwner)
	: ComponentGround(pOwner)
{
}

/* ========================================
	�O�p�`���_�X�V�֐�
	-------------------------------------
	���e�F���L�I�u�W�F�N�g�̃X�P�[���A���W�ɍ��킹��
		�@�O�p�`�̒��_���X�V
=========================================== */
void ComponentGroundBox::UpdateTriangleVertex()
{
	// ���L�I�u�W�F�N�g�̍��W�A��]�A�X�P�[�����擾
	Vector3<float> vScale	= m_pCompTransform->GetWorldScale();
	Vector3<float> vPos		= m_pCompTransform->GetWorldPosition();
	Quaternion vRot			= m_pCompTransform->GetWorldRotation();

	// �l�p�`�̏�ʂ̒��_���W���v�Z
	std::vector<Vector3<float>> vVertexPos;		// ���_���W
	Vector3<float> vHalfScale = vScale / 2.0f;	// �����̃X�P�[��

	// ���_�ɂ��鎞�̃{�b�N�X�̓V�ʂ̒��_���W�����߂�
	vVertexPos.push_back(Vector3<float>(vHalfScale.x, vHalfScale.y, vHalfScale.z));		// �E��
	vVertexPos.push_back(Vector3<float>(-vHalfScale.x, vHalfScale.y, vHalfScale.z));	// ����
	vVertexPos.push_back(Vector3<float>(vHalfScale.x, vHalfScale.y, -vHalfScale.z));	// �E��
	vVertexPos.push_back(Vector3<float>(-vHalfScale.x, vHalfScale.y, -vHalfScale.z));	// ����

	// ���W�A��]��K�p
	for (int i = 0; i < vVertexPos.size(); i++)
	{
		// ��]
		vVertexPos[i] = vRot.Rotate(vVertexPos[i]);
		// ���W
		vVertexPos[i] += vPos;
	}

	// �O�p�`�̒��_��ݒ�
	// 1�ڂ̎O�p�` (����A�E��A����)
	m_TriangleVertices[0].pos[0] = vVertexPos[1];	// ����
	m_TriangleVertices[0].pos[1] = vVertexPos[0];	// �E��
	m_TriangleVertices[0].pos[2] = vVertexPos[2];	// ����

	// 2�ڂ̎O�p�` (�E��A�E���A����)
	m_TriangleVertices[1].pos[0] = vVertexPos[0];	// �E��
	m_TriangleVertices[1].pos[1] = vVertexPos[3];	// �E��
	m_TriangleVertices[1].pos[2] = vVertexPos[2];	// ����
	
}
