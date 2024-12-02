/* ========================================
	DX22Base/
	------------------------------------
	�J�����Ǘ��pcpp
	------------------------------------
	CameraManager.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "CameraManager.h"	
#include "SceneBase.h"		// �V�[�����N���X
#include "DebugMenu.h"

// =============== �萔��` =======================
const std::string DEFAULT_CAMERA_NAME = "DefaultCamera";	// �f�t�H���g�J������

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F����
=========================================== */
CCameraManager::CCameraManager()
	: m_pScene(nullptr)	// �V�[���N���X�̃|�C���^��������
	, m_pCameraList()	// �J�������X�g��������
	, m_pActiveCamera(nullptr)	// �A�N�e�B�u�J������������
{
}

/* ========================================
	�C���X�^���X(�V���O���g��)�擾�֐�
	-------------------------------------
	���e�F���N���X�̃C���X�^���X���擾
	-------------------------------------
	�ߒl�F���N���X�̃C���X�^���X
=========================================== */
CCameraManager& CCameraManager::GetInstance()
{
	static CCameraManager instance;
	return instance;
}



/* ========================================
	�J�����V�[���������֐�
	-------------------------------------
	���e�F�V�[���J�n���ɃJ�������Ď擾
	-------------------------------------
	����1�F�V�[���N���X�̃|�C���^
=========================================== */
void CCameraManager::Init(SceneBase* pScene)
{
	m_pScene = pScene;	// �V�[���N���X�̃|�C���^���擾

	// �J�����ꗗ���N���A
	m_pCameraList.clear();

	// �V�[������J�������X�g���擾
	m_pCameraList = m_pScene->GetSceneObjects<ObjectCamera>();

	// �J���������݂���ꍇ
	if (m_pCameraList.size() > 0)
	{
		SwitchCamera(0);												// �A�N�e�B�u�ɂ���
	}
	// �J���������݂��Ȃ��ꍇ
	else
	{
		m_pScene->AddSceneObject<ObjectCamera>(DEFAULT_CAMERA_NAME);				// �J�����ǉ�
		SwitchCamera(0);												// �A�N�e�B�u�ɂ���
	}
}


/* ========================================
	�J�����؂�ւ��֐�
	-------------------------------------
	���e�F�J�����̐؂�ւ�
	-------------------------------------
	����1�F�J�����ԍ�(0�`)
=========================================== */
void CCameraManager::SwitchCamera(int num)
{
	// �J�������X�g�̃T�C�Y���w�萔��菬�����ꍇ
	// �J�����z��ԍ�(0�`)�A�J�����ԍ�(1�`)�̂��߁A�T�C�Y���傫���ꍇ�͏������Ȃ�
	if (num >= m_pCameraList.size()) return;

	// �J�������X�g��S�Ĕ�A�N�e�B�u�ɂ���
	ResetActiveCamera();

	m_pCameraList.at(num)->SetActive(true);		// �w��ԍ��̃J�������A�N�e�B�u�ɂ���
	m_pActiveCamera = m_pCameraList.at(num);	// �A�N�e�B�u�J������ݒ�

#ifdef _DEBUG
	// �I�u�W�F�N�g�ꗗ�ɒǉ�
	WIN_CAMERA_INFO["ActiveCamera"].SetText(m_pActiveCamera->GetName().c_str());

#endif // _DEBUG
}

/* ========================================
	�J�����ǉ��֐�
	-------------------------------------
	���e�F�J�������ꗗ�ɒǉ�
	-------------------------------------
	�����F�J�����̃|�C���^
=========================================== */
void CCameraManager::AddCamera(ObjectCamera* pCamera)
{
	m_pCameraList.push_back(pCamera);
#ifdef _DEBUG
	// �I�u�W�F�N�g�ꗗ�ɒǉ�
	WIN_CAMERA_INFO["CameraList"].AddListItem(pCamera->GetName().c_str());

#endif // _DEBUG

}

/* ========================================
	�J�����폜�֐�
	-------------------------------------
	���e�F�J�������ꗗ����폜
	-------------------------------------
	�����F�J�����̃|�C���^
=========================================== */
void CCameraManager::RemoveCamera(ObjectCamera* pCamera)
{
	// �J�������X�g����폜
	m_pCameraList.erase(std::remove(m_pCameraList.begin(), m_pCameraList.end(), pCamera), m_pCameraList.end());
#ifdef _DEBUG
	if (pCamera == nullptr) return;
	// �I�u�W�F�N�g�ꗗ�ɒǉ�
	WIN_CAMERA_INFO["CameraList"].RemoveListItem(pCamera->GetName().c_str());

#endif // _DEBUG
}

/* ========================================
	�A�N�e�B�u�J�������Z�b�g�֐�
	-------------------------------------
	���e�F�A�N�e�B�u�J���������Z�b�g
=========================================== */
void CCameraManager::ResetActiveCamera()
{
	// �J�������X�g��S�Ĕ�A�N�e�B�u�ɂ���
	for (auto pCamera : m_pCameraList)
	{
		pCamera->SetActive(false);
	}
}


/* ========================================
	�L���J�����擾�֐�
	-------------------------------------
	���e�F���ݗL���ȃJ�����̎擾
	-------------------------------------
	�ߒl�FObjectCamera*		�L���J�����̃|�C���^
============================================ */
ObjectCamera* CCameraManager::GetActiveCamera()
{
	return m_pActiveCamera;
}

/* ========================================
	�J�������擾�֐�
	-------------------------------------
	���e�F�V�[����̃J���������擾
	-------------------------------------
	�ߒl�Fint		�J������
============================================ */
int CCameraManager::GetCameraNum()
{
	return m_pCameraList.size();
}