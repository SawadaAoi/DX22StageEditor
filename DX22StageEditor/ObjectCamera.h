/* ========================================
	CatRobotGame/
	------------------------------------
	�J�����I�u�W�F�N�g(��b)�p�w�b�_
	------------------------------------
	�����F�J�����I�u�W�F�N�g�̊�b�������Ǘ�����
	------------------------------------
	ObjectCamera.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectBase.h"
#include <DirectXMath.h>

// =============== �O���錾 =======================
class ComponentTransform;
class ComponentCameraBase;

// =============== �N���X��` =====================
class ObjectCamera :
	public ObjectBase
{
public:
	ObjectCamera(SceneBase* pScene);

	void InitLocal() override;
	void UninitLocal() override;

	// �Q�b�^�[
	DirectX::XMFLOAT4X4 GetViewMatrix();		// �r���[�s��擾�֐�
	DirectX::XMMATRIX GetInvViewMatrix();		// �r���[�s��̋t�s��擾�֐�

	DirectX::XMFLOAT4X4 GetProjectionMatrix();	// �v���W�F�N�V�����s��擾�֐�
	DirectX::XMFLOAT4X4 GetProjectionMatrixUI();	// 2D�p�v���W�F�N�V�����s��擾�֐�
	DirectX::XMFLOAT4X4 GetProjectionMatrixOrtho();	// ���s���e�v���W�F�N�V�����s��擾�֐�

	bool GetActive();

	ComponentCameraBase* GetCameraBase();

	DEFINE_OBJECT_TYPE(ObjectCamera)	// �I�u�W�F�N�g�̎��ID�擾�֐�

		// �Z�b�^�[
		void SetActive(bool bActive);

protected:
	ComponentCameraBase* m_pCompCameraBase;	// �J�����̊�{�@�\

	bool m_bActive;	// �A�N�e�B�u���
};
