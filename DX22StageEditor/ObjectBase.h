/* ========================================
	DX22Base/
	------------------------------------
	�I�u�W�F�N�g�p�w�b�_
	------------------------------------
	�����F�e�I�u�W�F�N�g�̊��N���X
	------------------------------------
	ObjectBase.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "vector"
#include "ComponentBase.h"	// �R���|�[�l���g���N���X
#include "TimeManager.h"	// �o�ߎ��Ԃ��ǂ��ł��擾�ł���悤�ɂ��邽��
#include "DebugConsole.h"	// �f�o�b�O�R���\�[���o�͗p
#include <memory>			// std::unique_ptr�p
#include "ObjectTag.h"

// =============== �O���錾 =====================
class SceneBase;	// �V�[�����N���X


// =============== �萔��` =======================

// �I�u�W�F�N�gID�擾�֐��̒�`���ȗ����邽�߂̃}�N��
// �g�����F�N���X��`����DEFINE_OBJECT_TYPE(�N���X��)���L�q����
// ��FDEFINE_OBJECT_TYPE(ObjectPlayer);
// �֐��̃|�C���^��ID�Ƃ��Ĉ������߁A�R���|�[�l���g�̎�ނɂ���ĈقȂ�ID���擾�ł���
#define DEFINE_OBJECT_TYPE(name)												\
    static size_t GetStaticTypeID() {									\
		static const size_t typeID = reinterpret_cast<size_t>(&typeID); \
		return typeID;													\
    }																	\
    size_t GetTypeID() const override {									\
        return GetStaticTypeID();										\
    }																	\
	std::string GetObjClassName() const override {						\
		return #name;													\
	} 

// =============== �N���X��` =====================
class ObjectBase
{
public:

	// �I�u�W�F�N�g�̏��
	enum E_State
	{
		STATE_ACTIVE,	// �A�N�e�B�u
		STATE_PAUSE,	// �ꎞ��~
		STATE_DEAD,		// ���S

		STATE_MAX,		// ��Ԃ̍ő吔
	};

public:
	ObjectBase() = delete;			// �f�t�H���g�R���X�g���N�^�͋֎~
	ObjectBase(SceneBase* pScene);	// �R���X�g���N�^(�V�[����ݒ�)
	virtual ~ObjectBase() {};

	// �������ς��Ȃ��֐�
	void Init(std::string sName);
	void Uninit();
	void Update();
	void Draw();

	// �q�N���X�Ŏ�������֐�
	virtual void InitLocal() {};	// �ʏ���������
	virtual void UninitLocal() {};	// �ʏI������
	virtual void UpdateLocal() {};	// �ʍX�V����
	virtual void DrawLocal() {};	// �ʕ`�揈��

	virtual void OnCollisionEnter(ObjectBase* pHit);	// �ՓˊJ�n����
	virtual void OnCollisionStay(ObjectBase* pHit);		// �Փ˒�����
	virtual void OnCollisionExit(ObjectBase* pHit);		// �ՓˏI������

	// �e�q�֌W
	void SetParentObject(ObjectBase* pParentObj);	// �e�I�u�W�F�N�g�ݒ�
	void AddChildObject(ObjectBase* pChildObj);		// �q�I�u�W�F�N�g�ǉ�
	void RemoveParentObject();						// �e�I�u�W�F�N�g����
	void RemoveChildObject(ObjectBase* pChildObj);	// �q�I�u�W�F�N�g����
	void RemoveAllChildObjects();					// �S�q�I�u�W�F�N�g����
	int GetGenerationCount();						// ���㐔�̎擾

	// �Q�b�^�[
	SceneBase* GetOwnerScene() const;					// ���L�V�[���̎擾
	E_State GetState() const;							// ��Ԃ̎擾
	virtual size_t GetTypeID() const;					// �R���|�[�l���g�̎��ID�̎擾
	static size_t GetStaticTypeID();					// �R���|�[�l���g�̎��ID�̎擾(�ÓI)
	virtual std::string GetObjClassName() const;		// �N���X���̎擾
	ObjectBase* GetParentObject() const;				// �e�I�u�W�F�N�g�̎擾
	std::vector<ObjectBase*> GetChildObjects() const;	// �q�I�u�W�F�N�g�̎擾
	E_ObjectTag GetTag() const;							// �^�O�̎擾
	std::string GetName() const;						// �I�u�W�F�N�g���̎擾

	// �Z�b�^�[
	void SetState(E_State eState);		// ��Ԃ̐ݒ�
	void SetTag(E_ObjectTag eTag);		// �^�O�̐ݒ�
	void SetName(std::string sName);	// �I�u�W�F�N�g���̐ݒ�

#ifdef _DEBUG
	void Debug();					// �f�o�b�O�p�̏���(�I�u�W�F�N�g���E�B���h�E�ɕ\��)
	virtual void DebugLocal() {};	// �ʃf�o�b�O����
	void ChangeName();	// ���O�ύX

	std::string GetListName();	// ���X�g�\���p�̖��O�擾
#endif

	// �R���|�[�l���g�֘A
	template<typename T>
	T* AddComponent();	// �R���|�[�l���g�ǉ�

	template<typename T>
	T* GetComponent();	// �R���|�[�l���g�擾

	template<typename T>
	void RemoveComponent();	// �R���|�[�l���g�폜

private:
	void InitDefaultComponent();	// �f�t�H���g�R���|�[�l���g�ݒ�

protected:
	SceneBase* m_pOwnerScene;	// ���L�V�[��
	std::vector<std::unique_ptr<ComponentBase>> m_pComponents;	// �R���|�[�l���g�ꗗ

	std::string m_sName;	// �I�u�W�F�N�g��
	E_State		m_eState;	// �I�u�W�F�N�g�̏��
	E_ObjectTag m_eTag;		// �I�u�W�F�N�g�̕���

	// �e�q�֌W
	ObjectBase*					m_pParentObj;	// �e�I�u�W�F�N�g
	std::vector<ObjectBase*>	m_pChildObjs;	// �q�I�u�W�F�N�g

};

#include "ObjectBase.inl"
