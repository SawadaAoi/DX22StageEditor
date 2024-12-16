/* ========================================
	DX22Base/
	------------------------------------
	�R���|�[�l���g(�ÓI���f��)�pcpp
	------------------------------------
	ComponentModelStatic.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ComponentModelStatic.h"
#include "ComponentTransform.h"
#include "ObjectBase.h"
#include "ModelStatic.h"
#include "ModelStaticManager.h"
#include "CameraManager.h"
#include "LightManager.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	�����F���L�I�u�W�F�N�g
=========================================== */
ComponentModelStatic::ComponentModelStatic(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderModelStatic)
	, m_pModel(nullptr)
	, m_pCompTran(nullptr)
	, m_vPosOffset(0.0f, 0.0f, 0.0f)
	, m_bIsVisible(true)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F�R���|�[�l���g�̏�����
=========================================== */
void ComponentModelStatic::Init()
{
	m_pCompTran = m_pOwnerObj->GetComponent<ComponentTransform>();
	m_pModel = GET_MODEL_STATIC(MODEL_KEY::TEST_MODEL);	// ���f���f�[�^�擾
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F���t���[���X�V����
=========================================== */
void ComponentModelStatic::Draw()
{
	if (!m_pModel) return;
	if (!m_bIsVisible) return;

	// ���[���h�s��쐬
	DirectX::XMFLOAT4X4 matWVP[3];
	matWVP[0] = m_pCompTran->GetWorldMatrixOffset(m_vPosOffset);
	matWVP[1] = CAMERA_MNG_INST.GetActiveCamera()->GetViewMatrix();
	matWVP[2] = CAMERA_MNG_INST.GetActiveCamera()->GetProjectionMatrix();

	m_pModel->SetWVPMatrix(matWVP);	// ���[���h�s����Z�b�g

	ObjectBase::T_LightParam lightParam = m_pOwnerObj->GetLightMaterial();
	m_pModel->SetLightMaterial(lightParam.fDiffuse, lightParam.fSpecular, lightParam.fAmbient, lightParam.bLightUse);	// ���C�g�p�����[�^�ݒ�
	m_pModel->SetLights(LIGHT_MNG_INST.GetLightList());																	// ���C�g�ݒ�
	m_pModel->SetCameraPos(CAMERA_MNG_INST.GetActiveCamera()->GetComponent<ComponentTransform>()->GetWorldPosition());	// �J�����ʒu�ݒ�

	m_pModel->Draw();

}


/* ========================================
	�Q�b�^�[(���f��)�֐�
	-------------------------------------
	�ߒl�FModelStatic*	���f���|�C���^
=========================================== */
ModelStatic* ComponentModelStatic::GetModel()
{
	return m_pModel;
}

/* ========================================
	�Q�b�^�[(�\���t���O)�֐�
	-------------------------------------
	�ߒl�Fbool	�\���t���O
=========================================== */
bool ComponentModelStatic::GetIsVisible()
{
	return m_bIsVisible;
}

/* ========================================
	�Q�b�^�[(�ʒu�I�t�Z�b�g)�֐�
	-------------------------------------
	�ߒl�FVector3<float>	�ʒu�I�t�Z�b�g
=========================================== */
Vector3<float> ComponentModelStatic::GetPosOffset()
{
	return m_vPosOffset;
}

/* ========================================
	�Z�b�^�[(���f��)�֐�
	-------------------------------------
	�����FModelStatic*	pModel	���f���|�C���^
=========================================== */
void ComponentModelStatic::SetModel(ModelStatic* pModel)
{
	m_pModel = pModel;
}

/* ========================================
	�Z�b�^�[(�\���t���O)�֐�
	-------------------------------------
	�����Fbool	bIsVisible	�\���t���O
=========================================== */
void ComponentModelStatic::SetIsVisible(bool bIsVisible)
{
	m_bIsVisible = bIsVisible;
}

/* ========================================
	�Z�b�^�[(�ʒu�I�t�Z�b�g)�֐�
	-------------------------------------
	�����FVector3<float>	offset	�ʒu�I�t�Z�b�g
=========================================== */
void ComponentModelStatic::SetPosOffset(Vector3<float> offset)
{
	m_vPosOffset = offset;
}

#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentModelStatic::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupModel = Item::CreateGroup("ModelStatic");

	pGroupModel->AddGroupItem(Item::CreateBind("IsVisible", Item::Kind::Bool, &m_bIsVisible));
	pGroupModel->AddGroupItem(Item::CreateBind("PosOffset ", Item::Kind::Vector, &m_vPosOffset));

	pGroupModel->AddGroupItem(InitModelList());	// ���f�����X�g

	window.AddItem(pGroupModel);
}

/* ========================================
	���f�����X�g�������֐�
	-------------------------------------
	���e�F���f�����X�g�̏�����
	-------------------------------------
	�߂�l�F���X�g����
=========================================== */
DebugUI::Item* ComponentModelStatic::InitModelList()
{
	using namespace DebugUI;

	DebugUI::Item* pModelList = Item::CreateList("Model", [this](const void* arg)
	{
		// �e�N�X�`���ύX
		ClickModelList(reinterpret_cast<const char*>(arg));

	}, false, true);

	// �o�^����Ă���e�N�X�`�������X�g�ɒǉ�
	std::unordered_map<ModelStaticManager::E_MODEL_KEY, std::string> modelPathMap = MODEL_STATIC_MNG_INST.GetModelPaths();
	for (auto& modelPath : modelPathMap)
	{
		pModelList->AddListItem(modelPath.second.c_str());
	}

	// ���݂̃e�N�X�`����I��
	std::unordered_map<ModelStaticManager::E_MODEL_KEY, std::shared_ptr<ModelStatic>> modelMap = MODEL_STATIC_MNG_INST.GetModelDatas();
	for (auto& model : modelMap)
	{
		if (model.second.get() == m_pModel)
		{
			pModelList->SetListNo(model.first);
			break;
		}
	}

	return pModelList;
}

/* ========================================
	���f�����X�g�N���b�N�֐�
	-------------------------------------
	���e�F���f�����X�g���N���b�N�����ۂ̏���
	-------------------------------------
	�����F���X�g�ɕ\������Ă��镶����
=========================================== */
void ComponentModelStatic::ClickModelList(std::string sListName)
{
	ModelStaticManager::E_MODEL_KEY eKey;

	// ���f���L�[�擾
	std::unordered_map<ModelStaticManager::E_MODEL_KEY, std::string> modelPathMap = MODEL_STATIC_MNG_INST.GetModelPaths();
	for (auto& modelPath : modelPathMap)
	{
		if (modelPath.second == sListName)
		{
			eKey = modelPath.first;
			break;
		}
	}

	// ���f���f�[�^�擾
	std::unordered_map<ModelStaticManager::E_MODEL_KEY, std::shared_ptr<ModelStatic>> modelMap = MODEL_STATIC_MNG_INST.GetModelDatas();
	m_pModel = modelMap.find(eKey)->second.get();

}
#endif // _DEBUG