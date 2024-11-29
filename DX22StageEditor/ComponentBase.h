/* ========================================
	DX22Base/
	------------------------------------
	�R���|�[�l���g�p�w�b�_
	------------------------------------
	�����F�e�R���|�[�l���g�̊��N���X
	------------------------------------
	ComponentBase.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "TimeManager.h"	// �o�ߎ��Ԃ��ǂ��ł��擾�ł���悤�ɂ��邽��
#include "DebugConsole.h"	// �f�o�b�O�R���\�[���o�͗p
#include "DebugMenu.h"		// �f�o�b�O���j���[�o�͗p
#include "Vector3.h"
#include "Vector2.h"
#include "ComponentOrder.h"

// =============== �O���錾 =======================
class ObjectBase;	// �I�u�W�F�N�g���N���X�@��m_pOwnerObj���g�p����ۂ�cpp��ObjectBase.h���C���N���[�h����

// =============== �萔��` =======================

// �R���|�[�l���gID�擾�֐��̒�`���ȗ����邽�߂̃}�N��
// �g�����F�N���X��`����DEFINE_COMPONENT_TYPE���L�q����
// ��FDEFINE_COMPONENT_TYPE(ComponentTransform);
// �֐��̃|�C���^��ID�Ƃ��Ĉ������߁A�R���|�[�l���g�̎�ނɂ���ĈقȂ�ID���擾�ł���
#define DEFINE_COMPONENT_TYPE											\
    static size_t GetStaticTypeID() {									\
        static const size_t typeID = reinterpret_cast<size_t>(&typeID); \
        return typeID;													\
    }																	\
    size_t GetTypeID() const override {									\
        return GetStaticTypeID();										\
    }

// =============== �N���X��` =====================
class ComponentBase
{
public:
	ComponentBase() = delete;						// �f�t�H���g�R���X�g���N�^�͋֎~
	ComponentBase(ObjectBase* pOwner, int nOrder);	// �R���X�g���N�^(���L�҃I�u�W�F�N�g��ݒ�)
	virtual ~ComponentBase() {};
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	
	// �Q�b�^�[
	ObjectBase* GetOwnerObject();		// ���L�҃I�u�W�F�N�g�̎擾
	int GetUpdateOrder();				// �X�V�����̎擾
	virtual size_t GetTypeID() const;	// �R���|�[�l���g�̎��ID�̎擾
	static size_t GetStaticTypeID();	// �R���|�[�l���g�̎��ID�̎擾(�ÓI�֐�)

#ifdef _DEBUG
	// �f�o�b�O�p�̏���(�I�u�W�F�N�g���E�B���h�E�ɕ\��)
	virtual void Debug(DebugUI::Window& window) {};
#endif // _DEBUG

protected:
	ObjectBase*	m_pOwnerObj;	// ���L�҃I�u�W�F�N�g
	int			m_nUpdateOrder;	// �X�V����(���l���������قǐ�ɍX�V)


};

