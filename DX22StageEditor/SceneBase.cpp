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
#ifdef _DEBUG
	, m_nObjectListSelectNo(-1)
#endif // _DEBUG
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

	RemoveDeadObjects();		// ���S��Ԃ̃I�u�W�F�N�g���폜



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
	�I�u�W�F�N�g�폜�֐�
	-------------------------------------
	���e�F���S��Ԃ̃I�u�W�F�N�g���폜
=========================================== */
void SceneBase::RemoveDeadObjects()
{
	// ���S��Ԃ̃I�u�W�F�N�g���폜
	for (auto it = m_pObjects.begin(); it != m_pObjects.end();)
	{
		ObjectBase* pObject = it->get();
		// ���S��Ԃ��ǂ���
		if (pObject->GetState() == ObjectBase::E_State::STATE_DEAD)
		{
#ifdef _DEBUG
			// �I�u�W�F�N�g�ꗗ���X�g����폜
			if (m_nObjectListSelectNo == WIN_OBJ_LIST[ITEM_OBJ_LIST_NAME.c_str()].GetListNo(pObject->GetListName().c_str()))
			{
				WIN_OBJ_INFO.Clear();	// �I�u�W�F�N�g���E�B���h�E�N���A

				m_nObjectListSelectNo = -1;								// �I��ԍ������Z�b�g
				WIN_OBJ_LIST[ITEM_OBJ_LIST_NAME.c_str()].SetListNo(-1);	// �I��ԍ������Z�b�g
			}
			ITEM_OBJ_LIST.RemoveListItem(pObject->GetListName().c_str());	// �I�u�W�F�N�g�ꗗ����폜
#endif
			// �e�I�u�W�F�N�g������ꍇ
			if (pObject->GetParentObject())
			{
				pObject->GetParentObject()->RemoveChildObject(pObject);	// �e�I�u�W�F�N�g����폜
			}
			// �q�I�u�W�F�N�g������ꍇ
			if (pObject->GetChildObjects().size() > 0)
			{
				for (auto& pChild : pObject->GetChildObjects())
				{
					this->RemoveSceneObject(pChild);	// �q�I�u�W�F�N�g���폜
				}
			}

			pObject->Uninit();			// �I������
			it = m_pObjects.erase(it);	// �폜
		}
		else
		{
			++it;	// ���̗v�f��
		}
	}
}

/* ========================================
	�I�u�W�F�N�g�ǉ��֐�
	-------------------------------------
	���e�F�V�[���ɃI�u�W�F�N�g��ǉ�
		�@���t�@�C���f�[�^����I�u�W�F�N�g��ǉ�����ꍇ�Ɏg�p
	-------------------------------------
	�����F�ǉ�����I�u�W�F�N�g�|�C���^
=========================================== */
void SceneBase::AddSceneObjectBase(ObjectBase* pObject)
{
	// �V�[�����X�V�����ǂ������`�F�b�N���܂�
	if (m_bIsUpdating)
	{
		// �ꎞ�ۑ��p�̔z��Ƀ��j�[�N�|�C���^���ړ����܂�
		m_pStandbyObjects.push_back(std::unique_ptr<ObjectBase>(pObject));
	}
	else
	{
		// �V�[���̃I�u�W�F�N�g�z��Ƀ��j�[�N�|�C���^���ړ����܂�
		m_pObjects.push_back(std::unique_ptr<ObjectBase>(pObject));
	}

#ifdef _DEBUG
	// �I�u�W�F�N�g�ꗗ�ɒǉ�
	ITEM_OBJ_LIST.AddListItem(pObject->GetName().c_str());
#endif
}

/* ========================================
	�I�u�W�F�N�g�����֐�
	-------------------------------------
	���e�F�V�[���ɏ�������I�u�W�F�N�g������
	-------------------------------------
	�����FsName	�I�u�W�F�N�g��
	-------------------------------------
	�ߒl�F�擾�I�u�W�F�N�g�|�C���^
=========================================== */
ObjectBase* SceneBase::FindSceneObject(std::string sName)
{
	// ���O����v����I�u�W�F�N�g������
	for (auto& pObject : m_pObjects)
	{
		if (pObject->GetName() == sName)
		{
			return pObject.get();
		}
	}

	return nullptr;
}

