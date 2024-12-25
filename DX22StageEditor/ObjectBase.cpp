/* ========================================
	DX22Base/
	------------------------------------
	�I�u�W�F�N�g�pcpp
	------------------------------------
	ObjectBase.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectBase.h"
#include "ComponentBase.h"
#include <windows.h>
#include "ComponentTransform.h"
#include "DebugMenu.h"
#include "SceneManager.h"

// =============== �萔��` =======================
const float DEFAULT_LIGHT_DIFFUSE = 1.0f;	// �f�t�H���g�g�U��
const float DEFAULT_LIGHT_SPECULAR = 0.0f;	// �f�t�H���g���ʌ�
const float DEFAULT_LIGHT_AMBIENT = 0.3f;	// �f�t�H���g����

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[��
=========================================== */
ObjectBase::ObjectBase(SceneBase* pScene)
	: m_pOwnerScene(pScene)				// ���L�V�[����ݒ�
	, m_eState(E_State::STATE_ACTIVE)	// ��Ԃ��A�N�e�B�u�ɐݒ�
	, m_pParentObj(nullptr)				// �e�I�u�W�F�N�g��nullptr�ɐݒ�
	, m_pChildObjs()					// �q�I�u�W�F�N�g�z���������
	, m_pComponents()					// �R���|�[�l���g�z���������
	, m_eTag(E_ObjectTag::None)			// �^�O��None�ɐݒ�
	, m_sName("NoName")					// �I�u�W�F�N�g����NoName�ɐݒ�
	, m_tLightParam{ DEFAULT_LIGHT_DIFFUSE, DEFAULT_LIGHT_SPECULAR, DEFAULT_LIGHT_AMBIENT, true}	// ���C�g�p�����[�^������
{
	// ���L�҃I�u�W�F�N�g��nullptr�̏ꍇ�̓G���[���o��
	if (pScene == nullptr)
	{
		OutputDebugStringA("ObjectBase::ObjectBase() : ���L�V�[����nullptr�ł��B\n");
	}

}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
	-------------------------------------
	�����F�I�u�W�F�N�g��
=========================================== */
void ObjectBase::Init(std::string sName)
{
	m_pComponents.clear();	// �N���A
	SetName(sName);			// �I�u�W�F�N�g����ݒ�
	InitDefaultComponent();	// �f�t�H���g�R���|�[�l���g�ǉ�

	InitLocal();	// �ʏ���������
}

