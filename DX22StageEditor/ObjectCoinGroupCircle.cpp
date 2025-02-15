/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�R�C���O���[�v(�~)�pcpp
	------------------------------------
	ObjectCoinGroupCircle.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectCoinGroupCircle.h"
#include "ObjectCoin.h"
#include "SceneBase.h"

#include "ComponentTransform.h"
#include "ComponentRigidbody.h"

// =============== �萔��` =======================
const int	DEFAULT_COIN_NUM	= 5;	// �R�C����
const float DEFAULT_COIN_RADIUS = 2.0f;	// �R�C���~���a


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
=========================================== */
ObjectCoinGroupCircle::ObjectCoinGroupCircle(SceneBase* pScene)
	:ObjectBase(pScene)
	, m_pCoinList()
	, m_nCoinNum(DEFAULT_COIN_NUM)
	, m_fCircleRadius(DEFAULT_COIN_RADIUS)
	, m_bIsInitCreate(false)
	, m_bUseGravity(true)
{
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void ObjectCoinGroupCircle::UpdateLocal()
{	
	// ��������
	if (!m_bIsInitCreate)
	{
		m_bIsInitCreate = true;
		CreateCoin();
	}

}

/* ========================================
	�R�C�������֐�
	-------------------------------------
	���e�F�R�C���𐶐�����
=========================================== */
void ObjectCoinGroupCircle::CreateCoin()
{
	float fCreateRad = 0.0f;

	std::string strCoinName = "Coin_";
	for (int i = 0; i < m_nCoinNum; i++)
	{
		ObjectCoin* pCoin = m_pOwnerScene->AddSceneObject<ObjectCoin>(strCoinName + std::to_string(i));

		Vector2<float> vDirection	= Vector2<float>(cosf(fCreateRad), sinf(fCreateRad));	// ���������x�N�g��
		Vector3<float> vOffset		= Vector3<float>(vDirection.x * m_fCircleRadius, 0.0f, vDirection.y * m_fCircleRadius);

		pCoin->GetTransform()->SetPosition(m_pCompTransform->GetPosition() + vOffset);	// �ʒu�ݒ�
		pCoin->GetComponent<ComponentRigidbody>()->SetUseGravity(m_bUseGravity);		// �d�͎g�p�ݒ�
		pCoin->SetIsSave(false);														// �Z�[�u���Ȃ��ݒ�i�V�[���J�ڎ��ɔj�������悤�ɂ���j

		fCreateRad += MathUtils::ToRadian(360.0f / m_nCoinNum);	// �����ʒu��i�߂�(�~������R�C�����ŕ��������p�x��)

		m_pCoinList.push_back(pCoin);

		AddChildObject(pCoin);
	}

}
/* ========================================
	�R�C�����Z�b�g�֐�
	-------------------------------------
	���e�F�R�C�������Z�b�g����
=========================================== */
void ObjectCoinGroupCircle::ResetCoin()
{
	for (auto& pCoin : m_pCoinList)
	{
		pCoin->Destroy();
	}
	m_pCoinList.clear();
}

/* ========================================
	�Q�b�^�[(�d�͗L��t���O)�֐�
	-------------------------------------
	�ߒl�Fbool �d�͗L��t���O
=========================================== */
bool ObjectCoinGroupCircle::GetUseGravity()
{
	return m_bUseGravity;
}

/* ========================================
	�Z�b�^�[(�d�͗L��t���O)�֐�
	-------------------------------------
	�����Fbool �d�͗L��t���O
=========================================== */
void ObjectCoinGroupCircle::SetUseGravity(bool bUseGravity)
{
	m_bUseGravity = bUseGravity;
}

/* ========================================
	���[�J���f�[�^�o�͊֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̃��[�J���f�[�^���t�@�C���ɏ�������
	-------------------------------------
	����1�F�t�@�C��
=========================================== */
void ObjectCoinGroupCircle::OutPutLocalData(std::ofstream& file)
{
	S_SaveData data;

	// �R�C����
	data.nCoinNum = m_nCoinNum;
	// �R�C���~���a
	data.fCoinRadius = m_fCircleRadius;
	// �d�͎g�p�t���O
	data.bUseGravity = m_bUseGravity;

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
void ObjectCoinGroupCircle::InputLocalData(std::ifstream& file)
{
	S_SaveData data;

	// �t�@�C������ǂݍ���
	file.read((char*)&data, sizeof(S_SaveData));

	// �R�C����
	m_nCoinNum = data.nCoinNum;
	// �R�C���~���a
	m_fCircleRadius = data.fCoinRadius;
	// �d�͎g�p�t���O
	m_bUseGravity = data.bUseGravity;
}

#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
======================================== */
void ObjectCoinGroupCircle::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupCoinCircle = Item::CreateGroup("CoinCircle");

	// �ȉ��̒l��ύX����Ɛ���������
	// �R�C����
	pGroupCoinCircle->AddGroupItem(Item::CreateCallBack("CoinNum", Item::Kind::Int,
		[this](bool isWrite, void* arg)
	{
		int* nNum = reinterpret_cast<int*>(arg);
		if (isWrite)
		{
			m_nCoinNum = *nNum;
			ResetCoin();
			CreateCoin();
		}
		else
		{
			*nNum = m_nCoinNum;
		}
	}));

	// �R�C���~���a
	pGroupCoinCircle->AddGroupItem(Item::CreateCallBack("CircleRadius", Item::Kind::Float,
		[this](bool isWrite, void* arg)
	{
		float* fRadius = reinterpret_cast<float*>(arg);
		if (isWrite)
		{
			m_fCircleRadius = *fRadius;
			ResetCoin();
			CreateCoin();
		}
		else
		{
			*fRadius = m_fCircleRadius;
		}
	}));

	// �R�C�������{�^��
	pGroupCoinCircle->AddGroupItem(Item::CreateCallBack("CreateCoin", Item::Kind::Command,
		[this](bool isWrite, void* arg)
	{
		ResetCoin();
		CreateCoin();
	}));

	window.AddItem(pGroupCoinCircle);
}
#endif // _DEBUG