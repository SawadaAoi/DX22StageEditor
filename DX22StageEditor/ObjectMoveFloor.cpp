/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�ړ���)�pcpp
	------------------------------------
	ObjectMoveFloor.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectMoveFloor.h"
#include "ComponentGimmickMoveLinear.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ComponentGroundBox.h"
#include "ComponentCollisionBase.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
========================================== */
ObjectMoveFloor::ObjectMoveFloor(SceneBase* pScene)
	: ObjectBlock(pScene)
	, m_pCompMoveLinear(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_pCompGroundBox(nullptr)
{
	SetTag(E_ObjectTag::Ground);	// �^�O�̐ݒ�
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F������
========================================== */
void ObjectMoveFloor::InitLocal()
{
	m_eColType = E_COL_TYPE::COL_OBB;	// �Փ˔���`��
	ObjectBlock::InitLocal();			// �e�N���X�̏������֐����Ă�

	m_pCompMoveLinear = AddComponent<ComponentGimmickMoveLinear>();	// �����ړ��R���|�[�l���g
	m_pCompGroundBox = AddComponent<ComponentGroundBox>();			// �n�ʔ���R���|�[�l���g

	m_pCompColBase->SetDynamic(true);	// ���I�I�u�W�F�N�g�ɐݒ�
}

/* ========================================
	�Փ˔���(�J�n��)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�J�n��)
	-------------------------------------
	�����F�Փˑ���I�u�W�F�N�g
=========================================== */
void ObjectMoveFloor::OnCollisionEnter(ObjectBase* pHit)
{
	// �v���C���[���G�L�������������
	if (pHit->GetTag() == E_ObjectTag::Player ||
		pHit->GetTag() == E_ObjectTag::Enemy)
	{
		AddChildObject(pHit);	// �q�I�u�W�F�N�g�ɒǉ�(�ꏏ�Ɉړ�)
	}
}

/* ========================================
	�Փ˔���(�I����)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�I����)
	-------------------------------------
	�����F�Փˑ���I�u�W�F�N�g
=========================================== */
void ObjectMoveFloor::OnCollisionExit(ObjectBase* pHit)
{
	// �v���C���[���G�L���������ꂽ��
	if (pHit->GetTag() == E_ObjectTag::Player ||
		pHit->GetTag() == E_ObjectTag::Enemy)
	{
		RemoveChildObject(pHit);	// �q�I�u�W�F�N�g����폜
	}
}

/* ========================================
	�R�s�[�֐�(�ʏ���)
	-------------------------------------
	���e�F�I�u�W�F�N�g�ʂ̃R�s�[����
	-------------------------------------
	����1�F�R�s�[���ꂽ�I�u�W�F�N�g
=========================================== */
void ObjectMoveFloor::CopyLocal(ObjectBase* pObject)
{
	ObjectBlock::CopyLocal(pObject);	// �e�N���X�̃R�s�[�֐����Ă�

	ComponentGimmickMoveLinear* pCopyObjCompMoveLinear = pObject->GetComponent<ComponentGimmickMoveLinear>();	// �ړ��R���|�[�l���g

	// �ړ����x
	pCopyObjCompMoveLinear->SetMoveSpeed(m_pCompMoveLinear->GetMoveSpeed());
	// �ړ����W��
	int nWayPointNum = m_pCompMoveLinear->GetWayPoints().size();
	// �t���t���O
	pCopyObjCompMoveLinear->SetIsReverse(m_pCompMoveLinear->GetIsReverse());

	// �ړ����W
	for (int i = 0; i < nWayPointNum; i++)
	{
		// �ړ����W�ǉ�
		pCopyObjCompMoveLinear->AddWayPoint(m_pCompMoveLinear->GetWayPoints()[i]);
	}
}

/* ========================================
	���[�J���f�[�^�o�͊֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̃��[�J���f�[�^���t�@�C���ɏ�������
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectMoveFloor::OutPutLocalData(std::ofstream& file)
{
	ObjectBlock::OutPutLocalData(file);	// �e�N���X�̃��[�J���f�[�^�o�͊֐����Ă�

	S_SaveData data;

	// �ړ����x
	data.fMoveSpeed = m_pCompMoveLinear->GetMoveSpeed();
	// �ړ����W��
	data.nWayPointNum = m_pCompMoveLinear->GetWayPoints().size();
	// �t���t���O
	data.bIsReverse = m_pCompMoveLinear->GetIsReverse();

	// �t�@�C���ɏ�������
	file.write((char*)&data, sizeof(S_SaveData));

	// �ړ����W
	for (int i = 0; i < m_pCompMoveLinear->GetWayPoints().size(); i++)
	{
		// �t�@�C���ɏ�������
		file.write((char*)&m_pCompMoveLinear->GetWayPoints()[i], sizeof(Vector3<float>));
	}
}

/* ========================================
	���[�J���f�[�^���͊֐�
	-------------------------------------
	���e�F�t�@�C������I�u�W�F�N�g�̃��[�J���f�[�^��ǂݍ���
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectMoveFloor::InputLocalData(std::ifstream& file)
{
	ObjectBlock::InputLocalData(file);	// �e�N���X�̃��[�J���f�[�^���͊֐����Ă�

	S_SaveData data;

	// �t�@�C������ǂݍ���
	file.read((char*)&data, sizeof(S_SaveData));

	// �ړ����x
	m_pCompMoveLinear->SetMoveSpeed(data.fMoveSpeed);
	// �ړ����W��
	int nWayPointNum = data.nWayPointNum;
	// �t���t���O
	m_pCompMoveLinear->SetIsReverse(data.bIsReverse);

	// �ړ����W
	for (int i = 0; i < data.nWayPointNum; i++)
	{
		Vector3<float> wayPoint;

		// �t�@�C������ǂݍ���
		file.read((char*)&wayPoint, sizeof(Vector3<float>));

		// �ړ����W�ǉ�
		m_pCompMoveLinear->AddWayPoint(wayPoint);
	}
}

/* ========================================
	�Q�b�^�[(�ړ����R���|�[�l���g)�֐�
	-------------------------------------
	�ߒl: �R���|�[�l���g�|�C���^
=========================================== */
ComponentGimmickMoveLinear* ObjectMoveFloor::GetCompMoveLinear()
{
	return m_pCompMoveLinear;
}
