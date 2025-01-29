/* ========================================
	CatRobotGame/
	------------------------------------
	���X�|�[���pcpp
	------------------------------------
	ObjectRespawn.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectRespawn.h"
#include "ObjectPlayer.h"
#include "SceneBase.h"
#include "SceneManager.h"
#include "ComponentTransform.h"
#include "ComponentCollisionAABB.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	�����F���L�V�[��
=========================================== */
ObjectRespawn::ObjectRespawn(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_RespawnType(E_RespawnType::RT_RELOAD_SCENE)
	, m_RespawnPosition(Vector3<float>(0.0f, 0.0f, 0.0f))
	, m_pCompColAABB(nullptr)
	, m_IsReloadScene(false)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F������
=========================================== */
void ObjectRespawn::InitLocal()
{
	// �����蔻��ݒ�
	m_pCompColAABB = AddComponent<ComponentCollisionAABB>();
	m_pCompColAABB->SetTrigger(true);
	m_pCompColAABB->SetDynamic(false);

}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V
=========================================== */
void ObjectRespawn::UpdateLocal()
{
	// �V�[�����ēǂݍ���
	if (m_IsReloadScene)	SceneManager::ReloadScene();
}

/* ========================================
	�Փ˔���(�J�n��)�֐�
	-------------------------------------
	���e�F���I�u�W�F�N�g�Ƃ̏Փ˔���(�J�n��)
	-------------------------------------
	�����F�Փˑ���I�u�W�F�N�g
=========================================== */
void ObjectRespawn::OnCollisionEnter(ObjectBase* pHit)
{
	// �v���C���[�Ɠ��������ꍇ
	if (pHit->GetTag() == E_ObjectTag::Player)
	{
		// ���X�|�[����ނɂ���ď�����ς���
		switch (m_RespawnType)
		{
		// �V�[�����ēǂݍ���
		case E_RespawnType::RT_RELOAD_SCENE:
			// �����蔻�菈�����ŃV�[���ēǂݍ��ݍs���ƁA���g�̏��L�V�[�����j������邽�߁A
			// �t���O�𗧂Ă�Update�����ōēǂݍ��݂���
			m_IsReloadScene = true;
			break;
		// ���܂����ʒu���烊�X�|�[��
		case E_RespawnType::RT_POSITION:	
			pHit->GetTransform()->SetPosition(m_RespawnPosition);
			break;
		}
	}
}


/* ========================================
	�Q�b�^�[(���X�|�[�����)�֐�
	-------------------------------------
	�ߒl�F���X�|�[�����
=========================================== */
ObjectRespawn::E_RespawnType ObjectRespawn::GetRespawnType() const
{
	return m_RespawnType;
}


/* ========================================
	�Q�b�^�[(���X�|�[���ʒu)�֐�
	-------------------------------------
	�ߒl�F���X�|�[���ʒu
=========================================== */
Vector3<float> ObjectRespawn::GetRespawnPosition() const
{
	return m_RespawnPosition;
}

/* ========================================
	�Z�b�^�[(���X�|�[�����)�֐�
	-------------------------------------
	�����F���X�|�[�����
=========================================== */
void ObjectRespawn::SetRespawnType(E_RespawnType type)
{
	m_RespawnType = type;
}

/* ========================================
	�Z�b�^�[(���X�|�[���ʒu)�֐�
	-------------------------------------
	�����F���X�|�[���ʒu
=========================================== */
void ObjectRespawn::SetRespawnPosition(const Vector3<float>& pos)
{
	m_RespawnPosition = pos;
}

/* ========================================
	���[�J���f�[�^�o�͊֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̃��[�J���f�[�^���t�@�C���ɏ�������
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectRespawn::OutPutLocalData(std::ofstream& file)
{
	S_SaveData data;

	// ���X�|�[�����
	data.eRespawnType = m_RespawnType;
	// ���X�|�[���ʒu
	data.vRespawnPosition = m_RespawnPosition;

	// �t�@�C���ɏ�������
	file.write((char*)&data, sizeof(S_SaveData));
}

/* ========================================
	���[�J���f�[�^���͊֐�
	-------------------------------------
	���e�F�t�@�C������I�u�W�F�N�g�̃��[�J���f�[�^��ǂݍ���
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectRespawn::InputLocalData(std::ifstream& file)
{
	S_SaveData data;

	// �t�@�C������ǂݍ���
	file.read((char*)&data, sizeof(S_SaveData));

	// ���X�|�[�����
	m_RespawnType = data.eRespawnType;
	// ���X�|�[���ʒu
	m_RespawnPosition = data.vRespawnPosition;
}




#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
======================================== */
void ObjectRespawn::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupRespawn = Item::CreateGroup("Respawn");

	// ���X�|�[�����
	pGroupRespawn->AddGroupItem(Item::CreateList("RespawnType", [this](const void* arg)
	{
		int nNum = WIN_OBJ_INFO["Respawn"]["RespawnType"].GetInt();
		m_RespawnType = (E_RespawnType)nNum;

	}, false, true));

	// ���X�|�[���ʒu
	pGroupRespawn->AddGroupItem(Item::CreateBind("RespawnPosition", Item::Kind::Vector, &m_RespawnPosition));

	window.AddItem(pGroupRespawn);

	// ���X�|�[����ރ��X�g�ǉ�
	WIN_OBJ_INFO["Respawn"]["RespawnType"].AddListItem("ReloadScene");
	WIN_OBJ_INFO["Respawn"]["RespawnType"].AddListItem("Position");
	WIN_OBJ_INFO["Respawn"]["RespawnType"].SetListNo((int)m_RespawnType);

}

#endif // _DEBUG