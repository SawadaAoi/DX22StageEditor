/* ========================================
    DX22Base/
    ------------------------------------
    �I�u�W�F�N�g��ޓo�^�p�w�b�_
    ------------------------------------
	�����F�����񂩂�I�u�W�F�N�g�𐶐����邽�߂̓o�^�N���X
    ------------------------------------
    ObjectTypeRegistry.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBase.h"
#include "SceneManager.h"
#include <unordered_map>

// =============== �萔��` =======================
#define OBJ_TYPE_REGISTRY_INST ObjectTypeRegistry::GetInstance()    // �C���X�^���X�擾�p�}�N��
#define REGISTER_OBJECT_TYPE(className, cateNum)									\
    ObjectTypeRegistry::GetInstance().Register(#className,                          \
        []() -> ObjectBase* { return new className(SceneManager::GetScene()); });   \
	ObjectTypeRegistry::GetInstance().RegisterCategory(#className, cateNum);		\

// =============== �N���X��` ===================
class ObjectTypeRegistry
{
public:
	using CreateFunction = ObjectBase*(); // �֐��|�C���^�̌^���`

    enum ObjectCategoryType
	{
		OCT_EMPTY,
		OCT_PLAYER,
		OCT_ENEMY,
		OCT_TERRAIN,
		OCT_CAMERA,
		OCT_LIGHT,
		OCT_SYSTEM,
		OCT_OTHER,
	};;

public:
    static ObjectTypeRegistry& GetInstance();
	void Register(const std::string& sClassName, CreateFunction* func); // �}�b�v(������E�N���X�����֐�)�ɓo�^
	ObjectBase* CreateObject(const std::string& sClassName);            // �I�u�W�F�N�g����

	static void RegisterAllObjectTypes();                               // �I�u�W�F�N�g�̓o�^���s���֐�

	void RegisterCategory(const std::string& sClassName, ObjectCategoryType cateNum); // �J�e�S���}�b�v�ɓo�^

    // �Q�b�^�[
    std::unordered_map<std::string, CreateFunction*>& GetObjectTypeMap();
	std::unordered_map<std::string, ObjectCategoryType>& GetObjectCategoryMap();

private:
	// ������ƃN���X�����֐��̃}�b�v
    std::unordered_map<std::string, CreateFunction*> m_ObjectTypeMap; 
	// �ꗗ�Ŏ�ʕ\�����邽�߂̃}�b�v
	std::unordered_map<std::string, ObjectCategoryType> m_ObjectCategoryMap;
};

