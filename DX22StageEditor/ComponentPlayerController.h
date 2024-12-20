/* ========================================
	CatRobotGame/
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
#include <unordered_map>

// =============== �O���錾 =======================
class ComponentTransform;
class ComponentRigidbody;
class ObjectCameraPlayer;

// =============== �N���X��` =====================
class ComponentPlayerController :
    public ComponentBase
{
public:
	enum E_MoveKey
	{
		FORWARD = 0,	// �O�i
		BACK,			// ���
		LEFT,			// ��
		RIGHT,			// �E
		JUMP,			// �W�����v
		SHOT,			// �V���b�g
		Max
	};

public:
	ComponentPlayerController(ObjectBase* pOwner);

	void Init() override;
	void Uninit() override;
	void Update() override;

	// �Q�b�^�[
	float GetMoveSpeed();
	float GetRotateSpeed();
	bool GetUseJump();

	// �Z�b�^�[
	void SetMoveSpeed(float fMoveSpeed);
	void SetRotateSpeed(float fRotateSpeed);
	void SetUseJump(bool bUseJump);

	DEFINE_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	DebugUI::Item* CreateKeyList(std::string sName, BYTE* moveKey);
#endif // _DEBUG
private:
	void Move();	// �ړ�����
	void Jump();	// �W�����v����
	void Shot();	// �e���ˏ���

	Vector3<float> MoveKeybord();	// �L�[�{�[�h���͏���
	Vector3<float> MoveController();	// �R���g���[���[���͏���

	// �ړ������ɉ�]����
	void RotateToMoveDir(Vector3<float> moveDir);
private:
	 ComponentTransform* m_pCompTran;	 
	 ComponentRigidbody* m_pCompRigidbody;	
	 ObjectCameraPlayer* m_pObjCamera;

	 float m_fMoveSpeed;	// �ړ����x
	 float m_fRotateSpeed;	// ��]���x
	 float m_fJumpPower;	// �W�����v��

	 BYTE m_MoveKey[E_MoveKey::Max];		// �ړ��L�[
	 bool m_bUseJump;	// �W�����v���t���O
	 
	 // ���͗L���t���O
	 bool m_bIsInputEnable;


};

