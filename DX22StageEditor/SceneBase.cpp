/* ========================================
	DX22Base/
	------------------------------------
	�V�[���pcpp
	------------------------------------
	SceneBase.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "SceneBase.h"
#include "ObjectBase.h"
#include "ObjectCamera.h"
#include "CameraManager.h"
#include "ComponentCollisionBase.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
========================================== */
SceneBase::SceneBase()
	: m_bIsUpdating(false)	// �X�V���t���O��������
	, m_pObjects()			// �V�[���ɏ�������I�u�W�F�N�g�ꗗ
	, m_pStandbyObjects()	// �I�u�W�F�N�g���ꎞ�I�ɕۑ����Ă����z��
	, m_pObjectCollision()	// �e�I�u�W�F�N�g�����Փ˔���R���|�[�l���g
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
=========================================== */
void SceneBase::Init()
{
#ifdef _DEBUG
	InitObjectList();
#endif // _DEBUG

	// �N���A
	m_pObjects.clear();			// �V�[���ɏ�������I�u�W�F�N�g�ꗗ
	m_pStandbyObjects.clear();	// �I�u�W�F�N�g���ꎞ�I�ɕۑ����Ă����z��
	InitLocal();				// �ʏ���������

	CAMERA_MNG_INST.Init(this);	// �V�[���̃J������������
}

