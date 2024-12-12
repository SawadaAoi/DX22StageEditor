/* ========================================
	DX22Base/
	------------------------------------
	�R���|�[�l���g(�A�j���[�V�������f��)�pcpp
	------------------------------------
	ComponentModelAnime.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ComponentModelAnime.h"
#include "ModelAnime.h"
#include "ComponentTransform.h"
#include "ObjectBase.h"
#include "CameraManager.h"
#include "ModelAnimeManager.h"
#include "LightManager.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�҃I�u�W�F�N�g
=========================================== */
ComponentModelAnime::ComponentModelAnime(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderModelAnim)
	, m_pModel(nullptr)
	, m_pCompTran(nullptr)
	, m_bIsVisible(true)
	, m_vPosOffset(0.0f, 0.0f, 0.0f)
#ifdef _DEBUG
	, m_bDispBone(false)
	, m_bAnimeLoop(false)
	, m_fAnimeSpeed(1.0f)
	, m_nModelKey(ModelAnimeManager::E_ANIME_BASE_KEY::AB_MAX)
	, m_nAnimeKey(0)
	, m_pDelModel(nullptr)
#endif // _DEBUG
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F������
=========================================== */
void ComponentModelAnime::Init()
{
	m_pCompTran = m_pOwnerObj->GetComponent<ComponentTransform>();
	SetModel(GET_MODEL_ANIME(ANIME_BASE_KEY::AB_PLAYER));
	PlayAnime(ANIME_KEY_PLAYER::PLYR_IDLE, true, 1.0f);
}

/* ========================================
	�I���֐�
	-------------------------------------
	���e�F�I������
=========================================== */
void ComponentModelAnime::Uninit()
{
	if (m_pModel)
	{
		SAFE_DELETE(m_pModel);
	}
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void ComponentModelAnime::Update()
{
	if (!m_pModel) return;

	m_pModel->Update(DELTA_TIME);	// �A�j���[�V�����X�V

#ifdef _DEBUG

	if (m_pDelModel)
	{
		SAFE_DELETE(m_pDelModel);
		m_pDelModel = nullptr;
	}
#endif // _DEBUG
}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
=========================================== */
void ComponentModelAnime::Draw()
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
	m_pModel->SetLightMaterial(lightParam.fDiffuse, lightParam.fSpecular, lightParam.fAmbient, true);					// ���C�g�p�����[�^�ݒ�
	m_pModel->SetLights(LIGHT_MNG_INST.GetLightList());																	// ���C�g�ݒ�
	m_pModel->SetCameraPos(CAMERA_MNG_INST.GetActiveCamera()->GetComponent<ComponentTransform>()->GetWorldPosition());	// �J�����ʒu�ݒ�

#ifdef _DEBUG
	if (m_bDispBone)	// �{�[���\��
		m_pModel->DrawBone(m_pCompTran->GetWorldPosition(), m_pCompTran->GetWorldRotation(), m_pCompTran->GetWorldScale());
	else
		m_pModel->Draw();	// �`��
#else
	m_pModel->Draw();	// �`��

#endif // _DEBUG
}

/* ========================================
	�A�j���Đ��֐�
	-------------------------------------
	���e�F�A�j���[�V�����Đ�
	-------------------------------------
	����1�F�A�j���ԍ�
	����2�F���[�v�Đ��t���O
	����3�F�Đ����x
=========================================== */
void ComponentModelAnime::PlayAnime(int animeNo, bool loop, float speed)
{
	if (!m_pModel) return;
	m_pModel->Play(animeNo, loop, speed);

}

/* ========================================
	�A�j���Đ�(�u�����h)�֐�
	-------------------------------------
	���e�F�A�j���[�V�����Đ�(�u�����h)
	-------------------------------------
	����1�F�A�j���ԍ�
	����2�F�u�����h����
	����3�F���[�v�Đ��t���O
	����4�F�Đ����x
=========================================== */
void ComponentModelAnime::PlayAnimeBlend(int animeNo, float blendTime, bool loop, float speed)
{
	if (!m_pModel) return;
	m_pModel->PlayBlend(animeNo, blendTime, loop, speed);
}

/* ========================================
	�A�j���Đ�(����)�֐�
	-------------------------------------
	���e�F�A�j���[�V�����Đ�(����)
	-------------------------------------
	����1�F�A�j���ԍ�1
	����2�F�A�j���ԍ�2
	����3�F������
=========================================== */
void ComponentModelAnime::PlayAnimeParametric(int animeNo1, int animeNo2, float blendRate)
{
	if (!m_pModel) return;
	m_pModel->SetParametric(animeNo1, animeNo2);
	m_pModel->SetParametricBlend(blendRate);
	m_pModel->Play(ModelAnime::PARAMETRIC_ANIME, true);
}

