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
class ComponentModelAnime;
class ObjectCameraPlayer;
class ComponentSmokeEffect;

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
	float GetJumpPower();
	bool GetUseJump();
	Vector3<float> GetMoveDir();

	// �Z�b�^�[
	void SetMoveSpeed(float fMoveSpeed);
	void SetRotateSpeed(float fRotateSpeed);
	void SetJumpPower(float fJumpPower);
	void SetUseJump(bool bUseJump);
	void SetInputEnable(bool bEnable);

	DEFINE_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	DebugUI::Item* CreateKeyList(std::string sName, BYTE* moveKey);
#endif // _DEBUG
private:
	bool CheckComponent();	// �R���|�[�l���g�ݒ�
	void Move();	// �ړ�����
	void Jump();	// �W�����v����
	void Shot();	// �e���ˏ���

	Vector3<float> MoveKeybord();	// �L�[�{�[�h���͏���
	Vector3<float> MoveController();	// �R���g���[���[���͏���

	// �ړ������ɉ�]����
	void RotateToMoveDir(Vector3<float> moveDir);

	void MoveAnime(Vector3<float> vMoveDir);	// �ړ��A�j���[�V����
private:
	 ComponentTransform*	m_pCompTran;	 
	 ComponentRigidbody*	m_pCompRigidbody;	
	 ObjectCameraPlayer*	m_pObjCamera;
	 ComponentModelAnime*	m_pCompModelAnime;
	 ComponentSmokeEffect*		m_pCompSmoke;

	 float m_fMoveSpeed;	// �ړ����x
	 float m_fRotateSpeed;	// ��]���x
	 float m_fJumpPower;	// �W�����v��

	 BYTE m_MoveKey[E_MoveKey::Max];		// �ړ��L�[
	 bool m_bUseJump;	// �W�����v���t���O
	 
	 // ���͗L���t���O
	 bool m_bIsInputEnable;

	 bool m_bShot;	// �V���b�g�t���O(�A�˖h�~�p)

	 Vector3<float> m_vMoveDir;	// �ړ�����
};