/* ========================================
	�I���֐�
	-------------------------------------
	���e�F�I������
=========================================== */
void SceneBase::Uninit()
{
#ifdef _DEBUG
	WIN_OBJ_LIST.Clear();
	WIN_OBJ_INFO.Clear();
	WIN_UI_LIST.Clear();
	WIN_UI_INFO.Clear();
	WIN_CAMERA_INFO["CameraList"].RemoveListItemAll();

#endif // _DEBUG


	// �����I�u�W�F�N�g�z��̑S�v�f���폜
	for (auto& pObject : m_pObjects)
	{
		pObject->Uninit();
	}
	m_pObjects.clear();	// �N���A

	// �ꎞ�ۑ����Ă����I�u�W�F�N�g�z��̑S�v�f���폜
	for (auto& pObject : m_pStandbyObjects)
	{
		pObject->Uninit();
	}
	m_pStandbyObjects.clear();	// �N���A

	UninitLocal();	// �ʏI������
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void SceneBase::Update()
{
	m_bIsUpdating = true;	// �X�V���t���O�𗧂Ă�

	// �����I�u�W�F�N�g�z��̑S�v�f���X�V
	for (auto& pObject : m_pObjects)
	{
		pObject->Update();
	}

	m_bIsUpdating = false;	// �X�V���t���O������

	// �ꎞ�ۑ��I�u�W�F�N�g�z��
	for (auto& pObject : m_pStandbyObjects)
	{
		pObject->Update();
		m_pObjects.emplace_back(std::move(pObject));	// �V�[���ɒǉ�
	}
	m_pStandbyObjects.clear();	// �N���A

	// �e�����蔻��X�V(���S�����I�u�W�F�N�g��m_bColStatesMap����폜���邽�߂ɂ��̈ʒu�ɋL�q)
	UpdateCollision();	

	// ���S��Ԃ̃I�u�W�F�N�g���폜
	for (auto it = m_pObjects.begin(); it != m_pObjects.end();)
	{
		// ���S��Ԃ��ǂ���
		if ((*it)->GetState() == ObjectBase::E_State::STATE_DEAD)
		{
#ifdef _DEBUG
			// �I�u�W�F�N�g�ꗗ���X�g����폜
			ITEM_OBJ_LIST.RemoveListItem((*it)->GetName().c_str());
#endif
			(*it)->Uninit();			// �I������
			it = m_pObjects.erase(it);	// �폜
		}
		else
		{
			++it;	// ���̗v�f��
		}
	}


	UpdateLocal();	// �ʍX�V����
}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
=========================================== */
void SceneBase::Draw()
{
	// �����I�u�W�F�N�g�z��̑S�v�f��`��
	for (auto& pObject : m_pObjects)
	{
		pObject->Draw();	
	}

	DrawLocal();	// �ʕ`�揈��
}

/* ========================================
	�Փ˔���z��ǉ��֐�
	-------------------------------------
	���e�F�Փ˔�����Ǘ�����z��ɒǉ�
		�@�Փ˔���R���|�[�l���g�̏������ŌĂ΂��
	-------------------------------------
	����1�F�Փ˔���R���|�[�l���g
=========================================== */
void SceneBase::AddObjectCollision(ComponentCollisionBase* pCollision)
{
	m_pObjectCollision.emplace_back(pCollision);	// emplace_back�Œǉ�(�R�s�[�������)
}

/* ========================================
	�Փ˔���z��폜�֐�
	-------------------------------------
	���e�F�Փ˔�����Ǘ�����z�񂩂�폜
		�@�Փ˔���R���|�[�l���g�̏I�������ŌĂ΂��
	-------------------------------------
	����1�F�Փ˔���R���|�[�l���g
=========================================== */
void SceneBase::RemoveObjectCollision(ComponentCollisionBase* pCollision)
{
	// �Փ˔�����Ǘ�����z�񂩂�폜
	m_pObjectCollision.erase(
		std::remove(m_pObjectCollision.begin(), m_pObjectCollision.end(), pCollision), m_pObjectCollision.end());
}

/* ========================================
	�����蔻��z��X�V�֐�
	-------------------------------------
	���e�F�e�I�u�W�F�N�g���������蔻��
		�@�R���|�[�l���g���X�V����
=========================================== */
void SceneBase::UpdateCollision()
{
	// ��F0��1�A0��2�A1��2�A1��3�A2��3�A�ƑS�Ă̑g�ݍ��킹�ŏՓ˔�����s��
	for (size_t i = 0; i < m_pObjectCollision.size(); ++i) {
		for (size_t j = i + 1; j < m_pObjectCollision.size(); ++j) {

			// �Փ˔�����s���R���|�[�l���g���擾
			ComponentCollisionBase* collisionA = m_pObjectCollision.at(i);
			ComponentCollisionBase* collisionB = m_pObjectCollision.at(j);

			collisionA->UpdateCollision(collisionB);
			collisionB->UpdateCollision(collisionA); 
		}
	}
}

/* ========================================
	�^�O�ʃI�u�W�F�N�g���W�֐�
	-------------------------------------
	���e�F�V�[���ɏ����������̃^�O�̃I�u�W�F�N�g
		�@��S�Ď��W����
		 ���e���v���[�g�ł͂Ȃ����A�����悤�ȏ����ׁ̈A�����ɋL�ځB
	-------------------------------------
	�ߒl�F�擾�����I�u�W�F�N�g�̃|�C���^�z��
========================================== */
std::vector<ObjectBase*> SceneBase::GetSceneObjectsTag(E_ObjectTag tag)
{
	// �擾�����I�u�W�F�N�g���i�[����z��
	std::vector<ObjectBase*> objects;

	// �V�[���ɏ�������I�u�W�F�N�g������
	for (const auto& pObject : m_pObjects)
	{
		// �I�u�W�F�N�g�̌^����v������z��ɒǉ�
		if (pObject->GetTag() == tag)
		{
			objects.push_back(pObject.get());
		}
	}

	return objects;	// �擾�����I�u�W�F�N�g�̔z���Ԃ�
}

/* ========================================
	���j�[�N���O�����֐�
	-------------------------------------
	���e�F���O���d�����Ă���ꍇ�ɁA
		�@�A�Ԃ��������O�𐶐�����
	-------------------------------------
	�����FsName	�I�u�W�F�N�g��
	-------------------------------------
	�ߒl�F������̖��O
=========================================== */
std::string SceneBase::CreateUniqueName(std::string sName)
{
	// ���O���d�����Ă���ꍇ�͘A�Ԃ�t����
	int nDupCnt = 0;	// �d����
	for (auto& pObject : m_pObjects)
	{
		// ���O���܂܂�Ă���ꍇ(���ɘA�Ԃ����Ă���ꍇ��z�肵��)
		if (pObject->GetName().find(sName) != std::string::npos)
		{
			nDupCnt++;
		}
	}

	if (nDupCnt > 0)
	{
		sName += "_" + std::to_string(nDupCnt);
	}

	return sName;
}


// �f�o�b�O�p ========================================================
#ifdef _DEBUG

/* ========================================
	�E�B���h�E������(�I�u�W�F�N�g�ꗗ)�֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�ꗗ�̏��������s��
=========================================== */
void SceneBase::InitObjectList()
{
	DebugUI::Item::ConstCallback  FuncListClick = [this](const void* arg) {
		// �N���b�N���ꂽ�I�u�W�F�N�g�̏���\��

		std::string sObjName = reinterpret_cast<const char*>(arg);

		// ���O��"L"���܂܂�Ă���ꍇ(�q�I�u�W�F�N�g�̏ꍇ)
		if (sObjName.find(DebugUI::CHILD_HEAD_TEXT) != std::string::npos) 
		{
			// "L"�������������O�ɕϊ�
			int nHeadTextCnt = sObjName.find(DebugUI::CHILD_HEAD_TEXT);					
			sObjName = sObjName.substr(nHeadTextCnt + DebugUI::CHILD_HEAD_TEXT.size());	
		}


		InitObjectInfo(sObjName);
	};

	DebugUI::Item* pList = DebugUI::Item::CreateList(ITEM_OBJ_LIST_NAME.c_str(), FuncListClick, false);
	WIN_OBJ_LIST.AddItem(pList);
}

/* ========================================
	�E�B���h�E������(�I�u�W�F�N�g���)�֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g���̏��������s��
		�@���ꗗ�̃I�u�W�F�N�g�����N���b�N����x�Ă΂��
	-------------------------------------
	�����Fstring �I�u�W�F�N�g��
=========================================== */
void SceneBase::InitObjectInfo(std::string sName)
{
	using namespace DebugUI;

	WIN_OBJ_INFO.Clear();	// �\�����Z�b�g

	// ���O����v����I�u�W�F�N�g������
	for (auto& pObject : m_pObjects)
	{
		if (pObject->GetName() == sName)
		{
			// �I�u�W�F�N�g����\��
			pObject->Debug();
			break;
		}
	}
}



#endif