/* ========================================
	�A�j���Đ�(�����E�u�����h)�֐�
	-------------------------------------
	���e�F�A�j���[�V�����Đ�(�����E�u�����h)
	-------------------------------------
	����1�F�A�j���ԍ�1
	����2�F�A�j���ԍ�2
	����3�F������
=========================================== */
void ComponentModelAnime::PlayAnimeParametricBlend(int animeNo1, int animeNo2, float blendRate, float blendTime, bool loop, float speed)
{
	if (!m_pModel) return;
	m_pModel->SetParametric(animeNo1, animeNo2);
	m_pModel->SetParametricBlend(blendRate);
	m_pModel->PlayBlend(ModelAnime::PARAMETRIC_ANIME, blendTime, loop, speed);
}

/* ========================================
	�Q�b�^�[(���f��)�֐�
	-------------------------------------
	�ߒl�FModelAnime*	���f���|�C���^
=========================================== */
ModelAnime* ComponentModelAnime::GetModel()
{
	return m_pModel;
}

/* ========================================
	�Q�b�^�[(�\���t���O)�֐�
	-------------------------------------
	�ߒl�Fbool	�\���t���O
=========================================== */
bool ComponentModelAnime::GetIsVisible()
{
	return m_bIsVisible;
}

/* ========================================
	�Q�b�^�[(�ʒu�I�t�Z�b�g)�֐�
	-------------------------------------
	�ߒl�FVector3<float>	�ʒu�I�t�Z�b�g
=========================================== */
Vector3<float> ComponentModelAnime::GetPosOffset()
{
	return m_vPosOffset;
}

/* ========================================
	�Z�b�^�[(���f��)�֐�
	-------------------------------------
	�����FModelAnime*	pModel	���f���|�C���^
=========================================== */
void ComponentModelAnime::SetModel(ModelAnime* pModel)
{
	m_pModel = pModel;
}

/* ========================================
	�Z�b�^�[(�\���t���O)�֐�
	-------------------------------------
	�����Fbool	bIsVisible	�\���t���O
=========================================== */
void ComponentModelAnime::SetIsVisible(bool bIsVisible)
{
	m_bIsVisible = bIsVisible;
}

/* ========================================
	�Z�b�^�[(�ʒu�I�t�Z�b�g)�֐�
	-------------------------------------
	�����FVector3<float>	offset	�ʒu�I�t�Z�b�g
=========================================== */
void ComponentModelAnime::SetPosOffset(Vector3<float> offset)
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
void ComponentModelAnime::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupModel = Item::CreateGroup("ModelAnime");

	pGroupModel->AddGroupItem(Item::CreateBind("IsVisible ", Item::Kind::Bool, &m_bIsVisible));
	pGroupModel->AddGroupItem(Item::CreateBind("PosOffset  ", Item::Kind::Vector, &m_vPosOffset));
	pGroupModel->AddGroupItem(Item::CreateBind("DispBone ", Item::Kind::Bool, &m_bDispBone));

	pGroupModel->AddGroupItem(InitModelList());	// ���f�����X�g


	pGroupModel->AddGroupItem(Item::CreateBind("Loop", Item::Kind::Bool, &m_bAnimeLoop));
	pGroupModel->AddGroupItem(Item::CreateBind("Speed", Item::Kind::Float, &m_fAnimeSpeed));
	pGroupModel->AddGroupItem(Item::CreateList("Animation", [this](const void* arg)
	{
		ClickAnimeList(reinterpret_cast<const char*>(arg));
	}, false, true));	// �A�j�����X�g

	window.AddItem(pGroupModel);

	InitAnimeList();

}