/* ========================================
	�I�u�W�F�N�g�폜�֐�
	-------------------------------------
	���e�F�V�[���ɏ�������I�u�W�F�N�g���폜
		�@�����S��Ԃ̃I�u�W�F�N�g���폜����ꍇ�Ɏg�p(�q�I�u�W�F�N�g�̍폜��)
	-------------------------------------
	�����FpObject	�폜����I�u�W�F�N�g�|�C���^
=========================================== */
void SceneBase::RemoveSceneObject(ObjectBase* pObject)
{
#ifdef _DEBUG
	WIN_OBJ_LIST[ITEM_OBJ_LIST_NAME.c_str()].RemoveListItem(pObject->GetListName().c_str());
#endif // _DEBUG

	// �V�[���̃I�u�W�F�N�g�z�񂩂�폜
	m_pObjects.erase(std::remove_if(m_pObjects.begin(), m_pObjects.end(),
		[pObject](const std::unique_ptr<ObjectBase>& pObj) { return pObj.get() == pObject; }), m_pObjects.end());
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

/* ========================================
	�^�O�ʃI�u�W�F�N�g���W�֐�
	-------------------------------------
	���e�F�V�[���ɏ����������̃^�O�̃I�u�W�F�N�g
		�@��S�Ď��W����
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
	�S�I�u�W�F�N�g�擾�֐�
	-------------------------------------
	���e�F�V�[���ɏ�������S�ẴI�u�W�F�N�g���擾
	-------------------------------------
	�ߒl�F�擾�����I�u�W�F�N�g�̃|�C���^�z��
=========================================== */
std::vector<ObjectBase*> SceneBase::GetAllSceneObjects()
{
	std::vector<ObjectBase*> objects;

	for (const auto& pObject : m_pObjects)
	{
		objects.push_back(pObject.get());
	}

	return objects;
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
	using namespace DebugUI;

	Item::ConstCallback  FuncListClick = [this](const void* arg) {
		// �N���b�N���ꂽ�I�u�W�F�N�g�̏���\��

		std::string sObjName = reinterpret_cast<const char*>(arg);
		m_nObjectListSelectNo = WIN_OBJ_LIST[ITEM_OBJ_LIST_NAME.c_str()].GetListNo(sObjName.c_str());	// �I��ԍ����擾

		// ���O��"L"���܂܂�Ă���ꍇ(�q�I�u�W�F�N�g�̏ꍇ)
		if (sObjName.find(CHILD_HEAD_TEXT) != std::string::npos)
		{
			// "L"�������������O�ɕϊ�
			int nHeadTextCnt = sObjName.find(CHILD_HEAD_TEXT);
			sObjName = sObjName.substr(nHeadTextCnt + CHILD_HEAD_TEXT.size());
		}

		InitObjectInfo(sObjName);

	};

	Item* pList = Item::CreateList(ITEM_OBJ_LIST_NAME.c_str(), FuncListClick, false);
	WIN_OBJ_LIST.AddItem(pList);

	// �I�u�W�F�N�g�폜�{�^��
	WIN_OBJ_LIST.AddItem(Item::CreateCallBack("ObjectRemove", Item::Kind::Command, [this](bool isWrite, void* arg)
	{
		if (m_nObjectListSelectNo == -1) return;					// �I������Ă��Ȃ��ꍇ�͏������Ȃ�
		std::string sName = WIN_OBJ_INFO["ObjectName"].GetText();	// �I�����ꂽ�I�u�W�F�N�g�����擾

		// ���O����v����I�u�W�F�N�g������
		for (auto& pObject : m_pObjects)
		{
			if (pObject->GetName() == sName)
			{
				// �V�[����̃J������1�̏ꍇ�A�J�����I�u�W�F�N�g�͍폜�s��
				if (dynamic_cast<ObjectCamera*>(pObject.get()) != nullptr && CAMERA_MNG_INST.GetCameraNum() == 1)
				{
					break;
				}
				// �J�����I�u�W�F�N�g���A�N�e�B�u�̏ꍇ�A�폜�s��
				if (CAMERA_MNG_INST.GetActiveCamera() == pObject.get())
				{
					break;
				}

				pObject->SetState(ObjectBase::E_State::STATE_DEAD);		// ���S��Ԃɐݒ�
				break;
			}

		}

	}));

	// �I�u�W�F�N�g�t�H�[�J�X�{�^��
	WIN_OBJ_LIST.AddItem(Item::CreateCallBack("ObjectFocus", Item::Kind::Command, [this](bool isWrite, void* arg)
	{
		if (m_nObjectListSelectNo == -1) return;					// �I������Ă��Ȃ��ꍇ�͏������Ȃ�
		std::string sName = WIN_OBJ_INFO["ObjectName"].GetText();	// �I�����ꂽ�I�u�W�F�N�g�����擾

		// ���O����v����I�u�W�F�N�g������
		for (auto& pObject : m_pObjects)
		{
			if (pObject->GetName() == sName)
			{
				if (pObject.get() == CAMERA_MNG_INST.GetActiveCamera()) break; // �A�N�e�B�u�J�����̓t�H�[�J�X�ړ��s��

				CAMERA_MNG_INST.FocusMoveCamera(pObject.get());	// �J�������w��I�u�W�F�N�g�Ƀt�H�[�J�X�ړ�
				break;
			}

		}

	}));
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