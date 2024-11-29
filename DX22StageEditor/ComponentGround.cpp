/* ========================================
	DX22Base/
	------------------------------------
	�n�ʃR���|�[�l���g�pcpp
	------------------------------------
	ComponentGround.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ComponentGround.h"
#include "ComponentTransform.h"
#include "ShapeLine.h"
#include "ObjectBase.h"
#include "ColorVec3.h"

// =============== �萔��` =======================
const Vector3<float> NORMAL_DIRERCTION = Vector3<float>(0.0f, 1.0f, 0.0f);	// �@���̌���
const int TRIANGLE_NUM = 2;	// �O�p�`�̐�

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�I�u�W�F�N�g
=========================================== */
ComponentGround::ComponentGround(ObjectBase* pOwner)
	: ComponentBase(pOwner,OrderGround)
	, m_vNormalDirection(NORMAL_DIRERCTION)
	, m_bIsDispNormal(false)
	, m_pNormalLine(nullptr)
	, m_pOwnerTransform(nullptr)
{
	m_TriangleVertices.resize(TRIANGLE_NUM);

}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
=========================================== */
void ComponentGround::Init()
{
	// ���L�I�u�W�F�N�g�̍��W�A��]�A�X�P�[�����擾
	m_pOwnerTransform = m_pOwnerObj->GetComponent<ComponentTransform>();	

	// �@����\�����邽�߂̐���������
	m_pNormalLine = std::make_unique<ShapeLine>(Vector3<float>::Zero(), NORMAL_DIRERCTION, ColorVec3::RED);
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void ComponentGround::Update()
{
	UpdateTriangleVertex();		// �O�p�`�̒��_���X�V

	if (m_bIsDispNormal)
	{
		// �@���̈ʒu�A�����A�X�P�[�����X�V
		m_pNormalLine->SetPos(m_pOwnerTransform->GetWorldPosition());
		m_pNormalLine->SetScale(m_vNormalDirection);
		m_pNormalLine->SetRotation(m_pOwnerTransform->GetWorldRotation());
	}
}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
=========================================== */
void ComponentGround::Draw()
{
	if (m_bIsDispNormal)
	{
		// �@����`��
		m_pNormalLine->Draw();
	}
}


/* ========================================
	�O�p�`�̒��_�X�V�֐�
	-------------------------------------
	���e�F���L�I�u�W�F�N�g�̃X�P�[���A���W�ɍ��킹��
		�@�O�p�`�̒��_���X�V
=========================================== */
void ComponentGround::UpdateTriangleVertex()
{
	Vector3<float> vScaleHalf = m_pOwnerTransform->GetWorldScale() / 2.0f;
	Vector3<float> vPos = m_pOwnerTransform->GetWorldPosition();

	// �O�p�`�̒��_��ݒ�(Z���͉�����
	// 1�ڂ̎O�p�`(����A�E��A����)
	m_TriangleVertices[0].pos[0] = vPos + Vector3<float>(-vScaleHalf.x, vScaleHalf.y,  vScaleHalf.z);	// ����
	m_TriangleVertices[0].pos[1] = vPos + Vector3<float>( vScaleHalf.x, vScaleHalf.y,  vScaleHalf.z);	// �E��
	m_TriangleVertices[0].pos[2] = vPos + Vector3<float>(-vScaleHalf.x, vScaleHalf.y, -vScaleHalf.z);	// ����

	// 2�ڂ̎O�p�`(�E��A�E���A����)
	m_TriangleVertices[1].pos[0] = vPos + Vector3<float>( vScaleHalf.x, vScaleHalf.y,  vScaleHalf.z);	// �E��
	m_TriangleVertices[1].pos[1] = vPos + Vector3<float>( vScaleHalf.x, vScaleHalf.y, -vScaleHalf.z);	// �E��
	m_TriangleVertices[1].pos[2] = vPos + Vector3<float>(-vScaleHalf.x, vScaleHalf.y, -vScaleHalf.z);	// ����

	// �e�I�u�W�F�N�g�̉�]�ɍ��킹�ĎO�p�`�̒��_����]
	for (int i = 0; i < TRIANGLE_NUM; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m_TriangleVertices[i].pos[j] = m_pOwnerTransform->GetWorldRotation().Rotate(m_TriangleVertices[i].pos[j]);
		}
	}

}

/* ========================================
	�Q�b�^�[(�@���x�N�g��)�֐�
	-------------------------------------
	�ߒl�FVector3<float>	�@���x�N�g��
=========================================== */
Vector3<float> ComponentGround::GetNormalDirection()
{
	return m_vNormalDirection;
}

/* ========================================
	�Q�b�^�[(�@���\���t���O)�֐�
	-------------------------------------
	�ߒl�Fbool	�@���\���t���O
=========================================== */
bool ComponentGround::GetIsDispNormal()
{
	return m_bIsDispNormal;
}

/* ========================================
	�Q�b�^�[(�O�p�`�̒��_)�֐�
	-------------------------------------
	�ߒl�Fstd::vector<T_TriangleVertex>	�O�p�`�̒��_
=========================================== */
std::vector<ComponentGround::T_TriangleVertex> ComponentGround::GetTriangleVertex()
{
	return m_TriangleVertices;
}

/* ========================================
	�Z�b�^�[(�@���x�N�g��)�֐�
	-------------------------------------
	�����FVector3<float> vNormal	�@���x�N�g��
=========================================== */
void ComponentGround::SetNormalDirection(const Vector3<float>& vNormal)
{
	m_vNormalDirection = vNormal;
}

/* ========================================
	�Z�b�^�[(�@���\���t���O)�֐�
	-------------------------------------
	�����Fbool bIsDispNormal	�@���\���t���O
=========================================== */
void ComponentGround::SetIsDispNormal(bool bIsDispNormal)
{
	m_bIsDispNormal = bIsDispNormal;
}

#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentGround::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupGround = Item::CreateGroup("Ground");

	pGroupGround->AddGroupItem(Item::CreateBind("NormalDirection",	Item::Kind::Vector, &m_vNormalDirection));	// �@������
	pGroupGround->AddGroupItem(Item::CreateBind("IsDispNormal",		Item::Kind::Bool, &m_bIsDispNormal));		// �@���\���t���O

	// �O�p�`�̒��_
	for (int i = 0; i < TRIANGLE_NUM; i++)
	{
		std::string sTriangle = "Triangle" + std::to_string(i);
		pGroupGround->AddGroupItem(Item::CreateValue(sTriangle.c_str(), Item::Kind::Label));	// ���o��(�A��)

		// ���_�������[�v
		for (int j = 0; j < 3; j++)
		{
			std::string sVertex = "Vertex" + std::to_string(j);
			pGroupGround->AddGroupItem(Item::CreateBind(sVertex.c_str(), Item::Kind::Vector, &m_TriangleVertices[i].pos[j]));
		}

	}

	window.AddItem(pGroupGround);
}

#endif // _DEBUG