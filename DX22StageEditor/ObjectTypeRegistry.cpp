/* ========================================
	DX22Base/
	------------------------------------
	�I�u�W�F�N�g��ޓo�^�pcpp
	------------------------------------
	ObjectTypeRegistry.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectTypeRegistry.h"

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
	�I�u�W�F�N�g�o�^�֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̓o�^���s��
=========================================== */
void ObjectTypeRegistry::RegisterCategory(const std::string& sClassName, ObjectCategoryType cateNum)
{
	m_ObjectCategoryMap[sClassName] = cateNum;
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

/* ========================================
	�Q�b�^�[(�J�e�S���}�b�v)�֐�
	-------------------------------------
	�ߒl�Funordered_map<std::string, ObjectCategoryType>& �J�e�S���}�b�v
=========================================== */
std::unordered_map<std::string, ObjectTypeRegistry::ObjectCategoryType>& ObjectTypeRegistry::GetObjectCategoryMap()
{
	return m_ObjectCategoryMap;
}
