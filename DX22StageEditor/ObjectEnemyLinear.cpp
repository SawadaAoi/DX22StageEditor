/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�G�L�������ړ�)�pcpp
	------------------------------------
	ObjectEnemyLinear.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectEnemyLinear.h"
#include "ComponentEnemyMoveLinear.h"
#include "ComponentEnemyState.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
=========================================== */
ObjectEnemyLinear::ObjectEnemyLinear(SceneBase* pScene)
	:ObjectEnemy(pScene)
	, m_pCompEnemyMoveLinear(nullptr)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
=========================================== */
void ObjectEnemyLinear::InitLocal()
{
	ObjectEnemy::InitLocal();

	// �R���|�[�l���g�ǉ�
	m_pCompEnemyMoveLinear = AddComponent<ComponentEnemyMoveLinear>();

	m_pCompEnemyState->SetMoveComponent(m_pCompEnemyMoveLinear);
}

/* ========================================
	���[�J���f�[�^�o�͊֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̃��[�J���f�[�^���t�@�C���ɏ�������
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectEnemyLinear::OutPutLocalData(std::ofstream& file)
{
	ObjectEnemy::OutPutLocalData(file);

	S_SaveDataEnemyLinear data;

	// �ړ����x
	data.fMoveSpeed = m_pCompEnemyMoveLinear->GetMoveSpeed();
	// �ړ����W��
	data.nWayPointNum = m_pCompEnemyMoveLinear->GetWayPoints().size();
	// �t���t���O
	data.bIsReverse = m_pCompEnemyMoveLinear->GetIsReverse();

	// �t�@�C���ɏ�������
	file.write((char*)&data, sizeof(S_SaveDataEnemyLinear));

	// �ړ����W
	for (int i = 0; i < m_pCompEnemyMoveLinear->GetWayPoints().size(); i++)
	{
		// �t�@�C���ɏ�������
		file.write((char*)&m_pCompEnemyMoveLinear->GetWayPoints()[i], sizeof(Vector3<float>));
	}
}

/* ========================================
	���[�J���f�[�^���͊֐�
	-------------------------------------
	���e�F�t�@�C������I�u�W�F�N�g�̃��[�J���f�[�^��ǂݍ���
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectEnemyLinear::InputLocalData(std::ifstream& file)
{
	ObjectEnemy::InputLocalData(file);

	S_SaveDataEnemyLinear data;

	// �t�@�C������ǂݍ���
	file.read((char*)&data, sizeof(S_SaveDataEnemyLinear));

	// �ړ����x
	m_pCompEnemyMoveLinear->SetMoveSpeed(data.fMoveSpeed);
	// �ړ����W��
	int nWayPointNum = data.nWayPointNum;
	// �t���t���O
	m_pCompEnemyMoveLinear->SetIsReverse(data.bIsReverse);

	// �ړ����W
	for (int i = 0; i < data.nWayPointNum; i++)
	{
		Vector3<float> wayPoint;

		// �t�@�C������ǂݍ���
		file.read((char*)&wayPoint, sizeof(Vector3<float>));

		// �ړ����W�ǉ�
		m_pCompEnemyMoveLinear->AddWayPoint(wayPoint);
	}

}