/* ========================================
	CatRobotGame/
	------------------------------------
	�ǃI�u�W�F�N�g�pcpp
	------------------------------------
	ObjectWall.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectWall.h"
#include "ComponentCollisionOBB.h"
#include "ComponentGeometry.h"
#include "ComponentTransform.h"
#include "TextureManager.h"

// =============== �萔��` =======================
const float WALL_THICKNESS			= 0.01f;	// �ǂ̌���
const float WALL_SCALE				= 5.0f;		// �ǂ̃X�P�[��
const float DEFAULT_WALL_ROTATE_X	= -90.0f;	// �ǂ̏�����]

const float WALL_LIGHT_DIFFUSE = 1.0f;		// �ǂ̊g�U��
const float WALL_LIGHT_AMBIENT = 1.0f;		// �ǂ̊���

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
========================================== */
ObjectWall::ObjectWall(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompColOBB(nullptr)
	, m_pCompGeometry(nullptr)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================== */
void ObjectWall::InitLocal()
{
	m_pCompColOBB = AddComponent<ComponentCollisionOBB>();
	m_pCompColOBB->SetDynamic(false);	// �ÓI�I�u�W�F�N�g�ɐݒ�

	// �g�����X�t�H�[��
	m_pCompTransform->RotateX(DEFAULT_WALL_ROTATE_X);	// �c�ɗ��Ă�
	m_pCompTransform->ScaleX(WALL_SCALE);
	m_pCompTransform->ScaleZ(WALL_SCALE);
	m_pCompTransform->ScaleY(WALL_THICKNESS);			// ����(��������)
	// �`��
	m_pCompGeometry = AddComponent<ComponentGeometry>();
	m_pCompGeometry->SetShapeType(ComponentGeometry::E_ShapeType::TYPE_PLANE);
	m_pCompGeometry->SetCulling(false);										// ���ʕ`��
	m_pCompGeometry->SetIsTex(true);
	m_pCompGeometry->SetTexture(GET_TEXTURE_DATA(TEX_KEY::WALL_FENCE));

	// ���C�g�̐ݒ�(�Â��Ȃ肷���Ȃ��悤��)
	SetLightMaterial(WALL_LIGHT_DIFFUSE, 0.0f, WALL_LIGHT_AMBIENT);
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================== */
void ObjectWall::UpdateLocal()
{
	// �����ɍ��킹�ĉ摜�����[�v������
	float fScaleX			= m_pCompTransform->GetWorldScale().x;		
	float fAdjustUvScale	= fScaleX / WALL_SCALE;
	m_pCompGeometry->SetUvScale(Vector2<float>(fAdjustUvScale, 1.0f));
}
