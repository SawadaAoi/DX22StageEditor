/* ========================================
	DX22Base/
	------------------------------------
	UI�p���W�ϊ��R���|�[�l���g�p�w�b�_
	------------------------------------
	�����F�ʒu�E��]�E�X�P�[���Ǘ����s��
	------------------------------------
	UIComponentTransform.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "UIComponentBase.h"
#include "Vector2.h"	// 2�����x�N�g���N���X

// =============== �N���X��` =====================
class UIComponentTransform :
    public UIComponentBase
{
public:
	UIComponentTransform(UIObjectBase* pOwner);

	void Init() override;
	void Update() override;
	void UpdateWorldTransform();	// ���[���h�ϊ��̍X�V
	void ClearParent();			// �e�I�u�W�F�N�g���폜

	// ���W�ϊ�
	void Translate(const Vector2<float>& vPos);	// �ړ�
	void Translate(float x, float y);			// �ړ�
	void TranslateX(float x);					// X���ړ�
	void TranslateY(float y);					// Y���ړ�

	// ��]�ϊ�
	void Rotate(float fRot);					// ��]

	// �X�P�[���ϊ�
	void Scale(const Vector2<float>& vScale);	// �X�P�[��
	void Scale(float x, float y);				// �X�P�[��
	void ScaleX(float x);						// X���X�P�[��
	void ScaleY(float y);						// Y���X�P�[��

	// �Q�b�^�[
	Vector2<float> GetLocalPosition();	// ���[�J�����W�̎擾	
	Vector2<float> GetWorldPosition();	// ���[���h���W�̎擾
	float GetLocalRotation();			// ���[�J����]�̎擾
	float GetWorldRotation();			// ���[���h��]�̎擾
	Vector2<float> GetLocalScale();		// ���[�J���X�P�[���̎擾
	Vector2<float> GetWorldScale();		// ���[���h�X�P�[���̎擾

	// �Z�b�^�[
	void SetLocalPosition(Vector2<float> vPos);	// ���[�J�����W�̐ݒ�
	void SetWorldPosition(Vector2<float> vPos);	// ���[���h���W�̐ݒ�
	void SetLocalRotation(float fRot);			// ���[�J����]�̐ݒ�
	void SetWorldRotation(float fRot);			// ���[���h��]�̐ݒ�
	void SetLocalScale(Vector2<float> vScale);	// ���[�J���X�P�[���̐ݒ�
	void SetWorldScale(Vector2<float> vScale);	// ���[���h�X�P�[���̐ݒ�

	DEFINE_UI_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	// ���[�J���ϊ�
	Vector2<float>	m_vLocalPosition;	// �ʒu
	float			m_fLocalRotation;	// ��]
	Vector2<float>	m_vLocalScale;		// �X�P�[��

	// ���[���h�ϊ�
	Vector2<float>	m_vWorldPosition;	// �ʒu
	float			m_fWorldRotation;	// ��]
	Vector2<float>	m_vWorldScale;		// �X�P�[��
};

