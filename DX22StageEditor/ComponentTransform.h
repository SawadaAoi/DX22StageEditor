/* ========================================
	DX22Base/
	------------------------------------
	�R���|�[�l���g(���W�ϊ�)�p�w�b�_
	------------------------------------
	�����F�ʒu�E��]�E�X�P�[���Ǘ����s��
	------------------------------------
	ComponentTransform.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ComponentBase.h"
#include "Vector3.h"
#include "Quaternion.h"	// ��]


// =============== �N���X��` =====================
class ComponentTransform :
    public ComponentBase
{
public:
	ComponentTransform(ObjectBase* pOwner);

    void Init() override;
	void Update() override;
	void UpdateWorldTransform();	// ���[���h�ϐ��̍X�V
	void ClearParent();			// �e�I�u�W�F�N�g���폜

	// ���W�ϊ�
	void Translate(const Vector3<float>& translation);
	void Translate(float x, float y, float z);
	void TranslateX(float x);
	void TranslateY(float y);
	void TranslateZ(float z);

	// ��]�ϊ�
	void Rotate(const Quaternion& rotation);
	void Rotate(Vector3<float> axis, float angle);	
	void Rotate(const Vector3<float>& angles);
	void Rotate(float x, float y, float z);
	void RotateX(float degAngle);
	void RotateY(float degAngle);
	void RotateZ(float degAngle);

	// �X�P�[���ϊ�
	void Scale(const Vector3<float>& scale);
	void Scale(float x, float y, float z);
	void ScaleX(float x);	
	void ScaleY(float y);
	void ScaleZ(float z);

	// ���̑��֗��֐�
	void LookAt(const Vector3<float>& target, const Vector3<float>& up = Vector3<float>::Up());
	void MoveTo(const Vector3<float>& target, float fTime);
	void MoveToward(const Vector3<float>& target, float fSpeed, float fDistance = 0.0f);
	Vector3<float> GetForwardVector() const;
	Vector3<float> GetRightVector() const;
	Vector3<float> GetUpVector() const;

    // �Q�b�^�[
    Vector3<float> GetLocalPosition() const;
	Vector3<float> GetWorldPosition() const;
	Vector3<float> GetLocalRotationEuler() const;
	Vector3<float> GetWorldRotationEuler() const;
	Quaternion GetLocalRotation() const;
	Quaternion GetWorldRotation() const;
	Vector3<float> GetLocalScale() const;
	Vector3<float> GetWorldScale() const;

	DirectX::XMFLOAT4X4 GetWorldMatrix() const;
	DirectX::XMFLOAT4X4 GetWorldMatrixOffset(
		const Vector3<float>& vPos = Vector3<float>::Zero(), 
		const Quaternion& qRot = Quaternion(), 
		const Vector3<float>& vScale = {1.0f,1.0f,1.0f}) const;

	DEFINE_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�

	// �Z�b�^�[
	void SetLocalPosition(const Vector3<float>& position);
	void SetLocalRotation(const Quaternion& rotation);
	void SetLocalRotation(const Vector3<float> axis, float degAngle);
	void SetLocalRotationEuler(const Vector3<float>& angles);
	void SetLocalScale(const Vector3<float>& scale);

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	void CallbackRotation(bool bIsWrite, void* arg, Quaternion& qRotation);
#endif // _DEBUG

private:
	void UpdateMoveTo();	// MoveTo�̍X�V
private:
	// ���[�J��
    Vector3<float>	m_vLocalPosition;	// ���W
	Quaternion		m_qLocalRotation;	// ��]
	Vector3<float>	m_vLocalScale;		// �X�P�[��
	
	// ���[���h(�e�I�u�W�F�N�g���l���������W)
	Vector3<float>	m_vWorldPosition;	// ���W
	Quaternion		m_qWorldRotation;	// ��]
	Vector3<float>	m_vWorldScale;		// �X�P�[��

	// MoveTo�p
	Vector3<float>	m_vMoveStartPos;	// �ړ��J�n���W
	Vector3<float>	m_vMoveEndPos;		// �ړ��I�����W
	float			m_fMoveTime;		// �ړ�����
	float			m_fMoveCurrentTime;	// ���݂̈ړ�����
	bool			m_bMoveTo;			// �ړ������ǂ���
};

