/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�R�C���O���[�v(����)�pcpp
	------------------------------------
	ObjectCoinGroupLinear.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ObjectCoinGroupLinear.h"
#include "ObjectCoin.h"
#include "SceneBase.h"

#include "ComponentTransform.h"
#include "ComponentRigidbody.h"

// =============== �萔��` =======================
const int	DEFAULT_COIN_NUM	= 5;	// �R�C����
const float DEFAULT_COIN_SPACE	= 1.0f;	// �R�C���Ԋu


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�V�[���|�C���^
=========================================== */
ObjectCoinGroupLinear::ObjectCoinGroupLinear(SceneBase* pScene)
	:ObjectBase(pScene)
	, m_pCoinList()
	, m_nCoinNum(DEFAULT_COIN_NUM)
	, m_fCoinSpace(DEFAULT_COIN_SPACE)
	, m_bIsInitCreate(false)
	, m_bUseGravity(true)
{
}


/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void ObjectCoinGroupLinear::UpdateLocal()
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
void ObjectCoinGroupLinear::CreateCoin()
{
	// Y����]�p�x���擾�i0�`2�΁j
	float fRad = m_pCompTransform->GetRotation().ToEulerAngle(true).y;
	fRad = MathUtils::ConvertDirectionDXToTrigonometric(fRad); // DirectX�̕������O�p�֐��̕����ɕϊ�

	// ���g�������Ă�������x�N�g�����擾
	Vector2<float> vDirection = Vector2<float>(cosf(fRad), sinf(fRad));

	// �R�C���̊�ʒu���v�Z�i���W�����S�ɂȂ�悤�ɒ����j
	float fCenterOffset = ((m_nCoinNum - 1) * m_fCoinSpace) / 2;
	Vector3<float> vBasePos = m_pCompTransform->GetPosition() -
		Vector3<float>(vDirection.x * fCenterOffset, 0.0f, vDirection.y * fCenterOffset);

	// �R�C���̐���
	std::string strCoinName = "Coin_";
	for (int i = 0; i < m_nCoinNum; i++)
	{
		ObjectCoin* pCoin = m_pOwnerScene->AddSceneObject<ObjectCoin>(strCoinName + std::to_string(i));
		// �e�������Ă�������ɃR�C����z�u
		Vector3<float> vOffset = Vector3<float>(vDirection.x * m_fCoinSpace * i, 0.0f, vDirection.y * m_fCoinSpace * i);	

		pCoin->GetTransform()->SetPosition(vBasePos + vOffset);						// �ʒu�ݒ�
		pCoin->GetComponent<ComponentRigidbody>()->SetUseGravity(m_bUseGravity);	// �d�͎g�p�ݒ�
		pCoin->SetIsSave(false);													// �Z�[�u���Ȃ��ݒ�i�V�[���J�ڎ��ɔj�������悤�ɂ���j

		m_pCoinList.push_back(pCoin);
		AddChildObject(pCoin);
	}
}


/* ========================================
	�R�C�����Z�b�g�֐�
	-------------------------------------
	���e�F�R�C�������Z�b�g����
=========================================== */
void ObjectCoinGroupLinear::ResetCoin()
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
bool ObjectCoinGroupLinear::GetUseGravity()
{
	return m_bUseGravity;
}

/* ========================================
	�Z�b�^�[(�d�͗L��t���O)�֐�
	-------------------------------------
	�����Fbool �d�͗L��t���O
=========================================== */
void ObjectCoinGroupLinear::SetUseGravity(bool bUseGravity)
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
void ObjectCoinGroupLinear::OutPutLocalData(std::ofstream& file)
{
	S_SaveData data;

	// �R�C����
	data.nCoinNum = m_nCoinNum;
	// �R�C���Ԋu
	data.fCoinSpace = m_fCoinSpace;
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
void ObjectCoinGroupLinear::InputLocalData(std::ifstream& file)
{
	S_SaveData data;

	// �t�@�C������ǂݍ���
	file.read((char*)&data, sizeof(S_SaveData));

	// �R�C����
	m_nCoinNum = data.nCoinNum;
	// �R�C���Ԋu
	m_fCoinSpace = data.fCoinSpace;
	// �d�͎g�p�t���O
	m_bUseGravity = data.bUseGravity;
}

#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
======================================== */
void ObjectCoinGroupLinear::DebugLocal(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupCoinLinear = Item::CreateGroup("CoinLinear");

	// �ȉ��̒l��ύX����Ɛ���������
	// �R�C����
	pGroupCoinLinear->AddGroupItem(Item::CreateCallBack("CoinNum", Item::Kind::Int,
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

	// �R�C���Ԋu
	pGroupCoinLinear->AddGroupItem(Item::CreateCallBack("CoinSpace", Item::Kind::Float, 
	[this](bool isWrite, void* arg)
	{
		float* fSpace = reinterpret_cast<float*>(arg);
		if (isWrite)
		{
			m_fCoinSpace = *fSpace;
			ResetCoin();
			CreateCoin();
		}
		else
		{
			*fSpace = m_fCoinSpace;
		}
	}));

	// �R�C�������{�^��
	pGroupCoinLinear->AddGroupItem(Item::CreateCallBack("CreateCoin", Item::Kind::Command,
		[this](bool isWrite, void* arg)
	{
		ResetCoin();
		CreateCoin();
	}));

	window.AddItem(pGroupCoinLinear);
}
#endif // _DEBUG