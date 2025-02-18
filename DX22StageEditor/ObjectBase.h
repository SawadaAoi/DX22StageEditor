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
#include <fstream>			// �t�@�C�����o�͗p

// =============== �O���錾 =====================
class SceneBase;	// �V�[�����N���X
class ComponentTransform;	// �g�����X�t�H�[���R���|�[�l���g

// =============== �萔��` =======================

// �I�u�W�F�N�gID�擾�֐��̒�`���ȗ����邽�߂̃}�N��
// �g�����F�N���X��`����DEFINE_OBJECT_TYPE(�N���X��)���L�q����
// ��FDEFINE_OBJECT_TYPE(ObjectPlayer);
// �֐��̃|�C���^��ID�Ƃ��Ĉ������߁A�R���|�[�l���g�̎�ނɂ���ĈقȂ�ID���擾�ł���
#define DEFINE_OBJECT_TYPE(name)										\
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

	// ���C�g�p�����[�^
	struct T_LightParam
	{
		float fDiffuse;		// �g�U���˒l
		float fSpecular;	// ���ʔ��˒l
		float fAmbient;		// �����l
		bool  bLightUse;	// ���C�g�g�p�t���O
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

	// �I�u�W�F�N�g�̔j��
	void Destroy(float nTime = 0.0f);

	// �Փˏ���
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
	bool CheckIsDescendant(ObjectBase* pObject);	// �q�����ǂ����̔���

	// �R�s�[�֐�
	ObjectBase* Copy();								// �I�u�W�F�N�g�̃R�s�[
	virtual void CopyLocal(ObjectBase* pObject);	// �I�u�W�F�N�g�̃R�s�[(�ʏ���)

	virtual void OutPutLocalData(std::ofstream& file);	// �I�u�W�F�N�g�̃f�[�^�o��
	virtual void InputLocalData(std::ifstream& file);	// �I�u�W�F�N�g�̃f�[�^����

	// �Q�b�^�[
	ComponentTransform* GetTransform() const;			// �g�����X�t�H�[���R���|�[�l���g�̎擾
	SceneBase* GetOwnerScene() const;					// ���L�V�[���̎擾
	E_State GetState() const;							// ��Ԃ̎擾
	virtual size_t GetTypeID() const;					// �R���|�[�l���g�̎��ID�̎擾
	static size_t GetStaticTypeID();					// �R���|�[�l���g�̎��ID�̎擾(�ÓI)
	virtual std::string GetObjClassName() const;		// �N���X���̎擾
	ObjectBase* GetParentObject() const;				// �e�I�u�W�F�N�g�̎擾
	std::vector<ObjectBase*> GetChildObjects() const;	// �q�I�u�W�F�N�g�̎擾
	E_ObjectTag GetTag() const;							// �^�O�̎擾
	std::string GetName() const;						// �I�u�W�F�N�g���̎擾
	T_LightParam GetLightMaterial() const;				// ���C�g�p�����[�^�̎擾
	bool GetIsSave() const;								// �Z�[�u���邩�ǂ����̎擾
	bool GetIsFold() const;								// �I�u�W�F�N�g�ꗗ�܂肽���݃t���O�̎擾

	// �Z�b�^�[
	void SetState(E_State eState);		// ��Ԃ̐ݒ�
	void SetTag(E_ObjectTag eTag);		// �^�O�̐ݒ�
	void SetName(std::string sName);	// �I�u�W�F�N�g���̐ݒ�
	void SetLightMaterial(float fDiffuse = 1.0f, float fSpecular = 0.0f, float fAmbient = 0.3f);	// ���C�g�p�����[�^�̐ݒ�
	void SetLightUse(bool bUse);		// ���C�g�g�p�̐ݒ�
	void SetIsSave(bool bIsSave);		// �Z�[�u���邩�ǂ����̐ݒ�
	void SetIsFold(bool bIsFold);		// �I�u�W�F�N�g�ꗗ�܂肽���݃t���O�̐ݒ�


	// �R���|�[�l���g�֘A
	template<typename T>
	T* AddComponent();	// �R���|�[�l���g�ǉ�

	template<typename T>
	T* GetComponent();	// �R���|�[�l���g�擾

	template<typename T>
	void RemoveComponent();	// �R���|�[�l���g�폜

#ifdef _DEBUG
	void Debug();					// �f�o�b�O�p�̏���(�I�u�W�F�N�g���E�B���h�E�ɕ\��)
	virtual void DebugLocal(DebugUI::Window& window) {};	// �ʃf�o�b�O����

	DebugUI::Item* InitParentList();	// �e���X�g�̏�����

	void ChangeName();			// ���O�ύX
	void ChangeParentList(std::string sParentName);		// �e�ύX

	std::string GetListName();	// ���X�g�\���p�̖��O�擾

#endif
private:
	void InitDefaultComponent();	// �f�t�H���g�R���|�[�l���g�ݒ�

protected:
	SceneBase* m_pOwnerScene;	// ���L�V�[��
	std::vector<std::unique_ptr<ComponentBase>> m_pComponents;	// �R���|�[�l���g�ꗗ

	ComponentTransform* m_pCompTransform;	// �g�����X�t�H�[���R���|�[�l���g

	std::string m_sName;	// �I�u�W�F�N�g��
	E_State		m_eState;	// �I�u�W�F�N�g�̏��
	E_ObjectTag m_eTag;		// �I�u�W�F�N�g�̕���

	// �e�q�֌W
	ObjectBase*					m_pParentObj;	// �e�I�u�W�F�N�g
	std::vector<ObjectBase*>	m_pChildObjs;	// �q�I�u�W�F�N�g

	T_LightParam m_tLightParam;	// ���C�g�p�����[�^

	// �Z�[�u���邩�ǂ���(���s���Ɏ��g�̎q�N���X���쐬�����I�u�W�F�N�g�̕ۑ����ɁA�q�N���X��ۑ����Ȃ��悤�ɂ��邽��)
	bool m_bIsSave;	

	// �I�u�W�F�N�g�ꗗ�܂肽���݃t���O(true:�܂肽����, false:�W�J)
	bool m_bIsFold;

	bool m_bIsDestroy;			// �I�u�W�F�N�g�j���t���O(true:�j��, false:���j��)
	float m_fDestroyTime;		// �j������
	float m_fDestroyTimeCnt;	// �j�����ԃJ�E���g
};

#include "ObjectBase.inl"
