/* ========================================
	DX22/Dropin
	------------------------------------
	���f���f�[�^�Ǘ��pcpp
	------------------------------------
	ModelDataManager.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ModelStaticManager.h"


// =============== �萔��` =======================
// ���f���f�[�^�ۑ���p�X


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
========================================== */
ModelStaticManager::ModelStaticManager()
	: bIsInit(false)
{
	m_apModelDatas.clear();
}


/* ========================================
	�C���X�^���X�擾�֐�
	-------------------------------------
	���e�F�C���X�^���X���擾����
	-------------------------------------
	�ߒl�F�C���X�^���X
========================================== */
ModelStaticManager& ModelStaticManager::GetInstance()
{
	static ModelStaticManager instance;
	return instance;
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F���f���f�[�^��ǂݍ���
========================================== */
void ModelStaticManager::Init()
{
	// ���ɏ���������Ă�����I��
	if (bIsInit) return;

	// ���f���f�[�^��ǂݍ���
	for (std::pair<E_MODEL_KEY, std::string> PathMap : MODEL_FBX_PATH)
	{
		E_MODEL_KEY eNowKey = PathMap.first;	// ���݂̃L�[
		std::string sNowPath = PathMap.second;	// ���݂̕ۑ��ʒu�p�X

		ModelStatic* pModel = new ModelStatic();
		if (!pModel->Load(sNowPath.c_str(), MODEL_FBX_SCALE.at(eNowKey), ModelStatic::E_Flip::ZFlip))	// 3D���f���ǂݍ���
		{
			MessageBox(NULL, sNowPath.c_str(), "Error", MB_OK);	//�G���[���b�Z�[�W�̕\��
		}

		m_apModelDatas.insert(std::make_pair(eNowKey, pModel));	// ���f���f�[�^�z��ɒǉ�
	}

	bIsInit = true;
}


/* ========================================
	���f���f�[�^�擾�֐�
	-------------------------------------
	���e�F�L�[�����ɊY���̃��f���f�[�^��n��
	-------------------------------------
	����1�FE_MODEL_KEY	�擾���������f���f�[�^�̃L�[
	-------------------------------------
	�ߒl�FModelStatic*	���f���f�[�^
=========================================== */
ModelStatic* ModelStaticManager::GetModelData(E_MODEL_KEY e_GetKey)
{
	return m_apModelDatas.at(e_GetKey).get();
}

/* ========================================
	�Q�b�^�[(�ÓI���f���z��)�֐�
	-------------------------------------
	�ߒl�Fstd::unordered_map<E_MODEL_KEY, std::shared_ptr<ModelStatic>>	���f���f�[�^�z��
=========================================== */
std::unordered_map<ModelStaticManager::E_MODEL_KEY, std::shared_ptr<ModelStatic>> ModelStaticManager::GetModelDatas()
{
	return m_apModelDatas;
}

/* ========================================
	�Q�b�^�[(���f���f�[�^�p�X)�֐�
	-------------------------------------
	�ߒl�Fstd::unordered_map<E_MODEL_KEY, std::string>	���f���f�[�^�p�X
=========================================== */
std::unordered_map<ModelStaticManager::E_MODEL_KEY, std::string> ModelStaticManager::GetModelPaths()
{
	return MODEL_FBX_PATH;
}
