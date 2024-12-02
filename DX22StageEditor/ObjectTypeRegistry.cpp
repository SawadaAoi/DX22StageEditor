/* ========================================
	DX22Base/
	------------------------------------
	�I�u�W�F�N�g��ޓo�^�pcpp
	------------------------------------
	ObjectTypeRegistry.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectTypeRegistry.h"

// �I�u�W�F�N�g
#include "ObjectPlayer.h"
#include "ObjectGround.h"
#include "ObjectCamera.h"

/* ========================================
	�C���X�^���X�擾�֐�
	-------------------------------------
	���e�F�C���X�^���X���擾����
	-------------------------------------
	�ߒl�F�C���X�^���X
=========================================== */
ObjectTypeRegistry& ObjectTypeRegistry::GetInstance()
{
	static ObjectTypeRegistry instance;
	return instance;
}

/* ========================================
	�I�u�W�F�N�g�N���X�o�^�֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�N���X���}�b�v�ɓo�^����
	-------------------------------------
	����1�F�I�u�W�F�N�g�N���X��
	����2�F�I�u�W�F�N�g�����֐�
=========================================== */
void ObjectTypeRegistry::Register(const std::string& sClassName, CreateFunction* func)
{
	m_ObjectTypeMap[sClassName] = func;
}

/* ========================================
	�I�u�W�F�N�g�����֐�
	-------------------------------------
	���e�F�N���X�������񂩂�I�u�W�F�N�g�𐶐�����
	-------------------------------------
	�����F�I�u�W�F�N�g�N���X��
	-------------------------------------
	�ߒl�F�I�u�W�F�N�g
=========================================== */
ObjectBase* ObjectTypeRegistry::CreateObject(const std::string& sClassName)
{
	// �N���X�����o�^����Ă��邩�m�F
	if (m_ObjectTypeMap.find(sClassName) != m_ObjectTypeMap.end())
	{
		return m_ObjectTypeMap.at(sClassName)();
	}
	return nullptr;
}

/* ========================================
	�S�I�u�W�F�N�g�o�^�֐�
	-------------------------------------
	���e�F�S�ẴI�u�W�F�N�g��o�^����
=========================================== */
void ObjectTypeRegistry::RegisterAllObjectTypes()
{
	REGISTER_OBJECT_TYPE(ObjectBase);
	REGISTER_OBJECT_TYPE(ObjectPlayer);
	REGISTER_OBJECT_TYPE(ObjectGround);
	REGISTER_OBJECT_TYPE(ObjectCamera);
    
}

/* ========================================
	�Q�b�^�[(�I�u�W�F�N�g�}�b�v)�֐�
	-------------------------------------
	�ߒl�Funordered_map<std::string, CreateFunction*>& �I�u�W�F�N�g�}�b�v
=========================================== */
std::unordered_map<std::string, ObjectTypeRegistry::CreateFunction*>& ObjectTypeRegistry::GetObjectTypeMap()
{
	return m_ObjectTypeMap;
}
