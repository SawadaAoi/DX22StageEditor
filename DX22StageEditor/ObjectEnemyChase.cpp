/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�G�L�����ǐՈړ�)�pcpp
	------------------------------------
	ObjectEnemyChase.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ObjectEnemyChase.h"
#include "ComponentEnemyMoveChase.h"
#include "ComponentEnemyState.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
=========================================== */
ObjectEnemyChase::ObjectEnemyChase(SceneBase* pScene)
	:ObjectEnemy(pScene)
	, m_pCompEnemyMoveChase(nullptr)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
=========================================== */
void ObjectEnemyChase::InitLocal()
{
	ObjectEnemy::InitLocal();

	// �R���|�[�l���g�ǉ�
	m_pCompEnemyMoveChase = AddComponent<ComponentEnemyMoveChase>();

	m_pCompEnemyState->SetMoveComponent(m_pCompEnemyMoveChase);
}


/* ========================================
	���[�J���f�[�^�o�͊֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̃��[�J���f�[�^���t�@�C���ɏ�������
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectEnemyChase::OutPutLocalData(std::ofstream& file)
{
	ObjectEnemy::OutPutLocalData(file);

	S_SaveDataEnemyChase data;

	// �ړ����x
	data.fMoveSpeed = m_pCompEnemyMoveChase->GetMoveSpeed();
	// �ǐՊJ�n����
	data.fChaseStartDist = m_pCompEnemyMoveChase->GetChaseStartDist();
	// �ǐՌ��E����
	data.fLimitDistSq = m_pCompEnemyMoveChase->GetLimitDistSq();

	// �t�@�C���ɏ�������
	file.write((char*)&data, sizeof(S_SaveDataEnemyChase));
}

/* ========================================
	���[�J���f�[�^���͊֐�
	-------------------------------------
	���e�F�t�@�C������I�u�W�F�N�g�̃��[�J���f�[�^��ǂݍ���
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectEnemyChase::InputLocalData(std::ifstream& file)
{
	ObjectEnemy::InputLocalData(file);

	S_SaveDataEnemyChase data;

	// �t�@�C������ǂݍ���
	file.read((char*)&data, sizeof(S_SaveDataEnemyChase));
	// �ړ����x
	m_pCompEnemyMoveChase->SetMoveSpeed(data.fMoveSpeed);
	// �ǐՊJ�n����
	m_pCompEnemyMoveChase->SetChaseStartDist(data.fChaseStartDist);
	// �ǐՌ��E����
	m_pCompEnemyMoveChase->SetLimitDistSq(data.fLimitDistSq);

}