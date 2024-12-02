/* ========================================
	DX22Base/
	------------------------------------
	�J�����Ǘ��p�w�b�_
	------------------------------------
	����:�J�������Ǘ�����N���X(�K�X�؂�ւ���)
	------------------------------------
	CameraManager.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectCamera.h"	// �J�����I�u�W�F�N�g

// =============== �O���錾 =======================
class SceneBase;

// =============== �萔��` =====================
#define CAMERA_MNG_INST CCameraManager::GetInstance()	// instance�擾


// =============== �N���X��` =====================
class CCameraManager
{
public:
	static CCameraManager& GetInstance();	// �C���X�^���X�擾
	void Init(SceneBase* pScene);	// �V�[���J�n���ɃJ����������
	
	void SwitchCamera(int num);	// �J�����؂�ւ�

	void AddCamera(ObjectCamera* pCamera);	// �J�����ǉ�
	void RemoveCamera(ObjectCamera* pCamera);	// �J�����폜

	// �Q�b�^�[
	ObjectCamera* GetActiveCamera();
	int GetCameraNum();

private:
	CCameraManager();	// �R���X�g���N�^	
	void ResetActiveCamera();	// �A�N�e�B�u�J���������Z�b�g

	std::vector<ObjectCamera*> m_pCameraList;	// �J�������X�g
	SceneBase* m_pScene;	// �V�[���N���X�̃|�C���^

	ObjectCamera* m_pActiveCamera;	// �A�N�e�B�u�J����
};

