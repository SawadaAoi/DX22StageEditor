/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�G�L���������_���ړ�)�pcpp
	------------------------------------
	ObjectEnemyRandom.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectEnemyRandom.h"
#include "ComponentEnemyMoveRandom.h"
#include "ComponentEnemyState.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
========================================== */
ObjectEnemyRandom::ObjectEnemyRandom(SceneBase* pScene)
	:ObjectEnemy(pScene)
	, m_pCompEnemyMoveRandom(nullptr)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
========================================== */
void ObjectEnemyRandom::InitLocal()
{
	ObjectEnemy::InitLocal();

	// �R���|�[�l���g�ǉ�
	m_pCompEnemyMoveRandom = AddComponent<ComponentEnemyMoveRandom>();

	m_pCompEnemyState->SetMoveComponent(m_pCompEnemyMoveRandom);
}

/* ========================================
	���[�J���f�[�^�o�͊֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̃��[�J���f�[�^���t�@�C���ɏ�������
	-------------------------------------
	����1�F�t�@�C��
========================================== */
void ObjectEnemyRandom::OutPutLocalData(std::ofstream& file)
{
	ObjectEnemy::OutPutLocalData(file);

	S_SaveDataEnemyRandom data;

	// �ړ����x
	data.fMoveSpeed = m_pCompEnemyMoveRandom->GetMoveSpeed();

	// �^�[�Q�b�g�؂�ւ�����
	data.fTargetSwitchTime = m_pCompEnemyMoveRandom->GetTargetSwitchTime();

	// �ړ��͈͒��S���W
	data.vMoveLimitCenter = m_pCompEnemyMoveRandom->GetMoveLimitRectCenter();

	// �ړ��͈�
	std::vector<Vector3<float>> vMoveLimit = m_pCompEnemyMoveRandom->GetMoveLimitRect();
	data.vMoveLimitRF = vMoveLimit[0];
	data.vMoveLimitLF = vMoveLimit[1];
	data.vMoveLimitLB = vMoveLimit[2];
	data.vMoveLimitRB = vMoveLimit[3];

	// �t�@�C���ɏ�������
	file.write((char*)&data, sizeof(S_SaveDataEnemyRandom));
}

/* ========================================
	���[�J���f�[�^���͊֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̃��[�J���f�[�^���t�@�C������ǂݍ���
	-------------------------------------
	����1�F�t�@�C��
========================================== */
void ObjectEnemyRandom::InputLocalData(std::ifstream& file)
{
	ObjectEnemy::InputLocalData(file);

	S_SaveDataEnemyRandom data;

	// �t�@�C������ǂݍ���
	file.read((char*)&data, sizeof(data));

	// �ړ����x
	m_pCompEnemyMoveRandom->SetMoveSpeed(data.fMoveSpeed);

	// �^�[�Q�b�g�؂�ւ�����
	m_pCompEnemyMoveRandom->SetTargetSwitchTime(data.fTargetSwitchTime);

	// �ړ��͈͒��S���W
	m_pCompEnemyMoveRandom->SetMoveLimitRectCenter(data.vMoveLimitCenter);

	// �ړ��͈�
	std::vector<Vector3<float>> vMoveLimit;
	vMoveLimit.push_back(data.vMoveLimitRF);
	vMoveLimit.push_back(data.vMoveLimitLF);
	vMoveLimit.push_back(data.vMoveLimitLB);
	vMoveLimit.push_back(data.vMoveLimitRB);

	m_pCompEnemyMoveRandom->SetMoveLimitRect(vMoveLimit);

	m_pCompEnemyMoveRandom->InitTargetPos();

}
