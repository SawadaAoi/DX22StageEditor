/* ========================================
	DX22Base/
	------------------------------------
	�v���C���[����R���|�[�l���g�p�w�b�_
	------------------------------------
	�����F�v���C���[�𑀍삷��ׂ̃L�[����
		�@�̏������Ǘ�����
	------------------------------------
	ComponentPlayerController.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ComponentBase.h"
#include "Windows.h"

// =============== �O���錾 =======================
class ComponentTransform;
class ComponentRigidbody;

// =============== �N���X��` =====================
class ComponentPlayerController :
    public ComponentBase
{
public:
	enum E_MoveKey
	{
		Forward = 0,	// �O�i
		Back,			// ���
		Left,			// ��
		Right,			// �E
		Jump,			// �W�����v
		RotateLeft,		// ����]
		RotateRight,	// �E��]

		Max
	};

public:
	ComponentPlayerController(ObjectBase* pOwner);

	void Init() override;
	void Uninit() override;
	void Update() override;

	DEFINE_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	DebugUI::Item* CreateKeyList(std::string sName, BYTE* moveKey);
#endif // _DEBUG
private:
	 ComponentTransform* m_pCompTran;	 
	 ComponentRigidbody* m_pCompRigidbody;	

	 float m_fMoveSpeed;	// �ړ����x
	 float m_fRotateSpeed;	// ��]���x
	 float m_fJumpPower;	// �W�����v��

	 BYTE m_MoveKey[E_MoveKey::Max];		// �ړ��L�[

	 // ���͗L���t���O
	 bool m_bIsInputEnable;
};

