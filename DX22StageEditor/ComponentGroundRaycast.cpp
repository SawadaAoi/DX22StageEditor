/* ========================================
	CatRobotGame/
	------------------------------------
	�n�ʐڐG����R���|�[�l���g�pcpp
	------------------------------------
	ComponentGroundRaycast.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ComponentGroundRaycast.h"

#include "ColorVec3.h"
#include "ComponentGround.h"	// �n�ʃI�u�W�F�N�g�����@�����擾���邽��
#include "ComponentGroundBox.h"
#include "ComponentTransform.h"
#include "ObjectGround.h"		// �ڐG�Ώۂ̒n�ʃI�u�W�F�N�g
#include "SceneBase.h"
#include <vector>

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�I�u�W�F�N�g
=========================================== */
ComponentGroundRaycast::ComponentGroundRaycast(ObjectBase* pOwner)
	: ComponentBase(pOwner,OrderGroundRayCast)
	, m_vStartPos(0.0f, 0.0f, 0.0f)
	, m_vStartPosOffset(0.0f, 0.0f, 0.0f)
	, m_vDirection(0.0f, -1.0f, 0.0f)
	, m_fRayLength(1.0f)
	, m_pOwnerTransform(nullptr)
	, m_pRayLine(nullptr)
	, m_vHitPos(0.0f, 0.0f, 0.0f)
	, m_sHitObjName("")
#ifdef _DEBUG
	, m_bIsHit(false)
	, m_bIsDispRayLine(false)
	, m_bIsDispTriPointToHitPos(false)
#else
	, m_bIsHit(false)
	, m_bIsDispRayLine(false)
	, m_bIsDispTriPointToHitPos(false)
