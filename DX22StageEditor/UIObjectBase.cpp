/* ========================================
	DX22Base/
	------------------------------------
	UI�I�u�W�F�N�g���pcpp
	------------------------------------
	UIObjectBase.cpp
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "UIObjectBase.h"
#include "SceneBase.h"			// �V�[�����N���X
#include "UIComponentBase.h"	// UI�R���|�[�l���g���N���X
#include <windows.h>
#include "UIComponentTransform.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[��
=========================================== */
UIObjectBase::UIObjectBase(SceneBase* pScene)
	: m_pOwnerScene(pScene)
	, m_pComponents()
	, m_sName("NoName")
	, m_pParentObj(nullptr)
	, m_pChildObjs()
{
	// ���L�V�[����nullptr�̏ꍇ�̓G���[���o��
	if (pScene == nullptr)
	{
		OutputDebugStringA("UIObjectBase::UIObjectBase() : ���L�V�[����nullptr�ł��B\n");
	}

}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
	-------------------------------------
	�����F�I�u�W�F�N�g��
=========================================== */
void UIObjectBase::Init(std::string sName)
{
	m_pComponents.clear();	// �R���|�[�l���g���X�g���N���A
	SetName(sName);			// �I�u�W�F�N�g����ݒ�
	InitDefaultComponent();	// �f�t�H���g�R���|�[�l���g��ǉ�

	InitLocal();			// �ʏ���������
}


