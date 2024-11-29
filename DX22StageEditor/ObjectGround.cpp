/* ========================================
	DX22Base/
	------------------------------------
	�n�ʃI�u�W�F�N�g�pcpp
	------------------------------------
	ObjectGround.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectGround.h"
#include "ComponentTransform.h"
#include "ComponentGeometry.h"
#include "ColorVec3.h"
#include "ComponentGround.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[��
========================================== */
ObjectGround::ObjectGround(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pTransform(nullptr)
	, m_pGeometry(nullptr)
{
	m_eTag = E_ObjectTag::Ground;
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================== */
void ObjectGround::InitLocal()
{
	m_pTransform =GetComponent<ComponentTransform>();
	m_pGeometry = AddComponent<ComponentGeometry>();
	AddComponent<ComponentGround>();

	m_pGeometry->SetShapeType(ComponentGeometry::TYPE_PLANE);
	//m_pTransform->SetLocalScale(Vector3(5.0f, 0.0f, 5.0f));
	//m_pTransform->RotateZ(10.0f);	// ��]

}


/* ========================================
	�I���֐�
	-------------------------------------
	���e�F�I������
========================================== */
void ObjectGround::UninitLocal()
{
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================== */
void ObjectGround::UpdateLocal()
{



}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
========================================== */
void ObjectGround::DrawLocal()
{
}