#endif // _DEBUG
{

}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
=========================================== */
void ComponentGroundRaycast::Init()
{
	m_pOwnerTransform	= m_pOwnerObj->GetComponent<ComponentTransform>();
	m_vStartPos			= m_pOwnerTransform->GetPosition() + m_vStartPosOffset;
	m_pRayLine			= std::make_unique<ShapeLine>(m_vStartPos, m_vStartPos + (m_vDirection * m_fRayLength));
	for (int i = 0; i < 3; i++)
	{
		m_pLines_TriPointToHitPos.push_back(std::make_unique<ShapeLine>(Vector3<float>(0.0f, 0.0f, 0.0f), Vector3<float>(0.0f, 0.0f, 0.0f)));
	}
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void ComponentGroundRaycast::Update()
{
	m_vStartPos = m_pOwnerTransform->GetPosition() + m_vStartPosOffset;
	m_pRayLine->UpdateLine(1, m_vStartPos, m_vStartPos + (m_vDirection * m_fRayLength), ColorVec3::RED);

	CheckGround();


#ifdef _DEBUG
	// �ڐG����\���p
	if (m_bIsHit)
	{
		m_sDispIsHit = "True";
	}
	else
	{
		m_sDispIsHit = "False";
	}


#endif // _DEBUG
}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
=========================================== */
void ComponentGroundRaycast::Draw()
{
	if (m_bIsDispRayLine) 
	{
		m_pRayLine->Draw();	// ���C�`��
	}

	if (m_bIsDispTriPointToHitPos)
	{
		for (auto& pLine : m_pLines_TriPointToHitPos)
		{
			pLine->Draw();	// ���O�`�F�b�N�p���`��
		}
	}

	
}


/* ========================================
	�n�ʔ��菈���֐�
	-------------------------------------
	���e�F�V�[����ɑ��݂���n�ʃI�u�W�F�N�g
		�@�Ǝ��g�̃��C�Ƃ̓����蔻����s��
=========================================== */
void ComponentGroundRaycast::CheckGround()
{
	// �V�[����ɑ��݂���n�ʃI�u�W�F�N�g���擾
	std::vector<ObjectBase*> vObjects;
	vObjects = m_pOwnerObj->GetOwnerScene()->GetSceneObjectsTag(E_ObjectTag::Ground);

	for (auto pObject : vObjects)
	{
		ComponentTransform* pPlaneTran	= pObject->GetComponent<ComponentTransform>();
		ComponentGround*	pGround		= pObject->GetComponent<ComponentGround>();

		// �n�ʃR���|�[�l���g���Ȃ��ꍇ�͒n�ʃ{�b�N�X�R���|�[�l���g���擾
		pGround  = pGround ? pGround : pObject->GetComponent<ComponentGroundBox>();

		// �R���|�[�l���g���擾�ł��Ȃ��ꍇ�͎��̃I�u�W�F�N�g��
		if (pPlaneTran == nullptr) continue;
		if (pGround == nullptr) continue;

		
		// ���C���n�ʂɓ������Ă��邩�ǂ����𔻒�
		if (!CheckHit(pGround))
		{
			m_bIsHit = false;
			continue;
		}

		// �ڐG���W���n�ʂ̏�ɂ��邩�ǂ����𔻒�
		// �ڐG���W���n�ʂ̓����ɂ���ꍇ
 		if (CheckOnGround(pGround))
		{
			m_bIsHit		= true;					
			m_sHitObjName	= pObject->GetName();	// �ڐG�I�u�W�F�N�g�����擾(�΂ߏ��p)

			break;	// �����ꂩ�̒n�ʂɐڐG���Ă���ꍇ�̓��[�v�𔲂���
		}
		// �ڐG���W���n�ʂ̊O���ɂ���ꍇ
		else
		{
			m_bIsHit = false;
		}

	}
}

/* ========================================
	�n�ʐڐG����֐�
	-------------------------------------
	���e�F���C���n�ʂɓ������Ă��邩�ǂ����𔻒�
	-------------------------------------
	����1�F�n�ʃR���|�[�l���g
	-------------------------------------
	�߂�l�F�������Ă���ꍇ��true
=========================================== */
bool ComponentGroundRaycast::CheckHit(ComponentGround* pPlaneGround)
{
	// �n�ʂ̍��W�Ɩ@�����擾
	Vector3<float> vGroundPos		= GetGroundCenterPos(pPlaneGround);
	Vector3<float> vGroundNormal	= pPlaneGround->GetWorldNormalDirection().GetNormalize();

	// ���C
	Vector3<float> vRayStart	= m_vStartPos;									// �n�_
	Vector3<float> vRayEnd		= m_vStartPos + (m_vDirection * m_fRayLength);	// �I�_


	// �n�ʂ̍��W���烌�C�̎n�_�܂ł̋���
	float fDisStart = vGroundNormal.Dot(vRayStart - vGroundPos);	// �n�ʂ̖@���Ƀ��C�̎n�_�܂ł̃x�N�g�����ˉe

	// �n�ʂ̍��W���烌�C�̏I�_�܂ł̋���
	float fDisEnd = vGroundNormal.Dot(vRayEnd - vGroundPos);		// �n�ʂ̖@���Ƀ��C�̏I�_�܂ł̃x�N�g�����ˉe

	// ���C�����ʂɐڐG���Ă��邩���� ====================================

	// ���C�̎n�_�ƏI�_�����ʂ̓������ɂ���ꍇ�͐ڐG���Ă��Ȃ�
	// �n�_(�])�I�_(�])�܂��͎n�_(+)�I�_(+)�̏ꍇ�͐ڐG���Ă��Ȃ�(�K����Z���ʂ̓v���X�ɂȂ�)
	if ((fDisStart * fDisEnd) > 0.0f) 
	{
		return false;
	}
	// ���C�̎n�_�ƏI�_�����ʂ̈قȂ鑤�ɂ���ꍇ�͐ڐG���Ă���
	// ���C�̎n�_�ƏI�_�ŕ��ʂ�����ł���
	else
	{
		float rate		= fDisStart / (fDisStart - fDisEnd);		// ���C�̎n�_���畽�ʂ܂ł̋����̊������v�Z
		float fHitDis	= m_fRayLength * rate;						// ���C��̎n�_����ڐG�_�܂ł̋������v�Z
		m_vHitPos		= vRayStart + (m_vDirection * fHitDis);		// �ڐG�_�̍��W���v�Z(�n�_+(�����~����))

		return true;
	}
}


/* ========================================
	�n�ʓ�������֐�
	-------------------------------------
	���e�F�ڐG���W���n�ʂ̓����ɂ��邩�ǂ����𔻒�
	-------------------------------------
	����1�F�n�ʃR���|�[�l���g
	-------------------------------------
	�߂�l�F�n�ʂ̓����ɂ���ꍇ��true
=========================================== */
bool ComponentGroundRaycast::CheckOnGround(ComponentGround* pPlaneGround)
{
	// �n�ʂ̎O�p�`�����擾
	std::vector<ComponentGround::T_TriangleVertex> vTriangleVertex = pPlaneGround->GetTriangleVertex();	

	// �n�ʂ��\������O�p�`�̐��������[�v
	for (auto& triangle : vTriangleVertex)
	{
		// �O�p�`�̊e�ӂ��擾
		Vector3<float> m_vTriEdges[3] = {
			triangle.pos[1] - triangle.pos[0],	// ���_1�����_2
			triangle.pos[2] - triangle.pos[1],	// ���_2�����_3
			triangle.pos[0] - triangle.pos[2]	// ���_3�����_1
		};

		// �e���_����ڐG�_�܂ł̃x�N�g�����擾
		Vector3<float> vHitVec[3] = {
			m_vHitPos - triangle.pos[0],	// ���_1���ڐG�_
			m_vHitPos - triangle.pos[1],	// ���_2���ڐG�_
			m_vHitPos - triangle.pos[2]		// ���_3���ڐG�_
		};


		Vector3<float> vCross[3];	// �O�ς��i�[����ϐ�

		// �e�ӂƐڐG�_�܂ł̃x�N�g���̊O�ς��v�Z(�ڐG�_���O�p�`�̓����ɂ��邩�ǂ����𔻒�)
		for (int i = 0; i < 3; i++)
		{
			vCross[i] = m_vTriEdges[i].Cross(vHitVec[i]);	// �O�ς��v�Z
		}

		// �ڐG�_���O�p�`�̕ӂƊ��S�ɏd�Ȃ��Ă���ꍇ�̊m�F
		if (vCross[0].Length() == 0.0f || vCross[1].Length() == 0.0f || vCross[2].Length() == 0.0f)
		{
			return true;
		}
	
		// �e�O�ς݂̌��̌������`�F�b�N
		float vDot[3] = {
			vCross[0].Dot(vCross[1]),	// �O��1�ƊO��2�̓���
			vCross[1].Dot(vCross[2]),	// �O��2�ƊO��3�̓���
			vCross[2].Dot(vCross[0])	// �O��3�ƊO��1�̓���
		};

		// ���ς�0�ȏ�̏ꍇ�A�e�O�ς͓��������������Ă���
		// �܂�A�ڐG�_���O�p�`�̓����ɂ���
		if (vDot[0] > 0.0f && 
			vDot[1] > 0.0f && 
			vDot[2] > 0.0f)
		{
			// �m�F�p�̐���`��
			for (int i = 0; i < 3; i++)
			{
				m_pLines_TriPointToHitPos[i]->UpdateLine(1, triangle.pos[i], m_vHitPos, ColorVec3::GREEN);
			}

			return true;	// �l�p�`���\������O�p�`�̓��A�ǂꂩ��ł������ɐڐG�_�������true��Ԃ�			
		}
	}

	// ���ׂĂ̎O�p�`�̓����ɂȂ��ꍇ��false��Ԃ�(�ڐG�_���n�ʂ̊O���ɂ���)
	return false;	
}

/* ========================================
	�n�ʒ��S���W�擾�֐�
	-------------------------------------
	���e�F�n�ʂ̒��S���W���擾
	-------------------------------------
	����1�F�n�ʃR���|�[�l���g
	-------------------------------------
	�߂�l�F�n�ʂ̒��S���W
=========================================== */
Vector3<float> ComponentGroundRaycast::GetGroundCenterPos(ComponentGround* pPlaneGround)
{

	Vector3<float> vCenter = Vector3<float>::Zero();

	std::vector<ComponentGround::T_TriangleVertex> vTriangleVertex = pPlaneGround->GetTriangleVertex();

	std::vector<Vector3<float>> vQuadVertexPos;	// �l�p�`�̒��_���W���i�[

	vQuadVertexPos.push_back(vTriangleVertex[0].pos[0]);	// ����
	vQuadVertexPos.push_back(vTriangleVertex[0].pos[1]);	// �E��
	vQuadVertexPos.push_back(vTriangleVertex[1].pos[1]);	// �E��
	vQuadVertexPos.push_back(vTriangleVertex[1].pos[2]);	// ����

	// �l�p�`�̒��S���W���v�Z�i4�_�̕��ρj
	for (const auto& vertex : vQuadVertexPos)
	{
		vCenter.x += vertex.x;
		vCenter.y += vertex.y;
		vCenter.z += vertex.z;
	}
	vCenter.x /= 4.0f;
	vCenter.y /= 4.0f;
	vCenter.z /= 4.0f;

	return vCenter;
}


/* ========================================
	�Q�b�^�[(�n�ʐڐG���W)�֐�
	-------------------------------------
	�ߒl�FVector3<float>	�n�ʐڐG���W
=========================================== */
Vector3<float> ComponentGroundRaycast::GetHitPos()
{
	return m_vHitPos;
}

/* ========================================
	�Q�b�^�[(�n�ʐڐG����t���O)�֐�
	-------------------------------------
	�ߒl�Fbool	�n�ʐڐG����t���O
=========================================== */
bool ComponentGroundRaycast::GetHitFlg()
{
	return m_bIsHit;
}

/* ========================================
	�Q�b�^�[(�ڐG�I�u�W�F�N�g��)�֐�
	-------------------------------------
	�ߒl�Fstd::string	�ڐG�I�u�W�F�N�g��
=========================================== */
std::string ComponentGroundRaycast::GetHitObjName()
{
	return m_sHitObjName;
}

/* ========================================
	�Z�b�^�[(���C�̒���)�֐�
	-------------------------------------
	����1�Ffloat	���C�̒���
=========================================== */
void ComponentGroundRaycast::SetRayLength(float fLength)
{
	m_fRayLength = fLength;
}

/* ========================================
	�Z�b�^�[(���C�̕���)�֐�
	-------------------------------------
	����1�FVector3<float>	���C�̕���
=========================================== */
void ComponentGroundRaycast::SetRayDirection(const Vector3<float>& vDir)
{
	m_vDirection = vDir;
}

/* ========================================
	�Z�b�^�[(���C�n�_�I�t�Z�b�g)�֐�
	-------------------------------------
	����1�FVector3<float>	���C�n�_�I�t�Z�b�g
=========================================== */
void ComponentGroundRaycast::SetStartPosOffset(const Vector3<float>& vOffset)
{
	m_vStartPosOffset = vOffset;
}

/* ========================================
	�Z�b�^�[(���C�`��t���O)�֐�
	-------------------------------------
	����1�Fbool	���C�`��t���O
=========================================== */
void ComponentGroundRaycast::SetDrawLine(bool bDraw)
{
	m_bIsDispRayLine = bDraw;
}


#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentGroundRaycast::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupGroundRaycast = Item::CreateGroup("GroundRaycast");

	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("StartPos",			Item::Vector, &m_vStartPos));		// ���C�̎n�_
	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("StartPosOffset",	Item::Vector, &m_vStartPosOffset));	// ���C�̎n�_�I�t�Z�b�g
	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("Direction",			Item::Vector, &m_vDirection));		// ���C�̕���
	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("Length",			Item::Float, &m_fRayLength));		// ���C�̒���

	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("HitPos",	Item::Vector, &m_vHitPos));		// ���C�n�ʐڐG���W
	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("IsHit",		Item::Text, &m_sDispIsHit));	// ���C�n�ʐڐG����

	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("IsDispRayLine",				Item::Bool, &m_bIsDispRayLine));			// ���C�`��t���O
	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("IsDispTriPointToHitPos",	Item::Bool, &m_bIsDispTriPointToHitPos));	// �O�p���_���n�ʐڐG���W�܂ł̐��\���t���O

	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("HitObjName", Item::Text, &m_sHitObjName));	// �ڐG�I�u�W�F�N�g��

	window.AddItem(pGroupGroundRaycast);

}
#endif // _DEBUG