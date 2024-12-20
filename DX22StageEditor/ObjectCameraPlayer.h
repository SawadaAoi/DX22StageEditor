/* ========================================
	CatRobotGame/
	------------------------------------
	�J�����I�u�W�F�N�g(�v���C���[)�p�w�b�_
	------------------------------------
	�����F�v���C���[�ɒǏ]����J����
		�@�v���C���[�𒆐S�ɉ���]�A�c��]���\
	------------------------------------
	ObjectCameraPlayer.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectCamera.h"

// =============== �N���X�̒�` =====================
class ObjectCameraPlayer :
    public ObjectCamera
{
public:
	ObjectCameraPlayer(SceneBase* pScene);

	void InitLocal() override;
	void UpdateLocal() override;

	// �Q�b�^�[
	float GetDistance();	// �J�����ƃv���C���[�̋����擾�֐�
	float GetRadY();		// �J������Y����]�p�x�擾�֐�
	float GetRadXZ();		// �J������XZ���ʉ�]�p�x�擾�֐�

	// �Z�b�^�[
	void SetDistance(float fDistance);	// �J�����ƃv���C���[�̋����ݒ�֐�
	void SetRadY(float fRadY);			// �J������Y����]�p�x�ݒ�֐�
	void SetRadXZ(float fRadXZ);		// �J������XZ���ʉ�]�p�x�ݒ�֐�

	DEFINE_OBJECT_TYPE(ObjectCameraPlayer)	// �I�u�W�F�N�g�̎��ID�擾�֐�
private:
	void CheckLimitRadY();	// Y����]�p�x�����֐�
private:
	ObjectBase* m_pPlayer;

	float m_fRotSpeed;	// ��]���x
	float m_fDistance;	// �J�����ƃv���C���[�̋���
	float m_fRadY;		// �J������Y����]�p�x
	float m_fRadXZ;		// �J������XZ���ʉ�]�p�x
};

