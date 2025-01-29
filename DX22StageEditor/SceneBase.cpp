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

// �g�����X�t�H�[���G�f�B�^�p
#include "Input.h"	
#include "ComponentTransform.h"

// �I�u�W�F�N�g�����p
#include "ObjectTypeRegistry.h"


// =============== �萔��` =======================
const int OBJECT_LIST_LINE_NUM = 17;	// �I�u�W�F�N�g���X�g�̍s��

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
	, m_pSelectObj(nullptr)	// �I�𒆂̃I�u�W�F�N�g
	, m_nObjectListSelectNo(-1)
	, m_nTransEditMode(0)
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
	m_bIsUpdating = false;		// �X�V���t���O��������
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
	UninitLocal();	// �ʏI������

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

}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void SceneBase::Update()
{
#ifdef _DEBUG
	// �ό`�G�f�B�^�X�V
	UpdateTransformEdit();	
	// ���X�g���I�����͑I���I�u�W�F�N�g���N���A
	if (m_nObjectListSelectNo == -1) m_pSelectObj = nullptr;	
	// �I�u�W�F�N�g���X�g�ēǂݍ���
	ReloadDebugObjectList();	
#endif // _DEBUG
	UpdateObject();	// �I�u�W�F�N�g�X�V
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
	�I�u�W�F�N�g�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void SceneBase::UpdateObject()
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
		m_pObjects.emplace_back(std::move(pObject));	// �I�u�W�F�N�g�z��Ɉړ�
	}
	m_pStandbyObjects.clear();	// �N���A


	UpdateCollision();	// �e�����蔻��X�V(���S�����I�u�W�F�N�g��m_bColStatesMap����폜���邽�߂ɂ��̈ʒu�ɋL�q)
	RemoveDeadObjects();// ���S��Ԃ̃I�u�W�F�N�g���폜
}

