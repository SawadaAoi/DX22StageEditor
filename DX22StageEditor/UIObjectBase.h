/* ========================================
	DX22Base/
	------------------------------------
	UI�I�u�W�F�N�g���p�w�b�_
	------------------------------------
	�����FUI�I�u�W�F�N�g�̊��N���X
	------------------------------------
	UIObjectBase.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "vector"
#include <string>
#include "UIComponentBase.h"	// UI�R���|�[�l���g���N���X
#include <memory>			// std::unique_ptr�p
#include "TimeManager.h"	// �o�ߎ��Ԃ��ǂ��ł��擾�ł���悤�ɂ��邽��


// =============== �O���錾 =====================
class SceneBase;	// �V�[�����N���X

// =============== �萔��` =======================
// �I�u�W�F�N�gID�擾�֐��̒�`���ȗ����邽�߂̃}�N��
// �g�����F�N���X��`����DEFINE_OBJECT_TYPE(�N���X��)���L�q����
// ��FDEFINE_OBJECT_TYPE(ObjectPlayer);
// �֐��̃|�C���^��ID�Ƃ��Ĉ������߁A�R���|�[�l���g�̎�ނɂ���ĈقȂ�ID���擾�ł���
#define DEFINE_UI_OBJECT_TYPE											\
    static size_t GetStaticTypeID() {									\
		static const size_t typeID = reinterpret_cast<size_t>(&typeID); \
        return typeID;													\
    }																	\
    size_t GetTypeID() const override {									\
        return GetStaticTypeID();										\
    }


// =============== �N���X��` =====================
class UIObjectBase
{
public:
	UIObjectBase() = delete;
	UIObjectBase(SceneBase* pScene);
	virtual ~UIObjectBase() {};

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

	// �e�q�֌W
	void SetParentObject(UIObjectBase* pParentObj);		// �e�I�u�W�F�N�g�̐ݒ�
	void AddChildObject(UIObjectBase* pChildObj);		// �q�I�u�W�F�N�g�̒ǉ�
	void RemoveParentObject();							// �e�I�u�W�F�N�g�̍폜
	void RemoveChildObject(UIObjectBase* pChildObj);	// �q�I�u�W�F�N�g�̍폜
	void RemoveAllChildObjects();						// �S�Ă̎q�I�u�W�F�N�g�̍폜
	int GetGenerationCount();							// �q�I�u�W�F�N�g�̐����擾

	// �Q�b�^�[
	SceneBase* GetOwnerScene() const;					// ���L�V�[���̎擾
	virtual size_t GetTypeID() const;					// �R���|�[�l���g�̎��ID�̎擾
	static size_t GetStaticTypeID();					// �R���|�[�l���g�̎��ID�̎擾
	std::string GetName() const;						// �I�u�W�F�N�g���̎擾
	UIObjectBase* GetParentObject() const;				// �e�I�u�W�F�N�g�̎擾
	std::vector<UIObjectBase*> GetChildObjects() const;	// �q�I�u�W�F�N�g�̎擾

	// �Z�b�^�[
	void SetName(std::string sName);	// �I�u�W�F�N�g���̐ݒ�

	// �R���|�[�l���g�֘A
	template<typename T>
	T* AddComponent();	// �R���|�[�l���g�ǉ�

	template<typename T>
	T* GetComponent();	// �R���|�[�l���g�擾

	template<typename T>
	void RemoveComponent();	// �R���|�[�l���g�폜

#ifdef _DEBUG
	void Debug();					// �f�o�b�O�p�̏���(�I�u�W�F�N�g���E�B���h�E�ɕ\��)
	virtual void DebugLocal() {};	// �ʃf�o�b�O����

	std::string GetListName();	// ���X�g�\���p�̖��O�擾
#endif

private:
	void InitDefaultComponent();	// �f�t�H���g�R���|�[�l���g�ݒ�

protected:
	SceneBase*										m_pOwnerScene;	// ���L�V�[��
	std::vector<std::unique_ptr<UIComponentBase>>	m_pComponents;	// �R���|�[�l���g�ꗗ

	std::string m_sName;		// �I�u�W�F�N�g��
	
	// �e�q�֌W
	UIObjectBase*				m_pParentObj;	// �e�I�u�W�F�N�g
	std::vector<UIObjectBase*>	m_pChildObjs;	// �q�I�u�W�F�N�g
};

#include "UIObjectBase.inl"	// �R���|�[�l���g�֘A�̃e���v���[�g�֐��̎���