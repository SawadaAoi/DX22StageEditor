/* ========================================
	DX22Base/
	------------------------------------
	�n�ʃR���|�[�l���g�p�w�b�_
	------------------------------------
	�����F�L���������n�ʂ̃R���|�[�l���g
		�@�@����O�p�`�̒��_���
	------------------------------------
	ComponentGround.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "Vector3.h"
#include "ComponentBase.h"
#include <memory>
#include <vector>

// =============== �O���錾 =======================
class ShapeLine;
class ComponentTransform;


// =============== �N���X��` ===================
class ComponentGround :
	public ComponentBase
{
public:
	// �l�p�`��2�̎O�p�`�ɕ����������̒��_�\����
	struct T_TriangleVertex
	{
		Vector3<float> pos[3];	// ���W
	};

public:
	ComponentGround(ObjectBase* pOwner);
	~ComponentGround() {};

	void Init() override;
	void Update() override;
	void Draw() override;

	// �Q�b�^�[
	Vector3<float> GetWorldNormalDirection();				// �@�����擾
	bool GetIsDispNormal();								// �@���̕\���t���O���擾
	std::vector<T_TriangleVertex> GetTriangleVertex();	// �O�p�`�̒��_���擾

	// �Z�b�^�[
	void SetNormalDirection(const Vector3<float>& vNormal);		// �@���̌�����ݒ�
	void SetIsDispNormal(bool bIsDispNormal);					// �@���̕\���t���O��ݒ�

	DEFINE_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�


#ifdef _DEBUG
		void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	void UpdateTriangleVertex();	// �O�p�`�̒��_���X�V
private:
	ComponentTransform* m_pOwnerTransform;	// ���L�I�u�W�F�N�g�̍��W�A��]�A�X�P�[��

	std::unique_ptr<ShapeLine>			m_pNormalLine;				// �@��
	Vector3<float>						m_vNormalDirection;			// �@���̕���
	Vector3<float>						m_vWorldNormalDirection;	// ���[���h���W�̖@���̕���
	bool								m_bIsDispNormal;	// �@���̕\���t���O
	std::vector<T_TriangleVertex>		m_TriangleVertices;	// �n�ʂ��\������O�p�`�̒��_(����A�E��)
};