/* ========================================
	�I���֐�
	-------------------------------------
	���e�F�I������
=========================================== */
void ObjectBase::Uninit()
{
	// �����R���|�[�l���g�z��̑S�v�f���폜
	for (auto& pComponent : m_pComponents)
	{
		pComponent->Uninit();
		pComponent.reset();	// ���������

	}
	m_pComponents.clear();	// �N���A

	UninitLocal();	// �ʏI������
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void ObjectBase::Update()
{
	// �����R���|�[�l���g�z��̑S�v�f���X�V
	for (auto& pComponent : m_pComponents)
	{
		if (!pComponent->GetActive()) continue;	// �R���|�[�l���g����A�N�e�B�u�̏ꍇ�͍X�V���Ȃ�
		pComponent->Update();
	}

	UpdateLocal();	// �ʍX�V����

	// �q�I�u�W�F�N�g�̍X�V
	for (auto pChildObj : m_pChildObjs)
	{
		pChildObj->Update();
	}
}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
=========================================== */
void ObjectBase::Draw()
{
	// �����R���|�[�l���g�z��̑S�v�f��`��
	for (auto& pComponent : m_pComponents)
	{
		if (!pComponent->GetActive()) continue;	// �R���|�[�l���g����A�N�e�B�u�̏ꍇ�͍X�V���Ȃ�
		pComponent->Draw();
	}

	DrawLocal();	// �ʕ`�揈��
}


/* ========================================
	�f�t�H���g�R���|�[�l���g�ǉ��֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�Ƀf�t�H���g�Ŏ�������
		�@�R���|�[�l���g��ǉ�����
=========================================== */
void ObjectBase::InitDefaultComponent()
{
	m_pCompTransform = AddComponent<ComponentTransform>();	// Transform�R���|�[�l���g��ǉ�	
}

/* ========================================
	�ՓˊJ�n�����֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�ƏՓ˂����ۂɌĂ΂��
	-------------------------------------
	����1�F�Փ˂����I�u�W�F�N�g
=========================================== */
void ObjectBase::OnCollisionEnter(ObjectBase* pHit)
{
	// �p�����Ċe�I�u�W�F�N�g�ŏ������L�q
}

/* ========================================
	�Փ˒������֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�ƏՓ˂��Ă���ԌĂ΂��
	-------------------------------------
	����1�F�Փ˂��Ă���I�u�W�F�N�g
=========================================== */
void ObjectBase::OnCollisionStay(ObjectBase* pHit)
{
	// �p�����Ċe�I�u�W�F�N�g�ŏ������L�q
}

/* ========================================
	�ՓˏI�������֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�ƏՓ˂��I�������ۂɌĂ΂��
	-------------------------------------
	����1�F�Փ˂��I�������I�u�W�F�N�g
=========================================== */
void ObjectBase::OnCollisionExit(ObjectBase* pHit)
{
	// �p�����Ċe�I�u�W�F�N�g�ŏ������L�q

}

/* ========================================
	�e�I�u�W�F�N�g�ݒ�֐�
	-------------------------------------
	���e�F�e�I�u�W�F�N�g��ݒ肷��
	-------------------------------------
	����1�F�e�ɐݒ肷��I�u�W�F�N�g
=========================================== */
void ObjectBase::SetParentObject(ObjectBase* pParentObj)
{
	// ���ɐݒ肳��Ă���e�I�u�W�F�N�g���玩�g���폜
	if (m_pParentObj)
		m_pParentObj->RemoveChildObject(this);

	m_pParentObj = pParentObj;	// ���I�u�W�F�N�g�̍X�V

	// ���ɐe�I�u�W�F�N�g���X�V�ς݂��`�F�b�N
	// ���q�I�u�W�F�N�g�ǉ��֐�����Ăяo���ꂽ�ꍇ
	for (auto& pChild : m_pParentObj->m_pChildObjs)
	{
		if (pChild == this)	return;
	}

	m_pParentObj->AddChildObject(this);	// �e�I�u�W�F�N�g�̍X�V
}

/* ========================================
	�q�I�u�W�F�N�g�ǉ��֐�
	-------------------------------------
	���e�F�q�I�u�W�F�N�g��ǉ�����
	-------------------------------------
	����1�F�q�I�u�W�F�N�g�ɂ���I�u�W�F�N�g
=========================================== */
void ObjectBase::AddChildObject(ObjectBase* pChildObj)
{
	// ���Ɏq�I�u�W�F�N�g�ɓo�^����Ă���ꍇ�͒ǉ����Ȃ�
	for (auto& pChild : m_pChildObjs)
	{
		if (pChild == pChildObj)	return;
	}
	m_pChildObjs.push_back(pChildObj);	// ���I�u�W�F�N�g�̍X�V

	// ���Ɏq�I�u�W�F�N�g���X�V�ς݂��`�F�b�N
	// ���e�I�u�W�F�N�g�ݒ�֐�����Ăяo���ꂽ�ꍇ
	if (pChildObj->GetParentObject() == this) return;

	pChildObj->SetParentObject(this);	// �q�I�u�W�F�N�g�̍X�V
}

/* ========================================
	�e�I�u�W�F�N�g�����֐�
	-------------------------------------
	���e�F�e�I�u�W�F�N�g����������
=========================================== */
void ObjectBase::RemoveParentObject()
{
	// �e�I�u�W�F�N�g���Ȃ��ꍇ�͏������Ȃ�
	if (m_pParentObj == nullptr) return;

	m_pParentObj->RemoveChildObject(this);	// �e�I�u�W�F�N�g���玩�g���폜
	m_pParentObj = nullptr;					// �e�I�u�W�F�N�g����ɐݒ�
	GetComponent<ComponentTransform>()->ClearParent();	// Transform�R���|�[�l���g�̐e��������

}

/* ========================================
	�q�I�u�W�F�N�g�폜�֐�
	-------------------------------------
	���e�F�q�I�u�W�F�N�g���폜����
	-------------------------------------
	����1�F�폜����q�I�u�W�F�N�g
=========================================== */
void ObjectBase::RemoveChildObject(ObjectBase* pChildObj)
{
	// �q�I�u�W�F�N�g���������Ĕz�񂩂�폜
	m_pChildObjs.erase(
		std::remove(m_pChildObjs.begin(), m_pChildObjs.end(), pChildObj), m_pChildObjs.end());

	pChildObj->m_pParentObj = nullptr;								// �e�I�u�W�F�N�g����ɐݒ�
	pChildObj->GetComponent<ComponentTransform>()->ClearParent();	// Transform�R���|�[�l���g�̐e��������

}

/* ========================================
	�S�q�I�u�W�F�N�g�폜�֐�
	-------------------------------------
	���e�F�S�q�I�u�W�F�N�g���폜����
=========================================== */
void ObjectBase::RemoveAllChildObjects()
{
	// �q�I�u�W�F�N�g��S�Ĕz�񂩂�폜
	for (auto pChild : m_pChildObjs)
	{
		pChild->m_pParentObj = nullptr;								// �e�I�u�W�F�N�g����ɐݒ�
		pChild->GetComponent<ComponentTransform>()->ClearParent();	// Transform�R���|�[�l���g�̐e��������

	}
	m_pChildObjs.clear();

}

/* ========================================
	���㐔�擾�֐�
	-------------------------------------
	���e�F���g�̐��㐔���擾����
	-------------------------------------
	�߂�l�F���㐔
=========================================== */
int ObjectBase::GetGenerationCount()
{
	// �e�I�u�W�F�N�g���Ȃ��ꍇ
	if (m_pParentObj == nullptr)
	{
		return 0;
	}
	else
	{
		return m_pParentObj->GetGenerationCount() + 1;
	}
}

/* ========================================
	�o�̓f�[�^�ʊ֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g���ɏo�͂���f�[�^��ݒ�
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectBase::OutPutLocalData(std::ofstream& file)
{
	// �p�����Ċe�I�u�W�F�N�g�ŏ������L�q
}

/* ========================================
	���̓f�[�^�ʊ֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g���ɓ��͂���f�[�^��ݒ�
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectBase::InputLocalData(std::ifstream& file)
{
	// �p�����Ċe�I�u�W�F�N�g�ŏ������L�q
}

/* ========================================
	�Q�b�^�[(�g�����X�t�H�[��)�֐�
	-------------------------------------
	�ߒl�FComponentTransform* �g�����X�t�H�[���R���|�[�l���g�̃|�C���^
=========================================== */
ComponentTransform* ObjectBase::GetTransform() const
{
	return m_pCompTransform;
}

/* ========================================
	�Q�b�^�[(�����V�[��)�֐�
	-------------------------------------
	�ߒl�F���L�V�[���̃|�C���^
=========================================== */
SceneBase* ObjectBase::GetOwnerScene() const
{
	return m_pOwnerScene;
}

/* ========================================
	�Q�b�^�[(���)�֐�
	-------------------------------------
	�ߒl�F���
=========================================== */
ObjectBase::E_State ObjectBase::GetState() const
{
	return m_eState;
}

/* ========================================
	�Q�b�^�[(�I�u�W�F�N�gID)�֐�
	-------------------------------------
	�ߒl�F�I�u�W�F�N�gID
=========================================== */
size_t ObjectBase::GetTypeID() const
{
	return GetStaticTypeID();
}

/* ========================================
	�Q�b�^�[(�ÓI�I�u�W�F�N�gID)�֐�
	-------------------------------------
	�ߒl�F�ÓI�I�u�W�F�N�gID
=========================================== */
size_t ObjectBase::GetStaticTypeID()
{
	return reinterpret_cast<size_t>(&GetStaticTypeID);
}

/* ========================================
	�Q�b�^�[(�I�u�W�F�N�g�N���X��)�֐�
	-------------------------------------
	�ߒl�F�I�u�W�F�N�g�N���X��
=========================================== */
std::string ObjectBase::GetObjClassName() const
{
	return "ObjectBase";
}

/* ========================================
	�Q�b�^�[(�e�I�u�W�F�N�g)�֐�
	-------------------------------------
	�ߒl�F�e�I�u�W�F�N�g�̃|�C���^
=========================================== */
ObjectBase* ObjectBase::GetParentObject() const
{
	return m_pParentObj;
}


/* ========================================
	�Q�b�^�[(�q�I�u�W�F�N�g)�֐�
	-------------------------------------
	�ߒl�F�q�I�u�W�F�N�g�z��
=========================================== */
std::vector<ObjectBase*> ObjectBase::GetChildObjects() const
{
	return m_pChildObjs;
}

/* ========================================
	�Q�b�^�[(�^�O)�֐�
	-------------------------------------
	�ߒl�F�^�O
=========================================== */
E_ObjectTag ObjectBase::GetTag() const
{
	return m_eTag;
}

/* ========================================
	�Q�b�^�[(�I�u�W�F�N�g��)�֐�
	-------------------------------------
	�ߒl�F�I�u�W�F�N�g��
=========================================== */
std::string ObjectBase::GetName() const
{
	return m_sName;
}

/* ========================================
	�Q�b�^�[(���C�g�p�����[�^)�֐�
	-------------------------------------
	�ߒl�F���C�g�p�����[�^
=========================================== */
ObjectBase::T_LightParam ObjectBase::GetLightMaterial() const
{
	return  m_tLightParam;
}

/* ========================================
	�Z�b�^�[(���)�֐�
	-------------------------------------
	����1�F���
=========================================== */
void ObjectBase::SetState(E_State eState)
{
	m_eState = eState;
}

/* ========================================
	�Z�b�^�[(�^�O)�֐�
	-------------------------------------
	����1�F�^�O
=========================================== */
void ObjectBase::SetTag(E_ObjectTag eTag)
{
	m_eTag = eTag;
}

/* ========================================
	�Z�b�^�[(�I�u�W�F�N�g��)�֐�
	-------------------------------------
	����1�F�I�u�W�F�N�g��
=========================================== */
void ObjectBase::SetName(std::string sName)
{
	m_sName = sName;
}

/* ========================================
	�Z�b�^�[(���C�g�p�����[�^)�֐�
	-------------------------------------
	����1�F�g�U��
	����2�F���ʌ�
	����3�F����
=========================================== */
void ObjectBase::SetLightMaterial(float fDiffuse, float fSpecular, float fAmbient)
{
	m_tLightParam.fDiffuse	= fDiffuse;
	m_tLightParam.fSpecular = fSpecular;
	m_tLightParam.fAmbient	= fAmbient;
}

/* ========================================
	�Z�b�^�[(���C�g�g�p)�֐�
	-------------------------------------
	����1�F���C�g�g�p�t���O
=========================================== */
void ObjectBase::SetLightUse(bool bUse)
{
	m_tLightParam.bLightUse = bUse;
}




#ifdef _DEBUG
/* ========================================
	�f�o�b�O�p�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
		�@��F�I�u�W�F�N�g���(�����R���|�[�l���g��)�̕\��
=========================================== */
void ObjectBase::Debug()
{
	using namespace DebugUI;

	Window& pObjInfo = WIN_OBJ_INFO;

	// �I�u�W�F�N�g�ڍ׏����X�V
	pObjInfo.AddItem(Item::CreateValue("ObjectName", Item::Text, false));	// ���O
	pObjInfo["ObjectName"].SetText(this->GetName().c_str());				// �I�u�W�F�N�g����ݒ�

	Item* pGroupObjectBase = Item::CreateGroup("ObjectBase");	// �I�u�W�F�N�g��{���O���[�v

	// �I�u�W�F�N�g���ύX
	pGroupObjectBase->AddGroupItem(Item::CreateCallBack("ChangeName", Item::Kind::Command, [this](bool isWrite, void* arg)	// ���O�ύX�{�^��
	{
		ChangeName();
	}));
	pGroupObjectBase->AddGroupItem(Item::CreateValue("ObjectReName", Item::Path, false, true));	// �ύX��̖��O

	pGroupObjectBase->AddGroupItem(InitParentList());	// �e�I�u�W�F�N�g���X�g

	pGroupObjectBase->AddGroupItem(Item::CreateBind("UseLight", Item::Bool, &m_tLightParam.bLightUse, false));	// ���C�g�g�p

	pObjInfo.AddItem(pGroupObjectBase);		// �O���[�v��ǉ�

	pObjInfo["ObjectBase"]["ObjectReName"].SetPath(this->GetName().c_str());	// �ύX��̖��O��ݒ�

	// ���[�J���f�o�b�O����
	DebugLocal(pObjInfo);

	// �e�R���|�[�l���g�����I�u�W�F�N�g���E�B���h�E�ɕ\��
	auto it = m_pComponents.begin();
	while (it != m_pComponents.end())
	{
		(*it)->Debug(pObjInfo);
		++it;
	}
}

/* ========================================
	�e�I�u�W�F�N�g���X�g�������֐�
	-------------------------------------
	���e�F�e�I�u�W�F�N�g���X�g�̏�����
	-------------------------------------
	�߂�l�F�e�I�u�W�F�N�g���X�g
=========================================== */
DebugUI::Item* ObjectBase::InitParentList()
{
	using namespace DebugUI;

	// �e�I�u�W�F�N�g���X�g�쐬
	Item* pParentList = Item::CreateList("ParentObject", [this](const void* arg)
	{
		std::string sParentName = reinterpret_cast<const char*>(arg);
		ChangeParentList(sParentName);	// �e�I�u�W�F�N�g�ύX

	}, false, true);

	// �V�[����̃I�u�W�F�N�g�����X�g�ɒǉ�
	int nParentNo = 0;					// �I�𒆂̃I�u�W�F�N�g�ԍ�
	pParentList->AddListItem("None");	// ���X�g�̐擪�͐e�I�u�W�F�N�g�Ȃ�

	// �V�[����̃I�u�W�F�N�g�����X�g�ɒǉ�
	for (const auto pObj : SceneManager::GetScene()->GetAllSceneObjects())
	{
		if (pObj->GetName() == this->GetName()) continue;	// ���g�͒ǉ����Ȃ�

		pParentList->AddListItem(pObj->GetName().c_str());	// �V�[����̃I�u�W�F�N�g����ǉ�
	}
	// �e�I�u�W�F�N�g������ꍇ
	if (m_pParentObj)
	{
		nParentNo = pParentList->GetListNo(m_pParentObj->GetName().c_str());	// �e�I�u�W�F�N�g��I��
	}

	pParentList->SetListNo(nParentNo);	// �I�𒆂̃I�u�W�F�N�g��ݒ�

	pParentList->RemoveListItem(this->GetName().c_str());


	return pParentList;
}

/* ========================================
	���O�ύX�֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g����ύX����
=========================================== */
void ObjectBase::ChangeName()
{
	std::string sReName = WIN_OBJ_INFO["ObjectBase"]["ObjectReName"].GetPath();	// �ύX��̖��O
	std::string sOldName = this->GetName();										// �ύX�O�̖��O

	if (sReName.empty()) return;		// �ύX��̖��O����̏ꍇ�͏������Ȃ�
	if (sReName == sOldName) return;	// �������O�̏ꍇ�͏������Ȃ�

	sReName = SceneManager::GetScene()->CreateUniqueName(sReName);	// �d�����Ȃ����O�ɕύX

	// �I�u�W�F�N�g���ύX
	int listNo = ITEM_OBJ_LIST.GetListNo(this->GetListName().c_str());			// �I�u�W�F�N�g�ꗗ�̕\���ʒu�擾
	ITEM_OBJ_LIST.RemoveListItem(sOldName.c_str(), DebugUI::CHILD_HEAD_TEXT);	// �ύX�O�̖��O�����X�g����폜
	
	this->SetName(sReName);												// �����̖��O�ύX
	ITEM_OBJ_LIST.InsertListItem(this->GetListName().c_str(), listNo);	// �I�u�W�F�N�g�ꗗ�ɕύX��̖��O��ǉ�

	WIN_OBJ_INFO["ObjectName"].SetText(this->GetName().c_str());		// �I�u�W�F�N�g�ڍׂ̖��O��ύX
}


/* ========================================
	�e�I�u�W�F�N�g���X�g�ύX�֐�
	-------------------------------------
	���e�F�e�I�u�W�F�N�g��ύX����
	-------------------------------------
	�����F�V�����e�I�u�W�F�N�g��
=========================================== */
void ObjectBase::ChangeParentList(std::string sParentName)
{
	ObjectBase* pParentNew = SceneManager::GetScene()->FindSceneObject(sParentName);	// �e�I�u�W�F�N�g�擾

	// ���ɐݒ肳��Ă���e�I�u�W�F�N�g�Ɠ����ꍇ�͏������Ȃ�
	if (pParentNew == m_pParentObj) return;

	if (pParentNew)
	{
		this->SetParentObject(pParentNew);	// �V�����e�I�u�W�F�N�g�ɐݒ�
		// �I�u�W�F�N�g�ꗗ�̑I���ʒu��ύX
		int nListNo = ITEM_OBJ_LIST.GetListNo(pParentNew->GetListName().c_str());	// �I�u�W�F�N�g�ꗗ�̕\���ʒu�擾
		ITEM_OBJ_LIST.SetListNo(nListNo + 1);
	}
	else
	{
		this->RemoveParentObject();			// �e�I�u�W�F�N�g���Ȃ��ꍇ(None��I��)�͉���
		ITEM_OBJ_LIST.SetListNo(-1);
	}
}

/* ========================================
	���X�g�\�����擾�֐�
	-------------------------------------
	���e�F���X�g�\���p�̖��O���擾����
	-------------------------------------
	�߂�l�F���X�g�\���p�̖��O
=========================================== */
std::string ObjectBase::GetListName()
{
	std::string sName;							// ���X�g�\����
	int nGeneCnt = this->GetGenerationCount();	// ���㐔�擾
	
	// �e�I�u�W�F�N�g������ꍇ
	if (nGeneCnt > 0)
	{
		// ���㐔���̃X�y�[�X��ǉ�(�\����i�K�I�ɂ��邽��)
		for (int i = 0; i < nGeneCnt; i++)
		{
			sName += DebugUI::CHILD_HEAD_SPACE;
		}
		sName += DebugUI::CHILD_HEAD_TEXT + this->GetName();
	}
	// �����ꍇ�͂��̂܂ܕ\��
	else
	{
		sName = this->GetName();
	}

	return sName;
}


#endif // _DEBUG




