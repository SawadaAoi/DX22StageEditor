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

// �I�u�W�F�N�g�t�H�[�J�X�Ɏg�p
#include "ObjectBase.h"
#include "ComponentTransform.h"
#include "ObjectCameraDebug.h"

// =============== �萔��` =======================
const std::string DEFAULT_CAMERA_NAME	= "DefaultCamera";	// �f�t�H���g�J������
const Vector3<float> FOCUS_OFFSET		= Vector3<float>(0.0f, 2.0f, -5.0f);	// �t�H�[�J�X���̃J�����ʒu


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
=========================================== */
CameraManager::CameraManager()
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
CameraManager& CameraManager::GetInstance()
{
	static CameraManager instance;
	return instance;
}



/* ========================================
	�J�����V�[���������֐�
	-------------------------------------
	���e�F�V�[���J�n���ɃJ�������Ď擾
	-------------------------------------
	����1�F�V�[���N���X�̃|�C���^
=========================================== */
void CameraManager::Init(SceneBase* pScene)
{
	m_pScene = pScene;	// �V�[���N���X�̃|�C���^���擾

	// �J�����ꗗ���N���A
	m_pCameraList.clear();	

	// �V�[������J�������X�g���擾
	for (ObjectBase* pObj : m_pScene->GetSceneObjectsTag(E_ObjectTag::Camera))
	{
		AddCamera(static_cast<ObjectCamera*>(pObj));
	}

	// �J���������݂���ꍇ
	if (m_pCameraList.size() > 0)
	{
		SwitchCamera(0);												// �A�N�e�B�u�ɂ���
	}
	// �J���������݂��Ȃ��ꍇ
	else
	{
		m_pScene->AddSceneObject<ObjectCameraDebug>(DEFAULT_CAMERA_NAME);	// �J�����ǉ�
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
void CameraManager::SwitchCamera(int num)
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
	WIN_CAMERA_INFO["Active"].SetText(m_pActiveCamera->GetName().c_str());

#endif // _DEBUG
}

/* ========================================
	�J�����؂�ւ��֐�
	-------------------------------------
	���e�F�J�����̐؂�ւ�
	-------------------------------------
	����1�F�J�����̃|�C���^
=========================================== */
void CameraManager::SwitchCamera(ObjectCamera* pCamera)
{
	if (pCamera == nullptr) return;

	// �J�������X�g��S�Ĕ�A�N�e�B�u�ɂ���
	ResetActiveCamera();

	pCamera->SetActive(true);		// �w��̃J�������A�N�e�B�u�ɂ���
	m_pActiveCamera = pCamera;	// �A�N�e�B�u�J������ݒ�

#ifdef _DEBUG
	// �I�u�W�F�N�g�ꗗ�ɒǉ�
	WIN_CAMERA_INFO["Active"].SetText(m_pActiveCamera->GetName().c_str());

#endif // _DEBUG
}

/* ========================================
	�J�����ǉ��֐�
	-------------------------------------
	���e�F�J�������ꗗ�ɒǉ�
	-------------------------------------
	�����F�J�����̃|�C���^
=========================================== */
void CameraManager::AddCamera(ObjectCamera* pCamera)
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
void CameraManager::RemoveCamera(ObjectCamera* pCamera)
{
	// �J�������X�g����폜
	m_pCameraList.erase(std::remove(m_pCameraList.begin(), m_pCameraList.end(), pCamera), m_pCameraList.end());
#ifdef _DEBUG
	// �J�������X�g����폜
	WIN_CAMERA_INFO["CameraList"].RemoveListItem(pCamera->GetName().c_str());

#endif // _DEBUG
}

/* ========================================
	�J�����t�H�[�J�X�ړ��֐�
	-------------------------------------
	���e�F�J�������w��I�u�W�F�N�g�Ƀt�H�[�J�X�ړ�
	-------------------------------------
	�����F�I�u�W�F�N�g�̃|�C���^
=========================================== */
void CameraManager::FocusMoveCamera(ObjectBase* pObj)
{
	ComponentTransform* pCameraTrans = GetActiveCamera()->GetComponent<ComponentTransform>();	// �J�����̃g�����X�t�H�[���擾
	ComponentTransform* pTargetTrans = pObj->GetComponent<ComponentTransform>();				// �^�[�Q�b�g�̃g�����X�t�H�[���擾

	// �g�����X�t�H�[�����擾�ł��Ȃ��ꍇ�͏������Ȃ�
	if (pCameraTrans == nullptr || pTargetTrans == nullptr) return;

	Vector3<float> vTargetPos = pTargetTrans->GetPosition();
	vTargetPos += FOCUS_OFFSET;	// �΂ߌ�납�猩���낷�悤�Ɉʒu�𒲐�

	// �J�����̈ʒu�ƌ�����ݒ�
	pCameraTrans->SetPosition(vTargetPos);
	pCameraTrans->LookAt(pTargetTrans->GetPosition());

}

/* ========================================
	�A�N�e�B�u�J�������Z�b�g�֐�
	-------------------------------------
	���e�F�A�N�e�B�u�J���������Z�b�g
=========================================== */
void CameraManager::ResetActiveCamera()
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
ObjectCamera* CameraManager::GetActiveCamera()
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
int CameraManager::GetCameraNum()
{
	return m_pCameraList.size();
}
