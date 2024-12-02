/* ========================================
	DX22Base/
	------------------------------------
	�V�[���p�w�b�_
	------------------------------------
	�����F�e�V�[���̊��N���X
	------------------------------------
	SceneBase.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include <vector>
#include "ObjectBase.h"
#include <memory>
#include "DebugMenu.h"
#include "UIObjectBase.h"

// =============== �O���錾 =======================
class ComponentCollisionBase;


// =============== �N���X��` =====================
class SceneBase
{
public:
	SceneBase();
	virtual ~SceneBase() {};

	void Init();	// ����������
	void Uninit();	// �I������
	void Update();	// �X�V����
	void Draw();	// �`�揈��

	virtual void InitLocal() {};	// �ʏ���������
	virtual void UninitLocal() {};	// �ʏI������
	virtual void UpdateLocal() {};	// �ʍX�V����
	virtual void DrawLocal() {};	// �ʕ`�揈��

	// �I�u�W�F�N�g�֘A
	template<typename T>
	T* AddSceneObject(std::string sName);	// �I�u�W�F�N�g�ǉ�
	template<typename T>
	T* GetSceneObject(std::string sName);	// �I�u�W�F�N�g�擾
	template<typename T>
	std::vector<T*> GetSceneObjects();		// �I�u�W�F�N�g�擾

	void AddSceneObjectBase(ObjectBase* pObject);	// �I�u�W�F�N�g�ǉ�
	ObjectBase* FindSceneObject(std::string sName);	// �I�u�W�F�N�g����

	// �Փ˔���֘A
	void AddObjectCollision(ComponentCollisionBase* pCollision);	// �Փ˔���R���|�[�l���g�ǉ�
	void RemoveObjectCollision(ComponentCollisionBase* pCollision);	// �Փ˔���R���|�[�l���g�폜


	// �Q�b�^�[
	std::vector<ObjectBase*> GetAllSceneObjects();					// �V�[����ɑ��݂���S�ẴI�u�W�F�N�g���擾 
	std::vector<ObjectBase*> GetSceneObjectsTag(E_ObjectTag tag);	// �^�O�ɑΉ�����I�u�W�F�N�g���擾
private:
	void UpdateCollision();	// �Փ˔���X�V�֐�

#ifdef _DEBUG
	void InitObjectList();
	void InitObjectInfo(std::string sName);
#endif // _DEBUG
private:
	std::string CreateUniqueName(std::string sName);	// ���O���d�����Ă���ꍇ�͘A�Ԃ�t����
private:
	std::vector<std::unique_ptr<ObjectBase>>	m_pObjects;			// �V�[���ɏ�������I�u�W�F�N�g�ꗗ
	std::vector<std::unique_ptr<ObjectBase>>	m_pStandbyObjects;	// �I�u�W�F�N�g���ꎞ�I�ɕۑ����Ă����z��
	std::vector<ComponentCollisionBase*>		m_pObjectCollision;	// �e�I�u�W�F�N�g�����Փ˔���R���|�[�l���g

	bool m_bIsUpdating;	// �X�V�����ǂ���

};

#include "SceneBase.inl"