/* ========================================
	�I���֐�
	-------------------------------------
	���e�F�I������
=========================================== */
void UIObjectBase::Uninit()
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
void UIObjectBase::Update()
{
	// �����R���|�[�l���g�z��̑S�v�f���X�V
	for (auto& pComponent : m_pComponents)
	{
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
void UIObjectBase::Draw()
{
	// �����R���|�[�l���g�z��̑S�v�f��`��
	for (auto& pComponent : m_pComponents)
	{
		pComponent->Draw();
	}
	DrawLocal();	// �ʕ`�揈��

}

/* ========================================
	�e�I�u�W�F�N�g�ݒ�֐�
	-------------------------------------
	���e�F�e�I�u�W�F�N�g��ݒ肷��
	-------------------------------------
	����1�F�e�ɐݒ肷��I�u�W�F�N�g
=========================================== */
void UIObjectBase::SetParentObject(UIObjectBase* pParentObj)
{
	// ���ɐe�I�u�W�F�N�g���ݒ肳��Ă���ꍇ
	if (m_pParentObj)
	{
		// �e�I�u�W�F�N�g���玩�g���폜
		m_pParentObj->RemoveChildObject(this);
	}

#ifdef _DEBUG

	// �폜����I�u�W�F�N�g�������X�g����폜
	ITEM_UI_LIST.RemoveListItem(this->GetName().c_str(), DebugUI::CHILD_HEAD_TEXT);

	m_pParentObj = pParentObj;	// ���I�u�W�F�N�g�̍X�V

	// �e�I�u�W�F�N�g�̃��X�g�ԍ��擾
	int nParentNum = ITEM_UI_LIST.GetListNo(m_pParentObj->GetListName().c_str()) + 1;

	// �I�u�W�F�N�g�ꗗ���X�g�Őe�I�u�W�F�N�g�̉��ɒǉ�
	ITEM_UI_LIST.InsertListItem(this->GetListName().c_str(), nParentNum);

	// �q�I�u�W�F�N�g�������Ă���ꍇ
	for (auto& pChild : m_pChildObjs)
	{
		// �q�I�u�W�F�N�g�̐e���X�V(���X�g�̕\�����X�V���邽��)
		pChild->SetParentObject(this);
	}
#else
	m_pParentObj = pParentObj;	// ���I�u�W�F�N�g�̍X�V

#endif // _DEBUG

	// �o�^�����e�I�u�W�F�N�g�̎q�I�u�W�F�N�g���X�g���m�F
	for (auto& pChild : m_pParentObj->m_pChildObjs)
	{
		// ���ɐe�I�u�W�F�N�g�Ɏ��g���o�^����Ă���ꍇ�͏������Ȃ�
		if (pChild == this)
		{
			return;
		}
	}

	m_pParentObj->AddChildObject(this);	// �e�I�u�W�F�N�g�Ɏ��g��o�^
}

/* ========================================
	�q�I�u�W�F�N�g�ǉ��֐�
	-------------------------------------
	���e�F�q�I�u�W�F�N�g��ǉ�����
	-------------------------------------
	����1�F�q�I�u�W�F�N�g�ɂ���I�u�W�F�N�g
=========================================== */
void UIObjectBase::AddChildObject(UIObjectBase* pChildObj)
{
	// ���ɓo�^����Ă��邩�m�F
	for (auto& pChild : m_pChildObjs)
	{
		if (pChild == pChildObj)
		{
			return;
		}
	}

	m_pChildObjs.push_back(pChildObj);	// �q�I�u�W�F�N�g���X�g�ɒǉ�

	// ���Ɏq�I�u�W�F�N�g���X�V�ς݂��`�F�b�N
	// ���e�I�u�W�F�N�g�ݒ�֐�����Ăяo���ꂽ�ꍇ
	if (pChildObj->GetParentObject() != this)
	{
		pChildObj->SetParentObject(this);	// �q�I�u�W�F�N�g�̍X�V
	}

}

/* ========================================
	�e�I�u�W�F�N�g�����֐�
	-------------------------------------
	���e�F�e�I�u�W�F�N�g����������
=========================================== */
void UIObjectBase::RemoveParentObject()
{
	// �e�I�u�W�F�N�g���Ȃ��ꍇ�͏������Ȃ�
	if (m_pParentObj == nullptr) return;

	m_pParentObj->RemoveChildObject(this);	// �e�I�u�W�F�N�g���玩�g���폜
	m_pParentObj = nullptr;					// �e�I�u�W�F�N�g����ɐݒ�

	// ���g��Transform�R���|�[�l���g�̐e��������
	this->GetComponent<UIComponentTransform>()->ClearParent();	

#ifdef _DEBUG

	ITEM_UI_LIST.RemoveListItem(this->GetName().c_str(), DebugUI::CHILD_HEAD_TEXT);

	// �I�u�W�F�N�g�ꗗ�̈�ԉ��ɒǉ�
	ITEM_UI_LIST.AddListItem(this->GetListName().c_str());

#endif
}

/* ========================================
	�q�I�u�W�F�N�g�폜�֐�
	-------------------------------------
	���e�F�q�I�u�W�F�N�g���폜����
	-------------------------------------
	����1�F�폜����q�I�u�W�F�N�g
=========================================== */
void UIObjectBase::RemoveChildObject(UIObjectBase* pChildObj)
{
	// �q�I�u�W�F�N�g���������Ĕz�񂩂�폜
	m_pChildObjs.erase(
		std::remove(m_pChildObjs.begin(), m_pChildObjs.end(), pChildObj), m_pChildObjs.end());


	pChildObj->m_pParentObj = nullptr;								// �e�I�u�W�F�N�g����ɐݒ�
	pChildObj->GetComponent<UIComponentTransform>()->ClearParent();	// Transform�R���|�[�l���g�̐e��������

#ifdef _DEBUG

	ITEM_UI_LIST.RemoveListItem(pChildObj->GetName().c_str(), DebugUI::CHILD_HEAD_TEXT);

	// �I�u�W�F�N�g�ꗗ�̈�ԉ��ɒǉ�
	ITEM_UI_LIST.AddListItem(pChildObj->GetListName().c_str());

#endif
}

/* ========================================
	�S�q�I�u�W�F�N�g�폜�֐�
	-------------------------------------
	���e�F�S�q�I�u�W�F�N�g���폜����
=========================================== */
void UIObjectBase::RemoveAllChildObjects()
{
	// �q�I�u�W�F�N�g��S�Ĕz�񂩂�폜
	for (auto pChild : m_pChildObjs)
	{
		pChild->m_pParentObj = nullptr;									// �e�I�u�W�F�N�g����ɐݒ�
		pChild->GetComponent<UIComponentTransform>()->ClearParent();	// Transform�R���|�[�l���g�̐e��������

#ifdef _DEBUG

		ITEM_UI_LIST.RemoveListItem(pChild->GetName().c_str(), DebugUI::CHILD_HEAD_TEXT);

		// �I�u�W�F�N�g�ꗗ�̈�ԉ��ɒǉ�
		ITEM_UI_LIST.AddListItem(pChild->GetListName().c_str());

#endif
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
int UIObjectBase::GetGenerationCount()
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
	�f�t�H���g�R���|�[�l���g�ǉ��֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�Ƀf�t�H���g�Ŏ�������
		�@�R���|�[�l���g��ǉ�����
=========================================== */
void UIObjectBase::InitDefaultComponent()
{
	AddComponent<UIComponentTransform>();	// Transform�R���|�[�l���g��ǉ�	
}

/* ========================================
	�Q�b�^�[(�����V�[��)�֐�
	-------------------------------------
	�ߒl�F���L�V�[���̃|�C���^
=========================================== */
SceneBase* UIObjectBase::GetOwnerScene() const
{
	return m_pOwnerScene;
}

/* ========================================
	�Q�b�^�[(�I�u�W�F�N�gID)�֐�
	-------------------------------------
	�ߒl�F�I�u�W�F�N�gID
=========================================== */
size_t UIObjectBase::GetTypeID() const
{
	return GetStaticTypeID();
}

/* ========================================
	�Q�b�^�[(�ÓI�I�u�W�F�N�gID)�֐�
	-------------------------------------
	�ߒl�F�ÓI�I�u�W�F�N�gID
=========================================== */
size_t UIObjectBase::GetStaticTypeID()
{
	static size_t nID = reinterpret_cast<size_t>(&GetStaticTypeID); 
	return nID;													
}

/* ========================================
	�Q�b�^�[(�I�u�W�F�N�g��)�֐�
	-------------------------------------
	�ߒl�F�I�u�W�F�N�g��
=========================================== */
std::string UIObjectBase::GetName() const
{
	return m_sName;
}

/* ========================================
	�Q�b�^�[(�e�I�u�W�F�N�g)�֐�
	-------------------------------------
	�ߒl�F�e�I�u�W�F�N�g
=========================================== */
UIObjectBase* UIObjectBase::GetParentObject() const
{
	return m_pParentObj;
}

/* ========================================
	�Q�b�^�[(�q�I�u�W�F�N�g)�֐�
	-------------------------------------
	�ߒl�F�q�I�u�W�F�N�g
=========================================== */
std::vector<UIObjectBase*> UIObjectBase::GetChildObjects() const
{
	return m_pChildObjs;
}

/* ========================================
	�Z�b�^�[(�I�u�W�F�N�g��)�֐�
	-------------------------------------
	����1�F�I�u�W�F�N�g��
=========================================== */
void UIObjectBase::SetName(std::string sName)
{
	m_sName = sName;
}

#ifdef _DEBUG
/* ========================================
	�f�o�b�O�p�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
		�@��FUI���(�����R���|�[�l���g��)�̕\��
=========================================== */
void UIObjectBase::Debug()
{
	DebugUI::Window& pObjInfo = WIN_UI_INFO;

	// �I�u�W�F�N�g�ڍ׏����X�V
	pObjInfo.AddItem(DebugUI::Item::CreateValue("UIName", DebugUI::Item::Text, false));	// ���O
	pObjInfo["UIName"].SetText(this->GetName().c_str());							// �I�u�W�F�N�g����ݒ�

	// �e�R���|�[�l���g�����I�u�W�F�N�g���E�B���h�E�ɕ\��
	auto it = m_pComponents.begin();
	while (it != m_pComponents.end())
	{
		(*it)->Debug(pObjInfo);
		++it;
	}
}

/* ========================================
	���X�g�\�����擾�֐�
	-------------------------------------
	���e�F���X�g�\���p�̖��O���擾����
	-------------------------------------
	�߂�l�F���X�g�\���p�̖��O
=========================================== */
std::string UIObjectBase::GetListName()
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