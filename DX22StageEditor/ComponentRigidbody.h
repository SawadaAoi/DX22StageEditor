/* ========================================
	DX22Base/
	------------------------------------
	���������R���|�[�l���g�p�w�b�_
	------------------------------------
	�����F�����������Ǘ�����R���|�[�l���g
		�@(���x�A�����x�A���ʁA�d�̗͂L��)
	------------------------------------
	ComponentRigidbody.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentBase.h"
#include "Vector3.h"

// =============== �O���錾 =======================
class ComponentTransform;
class ComponentGroundRaycast;
class ComponentCollisionOBB;

// =============== �N���X��` ===================
class ComponentRigidbody :
    public ComponentBase
{
public:
	// �͂̎��
	enum E_ForceMode
	{
		FORCE,				// �p���I�ȗ�
		IMPULSE,			// �u�ԓI�ȗ�
	};

public:
    ComponentRigidbody(ObjectBase* pOwner);
	~ComponentRigidbody() {};
	void Init() override;
	void Update() override;

	void AddForce(const Vector3<float>& vForce, E_ForceMode eMode = E_ForceMode::FORCE);

    // �Z�b�^�[
    void SetVelocity(const Vector3<float>& vVelocity);
	void SetAcceleration(const Vector3<float>& vAcceleration);
	void SetMass(float fMass);
	void SetUseGravity(bool bUseGravity);
	void SetAirDrag(float fAirDrag);
	void SetGroundDrag(float fGroundDrag);

	// �Q�b�^�[
	Vector3<float> GetVelocity() const;
	Vector3<float> GetAcceleration() const;
	float GetMass() const;
	bool GetUseGravity() const;

	DEFINE_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�

#ifdef _DEBUG
		void Debug(DebugUI::Window& window) override;
#endif // _DEBUG

private:
	void CheckRaycastGround();		// �n�ʐڐG����
	void ResolveOverlapCollision();	// �Փˎ��̂߂荞�݉���
private:
	ComponentTransform*		m_pCompTransform;		// ���W�A��]�A�X�P�[��
	ComponentGroundRaycast* m_pCompGroundRay;		// �n�ʐڐG����
	ComponentCollisionOBB*	m_pCompCollisionOBB;	// �Փ˔���(OBB)

    Vector3<float>			m_vVelocity;		// ���x
    Vector3<float>			m_vAcceleration;	// �����x
	float					m_fMass;			// ����
	bool					m_bUseGravity;		// �d�̗͂L��
	
	float m_fAirDrag;		// ��C��R
	float m_fGroundDrag;	// �n�ʖ��C

	bool					m_bIsGround;			// �n�ʂɐڐG���Ă��邩�ǂ���
	float					m_fGroundCheckDelay;	// �n�ʐڐG����̒x������

#ifdef _DEBUG
	Vector3<float>			m_vDispAccel;		// �����x�\��
#endif // _DEBUG


};