/* ========================================
	���f�����X�g�������֐�
	-------------------------------------
	���e�F���f�����X�g�̏�����
	-------------------------------------
	�߂�l�F���f�����X�g
=========================================== */
DebugUI::Item* ComponentModelAnime::InitModelList()
{
	using namespace DebugUI;

	// ���f�����X�g�쐬
	DebugUI::Item* pModelList = Item::CreateList("Model", [this](const void* arg)
	{
		// ���X�g�N���b�N���̏���
		ClickModelList(reinterpret_cast<const char*>(arg));

	}, false, true);

	// �o�^����Ă��郂�f�������X�g�ɒǉ�
	std::unordered_map<ModelAnimeManager::E_ANIME_BASE_KEY, std::string> modelPathMap = MODEL_ANIME_MNG_INST.GetModelPaths();
	for (auto& modelPath : modelPathMap)
	{
		pModelList->AddListItem(modelPath.second.c_str());
	}

	// ���݂̃��f����I��
	std::unordered_map<ModelAnimeManager::E_ANIME_BASE_KEY, std::shared_ptr<ModelAnime>> modelMap = MODEL_ANIME_MNG_INST.GetModelDatas();
	for (auto& model : modelMap)
	{
		// ���f��������v������
		if (model.second->GetModelName() == m_pModel->GetModelName())
		{
			pModelList->SetListNo(model.first);
			m_nModelKey = model.first;
			break;
		}
	}


	return pModelList;
}

/* ========================================
	���f�����X�g�N���b�N�֐�
	-------------------------------------
	���e�F���f�����X�g�N���b�N���̏���
	-------------------------------------
	�����F���X�g�ɕ\������Ă��镶����
=========================================== */
void ComponentModelAnime::ClickModelList(std::string sListName)
{
	ModelAnimeManager::E_ANIME_BASE_KEY eKey = ModelAnimeManager::E_ANIME_BASE_KEY::AB_MAX;

	// �O�̃��f�����폜�p�ϐ��ɃZ�b�g(�s�����̂��ߌ�ō폜)
	m_pDelModel = m_pModel;

	// ���f���L�[�擾
	std::unordered_map<ModelAnimeManager::E_ANIME_BASE_KEY, std::string> modelPathMap = MODEL_ANIME_MNG_INST.GetModelPaths();
	for (auto& modelPath : modelPathMap)
	{
		if (modelPath.second == sListName)
		{
			eKey = modelPath.first;
			break;
		}
	}
	// ���f���f�[�^�擾
	m_pModel = GET_MODEL_ANIME(eKey);
	m_nModelKey = eKey;

	m_pModel->Play(0, m_bAnimeLoop, m_fAnimeSpeed);	// �擪�̃A�j���[�V�����Đ�
	InitAnimeList();	// �A�j�����X�g�ēǂݍ���
}

/* ========================================
	�A�j�����X�g�������֐�
	-------------------------------------
	���e�F�A�j�����X�g�̏�����
=========================================== */
void ComponentModelAnime::InitAnimeList()
{
	using namespace DebugUI;

	DebugUI::Item* m_pAnimeList = &WIN_OBJ_INFO["ModelAnime"]["Animation"];		// �A�j�����X�g�擾
	reinterpret_cast<DebugUI::ItemList*>(m_pAnimeList)->RemoveListItemAll();	// ���X�g�N���A

	// ���݂̃A�j���[�V�������擾
	std::string target = m_pModel->GetAnimation(m_pModel->GetPlayNo())->sName;	
	// ���X�g�I��ԍ�
	int nSelect = 0;		

	// �o�^����Ă���A�j���[�V���������X�g�ɒǉ�
	std::unordered_map <int, std::string> modelPathMap = MODEL_ANIME_MNG_INST.GetAnimePaths(m_nModelKey);
	for (auto& modelPath : modelPathMap)
	{
		m_pAnimeList->AddListItem(modelPath.second.c_str());

		// �\�����̂Ɋ܂܂�Ă��邩
		if (modelPath.second.find(target) != std::string::npos) 
			nSelect = modelPath.first;
	}

	// ���݂̃��f����I��
	m_pAnimeList->SetListNo(nSelect);
	m_nAnimeKey = -1;

}

/* ========================================
	�A�j�����X�g�N���b�N�֐�
	-------------------------------------
	���e�F�A�j�����X�g�N���b�N���̏���
	-------------------------------------
	�����F���X�g�ɕ\������Ă��镶����
=========================================== */
void ComponentModelAnime::ClickAnimeList(std::string sListName)
{
	// ���X�g������A�j���[�V�����L�[�擾
	std::unordered_map <int, std::string> modelPathMap = MODEL_ANIME_MNG_INST.GetAnimePaths(m_nModelKey);
	for (auto& modelPath : modelPathMap)
	{
		if(sListName == modelPath.second)
		{
			m_pModel->Play(modelPath.first, m_bAnimeLoop, m_fAnimeSpeed);
			m_nAnimeKey = modelPath.first;
			break;
		}
	}
}
#endif // _DEBUG
