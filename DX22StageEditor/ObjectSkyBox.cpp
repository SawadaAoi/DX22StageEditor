/* ========================================
	CatRobotGame/
	------------------------------------
	�X�J�C�{�b�N�X�pcpp
	------------------------------------
	ObjectSkyBox.cpp
========================================== */


// =============== �C���N���[�h =====================
#include "ObjectSkyBox.h"
#include "ComponentModelStatic.h"
#include "ModelStaticManager.h"
#include "ComponentTransform.h"

// =============== �萔 ===========================
const Vector3<float> DEFAULT_SCALE = Vector3<float>(150.0f, 150.0f, 150.0f);

// =============== �R���X�g���N�^ =====================
ObjectSkyBox::ObjectSkyBox(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompModelStatic(nullptr)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================= */
void ObjectSkyBox::InitLocal()
{
	m_pCompModelStatic = AddComponent<ComponentModelStatic>();
	m_pCompModelStatic->SetModel(GET_MODEL_STATIC(MODEL_KEY::SKYBOX_SUNNY));

	SetLightUse(false);

	m_pCompTransform->SetLocalScale(DEFAULT_SCALE);
}

/* ========================================
	���[�J���f�[�^�o�͊֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̃��[�J���f�[�^���t�@�C���ɏ�������
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectSkyBox::OutPutLocalData(std::ofstream& file)
{
	S_SaveDataSkyBox data;

	// ���f��ID���擾
	data.nModelId = MODEL_STATIC_MNG_INST.GetModelKey(m_pCompModelStatic->GetModel());

	file.write((char*)&data, sizeof(S_SaveDataSkyBox));
}

/* ========================================
	���[�J���f�[�^���͊֐�
	-------------------------------------
	���e�F�t�@�C������I�u�W�F�N�g�̃��[�J���f�[�^��ǂݍ���
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectSkyBox::InputLocalData(std::ifstream& file)
{
	S_SaveDataSkyBox data;

	file.read((char*)&data, sizeof(S_SaveDataSkyBox));

	m_pCompModelStatic->SetModel(GET_MODEL_STATIC((ModelStaticManager::E_MODEL_KEY)data.nModelId));
}
