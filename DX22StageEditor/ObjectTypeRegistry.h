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


// =============== �萔��` =======================
#define OBJ_TYPE_REGISTRY_INST ObjectTypeRegistry::GetInstance()    // �C���X�^���X�擾�p�}�N��
#define REGISTER_OBJECT_TYPE(className)                                             \
    ObjectTypeRegistry::GetInstance().Register(#className,                          \
        []() -> ObjectBase* { return new className(SceneManager::GetScene()); })    \

// =============== �N���X��` ===================
class ObjectTypeRegistry
{
public:
	using CreateFunction = ObjectBase*(); // �֐��|�C���^�̌^���`
public:
    static ObjectTypeRegistry& GetInstance();
	void Register(const std::string& sClassName, CreateFunction* func); // �}�b�v(������E�N���X�����֐�)�ɓo�^
	ObjectBase* CreateObject(const std::string& sClassName);            // �I�u�W�F�N�g����

	static void RegisterAllObjectTypes();                               // �I�u�W�F�N�g�̓o�^���s���֐�

    // �Q�b�^�[
    std::unordered_map<std::string, CreateFunction*>& GetObjectTypeMap();

private:
	// ������ƃN���X�����֐��̃}�b�v
    std::unordered_map<std::string, CreateFunction*> m_ObjectTypeMap; 

};