/* ========================================
	�I�u�W�F�N�g�폜�֐�
	-------------------------------------
	���e�F���S��Ԃ̃I�u�W�F�N�g���폜
=========================================== */
void SceneBase::RemoveDeadObjects()
{
	// ���̃��[�v���ŃI�u�W�F�N�g�̏�Ԃ��ς��̂ŁA�ꎞ�ۑ��p�ɃR�s�[
	std::map<ObjectBase*, ObjectBase::E_State> pObjectStateMap;
	for (auto& pObject : m_pObjects)
		pObjectStateMap.insert(std::make_pair(pObject.get(), pObject->GetState()));


	// ���S��Ԃ̃I�u�W�F�N�g���폜
	for (auto it = m_pObjects.begin(); it != m_pObjects.end();)
	{
		ObjectBase* pObject = it->get();
		// ���S��Ԃ��ǂ���
		if (pObjectStateMap.at(pObject) == ObjectBase::E_State::STATE_DEAD)
		{
#ifdef _DEBUG
			// �폜�ΏۃI�u�W�F�N�g���ꗗ�őI�𒆂̏ꍇ
			if (m_nObjectListSelectNo == ITEM_OBJ_LIST.GetListNo(pObject->GetListName().c_str()))
			{
				WIN_OBJ_INFO.Clear();	// �I�u�W�F�N�g���E�B���h�E�N���A

				m_nObjectListSelectNo = -1;								// �I��ԍ������Z�b�g
				ITEM_OBJ_LIST.SetListNo(-1);	// �I��ԍ������Z�b�g
			}
#endif

			// �q�I�u�W�F�N�g������ꍇ
			if (pObject->GetChildObjects().size() > 0)
			{
				for (auto& pChild : pObject->GetChildObjects())
				{
					this->RemoveSceneObject(pChild);	// �q�I�u�W�F�N�g���폜
				}
			}
			// �e�I�u�W�F�N�g������ꍇ
			if (pObject->GetParentObject())
			{
				pObject->GetParentObject()->RemoveChildObject(pObject);	// �e�I�u�W�F�N�g����폜
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
	if (pObject->GetChildObjects().size() > 0)
	{
		for (auto& pChild : pObject->GetChildObjects())
		{
			this->RemoveSceneObject(pChild);	// �q�I�u�W�F�N�g���폜
		}
	}

	pObject->RemoveParentObject();						// �e�I�u�W�F�N�g����폜
	pObject->SetState(ObjectBase::E_State::STATE_DEAD);	// ���S��Ԃɐݒ�

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

	// �Փˏ�ԃ}�b�v�X�V(��L�̏����ŏՓˏ�Ԃ��ς��������)
	for (size_t i = 0; i < m_pObjectCollision.size(); ++i) {
		for (size_t j = i + 1; j < m_pObjectCollision.size(); ++j) {

			// �Փ˔�����s���R���|�[�l���g���擾
			ComponentCollisionBase* collisionA = m_pObjectCollision.at(i);
			ComponentCollisionBase* collisionB = m_pObjectCollision.at(j);

			collisionA->UpdateCollisionMap(collisionB);
			collisionB->UpdateCollisionMap(collisionA);
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
		return sName = CreateUniqueName(sName);	// �I�u�W�F�N�g���Əd���`�F�b�N
	}
	else
	{
		return sName;
	}
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
	�^�O�ʃI�u�W�F�N�g���W�֐�
	-------------------------------------
	���e�F�V�[���ɏ����������̃^�O�̃I�u�W�F�N�g
		�@����ԍŏ��Ɍ����������̂��擾����
	-------------------------------------
	�ߒl�F�擾�����I�u�W�F�N�g�̃|�C���^
========================================== */
ObjectBase* SceneBase::GetSceneObjectTag(E_ObjectTag tag)
{
	// �V�[���ɏ�������I�u�W�F�N�g������
	for (const auto& pObject : m_pObjects)
	{
		// ��ԍŏ��Ɍ��������I�u�W�F�N�g��Ԃ�
		if (pObject->GetTag() == tag)
		{
			return pObject.get();
		}
	}

	return nullptr;
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

	// �I�u�W�F�N�g�폜�{�^��
	WIN_OBJ_LIST.AddItem(Item::CreateCallBack("Remove", Item::Kind::Command, [this](bool isWrite, void* arg)
	{
		// �I������Ă��Ȃ��ꍇ�͏������Ȃ�
		if (m_nObjectListSelectNo == -1) return;					
		// �V�[����̃J������1�̏ꍇ�A�J�����I�u�W�F�N�g�͍폜�s��
		if (dynamic_cast<ObjectCamera*>(m_pSelectObj) != nullptr && CAMERA_MNG_INST.GetCameraNum() == 1) return;
		// �J�����I�u�W�F�N�g���A�N�e�B�u�̏ꍇ�A�폜�s��
		if (CAMERA_MNG_INST.GetActiveCamera() == m_pSelectObj)	return;

		m_pSelectObj->SetState(ObjectBase::E_State::STATE_DEAD);	// ���S��Ԃɐݒ�

	}));

	// �I�u�W�F�N�g�t�H�[�J�X�{�^��
	WIN_OBJ_LIST.AddItem(Item::CreateCallBack("Focus", Item::Kind::Command, [this](bool isWrite, void* arg)
	{
		// �I������Ă��Ȃ��ꍇ�͏������Ȃ�
		if (m_nObjectListSelectNo == -1) return;					
		// �A�N�e�B�u�J�����̓t�H�[�J�X�ړ��s��
		if (m_pSelectObj == CAMERA_MNG_INST.GetActiveCamera()) return; 

		CAMERA_MNG_INST.FocusMoveCamera(m_pSelectObj);	// �J�������w��I�u�W�F�N�g�Ƀt�H�[�J�X�ړ�

	}, false, true));

	// �I�u�W�F�N�g�����{�^��
	WIN_OBJ_LIST.AddItem(Item::CreateCallBack("Copy", Item::Kind::Command, [this](bool isWrite, void* arg)
	{
		// �I������Ă��Ȃ��ꍇ�͏������Ȃ�
		if (m_nObjectListSelectNo == -1) return;
		// �I�u�W�F�N�g�𕡐�
		CopyObject(m_pSelectObj);

	}, false, true));

	// �I�u�W�F�N�g�I�����̃R�[���o�b�N�֐�
	Item::ConstCallback  FuncListClick = [this](const void* arg) 
	{
		// �N���b�N���ꂽ�I�u�W�F�N�g�̏���\��
		std::string sObjName = reinterpret_cast<const char*>(arg);
		m_nObjectListSelectNo = ITEM_OBJ_LIST.GetListNo(sObjName.c_str());	// �I��ԍ����擾

		InitObjectInfo(sObjName);
	};

	// �I�u�W�F�N�g���X�g���쐬
	WIN_OBJ_LIST.AddItem(Item::CreateList(ITEM_OBJ_LIST_NAME.c_str(), FuncListClick, false, false, false, OBJECT_LIST_LINE_NUM));
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

	// ���O��"L"���܂܂�Ă���ꍇ(�q�I�u�W�F�N�g�̏ꍇ)
	if (sName.find(CHILD_HEAD_TEXT) != std::string::npos)
	{
		// "L"�������������O�ɕϊ�
		int nHeadTextCnt	= sName.find(CHILD_HEAD_TEXT);							// L���܂܂��ʒu���擾
		sName				= sName.substr(nHeadTextCnt + CHILD_HEAD_TEXT.size());	// L�ȍ~�̕�������擾
	}

	// ���O��"*"���܂܂�Ă���ꍇ(�e�I�u�W�F�N�g�̏ꍇ)
	if (sName.find(PARENT_END_TEXT) != std::string::npos)
	{
		// "*"�������������O�ɕϊ�
		int nEndTextCnt = sName.find(PARENT_END_TEXT);	// *���܂܂��ʒu���擾
		sName			= sName.substr(0, nEndTextCnt);	// *�ȑO�̕�������擾
	}

	// ���O����v����I�u�W�F�N�g������
	for (auto& pObject : m_pObjects)
	{
		if (pObject->GetName() == sName)
		{
			// �I�u�W�F�N�g����\��
			pObject->Debug();
			m_pSelectObj = pObject.get();	// �I�𒆂̃I�u�W�F�N�g��ێ�

			bool bIsFold = pObject->GetIsFold() ? false : true;
			pObject->SetIsFold(bIsFold);	// �܂肽���ݏ�Ԃ�ύX

			break;
		}
	}
}

/* ========================================
	�ό`�G�f�B�^���[�h�ݒ�֐�
	-------------------------------------
	���e�F�ό`�G�f�B�^�̃��[�h��ݒ�
	-------------------------------------
	�����Fint ���[�h(0:�Ȃ�, 1:�ړ�, 2:��], 3:�g��)
=========================================== */
void SceneBase::SetTransformEditMode(int nMode)
{
	m_nTransEditMode = nMode;
}

/* ========================================
	�ό`�G�f�B�^���Z�b�g�֐�
	-------------------------------------
	���e�F�ό`�G�f�B�^�̃��Z�b�g�{�^���������ꂽ���̏���
=========================================== */
void SceneBase::ResetTransformEdit()
{
	if (m_nObjectListSelectNo == -1) return;	// �I������Ă��Ȃ��ꍇ�͏������Ȃ�

	// �I�𒆂̃I�u�W�F�N�g�̃g�����X�t�H�[�����擾
	ComponentTransform* pTrans = m_pSelectObj->GetComponent<ComponentTransform>();

	// �ό`�G�f�B�^�̃��[�h�ɂ���ă��Z�b�g������ύX
	switch (m_nTransEditMode)
	{
	// None�̏ꍇ�͑S�ă��Z�b�g
	case 0:
		pTrans->SetLocalPosition(Vector3<float>(0.0f, 0.0f, 0.0f));
		pTrans->SetLocalRotationEuler(Vector3<float>(0.0f, 0.0f, 0.0f));
		pTrans->SetLocalScale(Vector3<float>(1.0f, 1.0f, 1.0f));
		break;
	// �ړ��̏ꍇ�͈ʒu���Z�b�g
	case 1:
		pTrans->SetLocalPosition(Vector3<float>(0.0f, 0.0f, 0.0f));
		break;
	// ��]�̏ꍇ�͉�]���Z�b�g
	case 2:
		pTrans->SetLocalRotationEuler(Vector3<float>(0.0f, 0.0f, 0.0f));
		break;
	// �g��̏ꍇ�͊g�僊�Z�b�g
	case 3:
		pTrans->SetLocalScale(Vector3<float>(1.0f, 1.0f, 1.0f));
		break;
	}
}

/* ========================================
	�ό`�G�f�B�^�X�V�֐�
	-------------------------------------
	���e�F�ό`�G�f�B�^�̍X�V����
=========================================== */
void SceneBase::UpdateTransformEdit()
{
	if (m_nObjectListSelectNo == -1) return;	// �I������Ă��Ȃ��ꍇ�͏������Ȃ�
	if (Input::IsKeyPress(VK_SHIFT)) return;	// Shift�L�[��������Ă���ꍇ�͏������Ȃ�

	// �I�𒆂̃I�u�W�F�N�g�̃g�����X�t�H�[�����擾
	ComponentTransform* pTrans = m_pSelectObj->GetComponent<ComponentTransform>();
	// �f�o�b�O���j���[�̃��[�h�e�L�X�g���擾
	DebugUI::Item* pModeText = &WIN_TRANSFORM_EDIT["Mode"];
	pModeText->SetText("None");	// �I���Ȃ���None

	// �ό`�G�f�B�^�̃��[�h�ɂ���ď�����ύX
	switch (m_nTransEditMode)
	{
	case 1:	// �ړ�
	{
		pModeText->SetText("Position");
		float fChangeVal = WIN_TRANSFORM_EDIT["ValuePos"].GetFloat() * DELTA_TIME;	// �ω��l

		// �ړ�����
		if (Input::IsKeyPress('W')) pTrans->TranslateZ(fChangeVal);
		if (Input::IsKeyPress('S')) pTrans->TranslateZ(-fChangeVal);
		if (Input::IsKeyPress('A')) pTrans->TranslateX(-fChangeVal);
		if (Input::IsKeyPress('D')) pTrans->TranslateX(fChangeVal);
		if (Input::IsKeyPress('E')) pTrans->TranslateY(fChangeVal);
		if (Input::IsKeyPress('Q')) pTrans->TranslateY(-fChangeVal);
		break;
	}
	case 2:	// ��]
	{
		pModeText->SetText("Rotation");
		float fChangeVal = WIN_TRANSFORM_EDIT["ValueRot"].GetFloat() * DELTA_TIME;	// �ω��l

		// ��]����
		if (Input::IsKeyPress('W')) pTrans->RotateX(fChangeVal);
		if (Input::IsKeyPress('S')) pTrans->RotateX(-fChangeVal);
		if (Input::IsKeyPress('A')) pTrans->RotateY(-fChangeVal);
		if (Input::IsKeyPress('D')) pTrans->RotateY(fChangeVal);
		if (Input::IsKeyPress('E')) pTrans->RotateZ(fChangeVal);
		if (Input::IsKeyPress('Q')) pTrans->RotateZ(-fChangeVal);
		break;
	}
	case 3:	// �g��
	{
		pModeText->SetText("Scale");
		float fChangeVal = WIN_TRANSFORM_EDIT["ValueScale"].GetFloat() * DELTA_TIME;	// �ω��l

		// �g�嗦
		float fChangPlus	= 1.0f + fChangeVal;
		float fChangMinus	= 1.0f - fChangeVal;

		// �g�又��
		if (Input::IsKeyPress('W')) pTrans->ScaleZ(fChangPlus);
		if (Input::IsKeyPress('S')) pTrans->ScaleZ(fChangMinus);
		if (Input::IsKeyPress('A')) pTrans->ScaleX(fChangMinus);
		if (Input::IsKeyPress('D')) pTrans->ScaleX(fChangPlus);
		if (Input::IsKeyPress('E')) pTrans->ScaleY(fChangPlus);
		if (Input::IsKeyPress('Q')) pTrans->ScaleY(fChangMinus);
		break;
	}
	}
}

/* ========================================
	�f�o�b�O�p�I�u�W�F�N�g�ꗗ�ēǍ��֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�ꗗ���ēǂݍ��݂���
=========================================== */
void SceneBase::ReloadDebugObjectList()
{
	// �I�u�W�F�N�g�ꗗ���N���A
	ITEM_OBJ_LIST.RemoveListItemAll();

	// �I�u�W�F�N�g�𖼑O�̏����Ƀ\�[�g����(�I�u�W�F�N�g�ꗗ�����₷�����邽��)
	std::vector<ObjectBase*> pSortObjects = GetAllSceneObjects();	// �V�[���ɏ�������S�ẴI�u�W�F�N�g���擾
	std::sort(pSortObjects.begin(), pSortObjects.end(), [](const ObjectBase* a, const ObjectBase* b)
	{
		return a->GetName() < b->GetName();
	});

	// �S�ẴI�u�W�F�N�g�����X�g�ɒǉ�
	for (const auto& pObject : pSortObjects)
	{
		if (pObject->GetParentObject()) continue;	// �e�I�u�W�F�N�g������ꍇ�͔�΂�
		// �I�u�W�F�N�g�ꗗ�ɒǉ�
		ITEM_OBJ_LIST.AddListItem(pObject->GetListName().c_str());

		// �܂肽���ݏ�Ԃł͂Ȃ��ꍇ�͎q�I�u�W�F�N�g��\������
		if(!pObject->GetIsFold())
			AddObjectListChild(pObject);
	}

}


/* ========================================
	�f�o�b�O�p�I�u�W�F�N�g�ꗗ�q�I�u�W�F�N�g�ǉ��֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�ꗗ�Ɏq�I�u�W�F�N�g��ǉ�
		�@���q���������ċA�I�ɌĂяo�����
	-------------------------------------
	�����FObjectBase* �e�I�u�W�F�N�g
=========================================== */
void SceneBase::AddObjectListChild(ObjectBase* pObject)
{
	// �q�I�u�W�F�N�g������ꍇ
	if (pObject->GetChildObjects().size() > 0)
	{
		if (pObject->GetIsFold()) return;	// �܂肽���ݏ�Ԃ̏ꍇ�͒ǉ����Ȃ�

		// �q�I�u�W�F�N�g�𖼑O�̏����Ƀ\�[�g����(�I�u�W�F�N�g�ꗗ�����₷�����邽��)
		std::vector<ObjectBase*> pSortChildObjects = pObject->GetChildObjects();	// �q�I�u�W�F�N�g���擾
		std::sort(pSortChildObjects.begin(), pSortChildObjects.end(), [](const ObjectBase* a, const ObjectBase* b)
		{
			return a->GetName() > b->GetName();
		});

		// �S�Ă̎q�I�u�W�F�N�g�����X�g�ɒǉ�
		for (auto& pChild : pSortChildObjects)
		{
			// �}���ʒu
			int nInsertNo = ITEM_OBJ_LIST.GetListNo(pObject->GetListName().c_str());
			// �I�u�W�F�N�g�ꗗ�ɒǉ�
			ITEM_OBJ_LIST.InsertListItem(pChild->GetListName().c_str(), nInsertNo + 1);
			// �q�I�u�W�F�N�g��ǉ�
			AddObjectListChild(pChild);
		}
	}
	else
	{
		return;
	}
}

/* ========================================
	�I�u�W�F�N�g�����֐�
	-------------------------------------
	���e�F�I�𒆂̃I�u�W�F�N�g�𕡐�����
	-------------------------------------
	�����FObjectBase* �������I�u�W�F�N�g
	-------------------------------------
	�ߒl�FObjectBase* ���������I�u�W�F�N�g
=========================================== */
ObjectBase* SceneBase::CopyObject(ObjectBase* pOriginalObj)
{
	// �g�����X�t�H�[�������擾
	ComponentTransform* pOriTrans = pOriginalObj->GetTransform();

	ObjectBase* pCopyObj = OBJ_TYPE_REGISTRY_INST.CreateObject(pOriginalObj->GetObjClassName());
	pCopyObj->Init(CreateUniqueName(pOriginalObj->GetName()));	// �I�u�W�F�N�g������(���O�d��������)
	ComponentTransform* pCopyTrans = pCopyObj->GetComponent<ComponentTransform>();
	pCopyTrans->SetPosition(pOriTrans->GetPosition());
	pCopyTrans->SetRotation(pOriTrans->GetRotation());
	pCopyTrans->SetScale(pOriTrans->GetScale());


	AddSceneObjectBase(pCopyObj);	// �V�[���ɃI�u�W�F�N�g��ǉ�

	// �e�q�֌W���Č�
	// �e�I�u�W�F�N�g������ꍇ
	if (pOriginalObj->GetParentObject())
	{
		pCopyObj->SetParentObject(pOriginalObj->GetParentObject());
	}

	// �q�I�u�W�F�N�g������ꍇ
	if (pOriginalObj->GetChildObjects().size() > 0)
	{
		for (auto& pChild : pOriginalObj->GetChildObjects())
		{
			ObjectBase* pCopyChild = CopyObject(pChild);
			pCopyObj->AddChildObject(pCopyChild);
		}
	}


	return pCopyObj;
}


#endif