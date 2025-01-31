/* ========================================
	CatRobotGame/
	------------------------------------
	�R���|�[�l���g(�r���{�[�h(�A�j���[�V�����L��))�pcpp
	------------------------------------
	ComponentBillboardAnime.cpp
========================================== */

// =============== �C���N���[�h =====================
#include "ComponentBillboardAnime.h"
#include "ComponentTransform.h"

#include "ObjectBase.h"

#include "TextureManager.h"
#include "ShaderManager.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F�����I�u�W�F�N�g
========================================== */
ComponentBillboardAnime::ComponentBillboardAnime(ObjectBase* pOwner)
	: ComponentBillboard(pOwner)
	, m_vFrameColRow(1, 1)
	, m_nFrameCnt(0)
	, m_nFrameMax(1)
	, m_fAnimeSpeed(1.0f)
	, m_fFrameSwitchCnt(0.0f)
	, m_fFrameSwitchTimeMax(0.0f)
	, m_bAnimeLoop(false)
	, m_bAnimeEnd(false)
{
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================== */
void ComponentBillboardAnime::Update()
{
	if (m_bAnimeEnd) return;	// �A�j���[�V�����I�����Ă�����X�V���Ȃ�

	m_fFrameSwitchCnt += DELTA_TIME;	// �R�}�ؑ։��Z
	if (m_fFrameSwitchCnt >= m_fFrameSwitchTimeMax)
	{
		m_fFrameSwitchCnt = 0.0f;	// �J�E���^���Z�b�g

		// ���̃R�}��
		m_nFrameCnt++;
		if (m_nFrameCnt >= m_nFrameMax)
		{
			if (m_bAnimeLoop)
			{
				m_nFrameCnt = 0;	// ���[�v����
			}
			else
			{
				m_nFrameCnt = m_nFrameMax - 1;	// ���[�v���Ȃ�
				m_bAnimeEnd = true;	// �A�j���[�V�����I��
			}
		}
	}
}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
========================================== */
void ComponentBillboardAnime::Draw()
{
	float fUvPosX = (m_nFrameCnt % m_vFrameColRow.x) * m_vUvScale.x;	// UV���WX
	float fUvPosY = (m_nFrameCnt / m_vFrameColRow.x) * m_vUvScale.y;	// UV���WY

	SetUvPos(Vector2<float>(fUvPosX, fUvPosY));	// UV���W���Đݒ�

	ComponentBillboard::Draw();
}

/* ========================================
	�Đ��֐�
	-------------------------------------
	���e�F�A�j���[�V�����Đ�
	-------------------------------------
	����1�FframeMax		�ő�R�}��
	����2�FframeColRow	�c���R�}��
	����3�FanimeSpeed	�A�j���[�V�������x
	����4�FanimeLoop	�A�j���[�V�������[�v�t���O
========================================== */
void ComponentBillboardAnime::Play(int frameMax, Vector2<int> frameColRow, float animeSpeed, bool animeLoop)
{
	SetFrameColRow(frameColRow);	// �c���R�}����ݒ�
	SetFrameMax(frameMax);			// �ő�R�}����ݒ�
	SetAnimeSpeed(animeSpeed);		// �A�j���[�V�������x��ݒ�
	SetAnimeLoop(animeLoop);		// �A�j���[�V�������[�v�t���O��ݒ�

	m_bAnimeEnd = false;	// �A�j���[�V�����I���t���O�����Z�b�g
}

/* ========================================
	�Đ��֐�
	-------------------------------------
	���e�F�A�j���[�V�����Đ�
	-------------------------------------
	����1�FframeCnt	�Đ��R�}��
========================================== */
void ComponentBillboardAnime::Play(int frameCnt)
{
	m_bAnimeEnd = false;	// �A�j���[�V�����I���t���O�����Z�b�g

	// �R�}���w�肪����ꍇ
	if (frameCnt >= 0)
	{
		m_nFrameCnt = frameCnt;	// �w��R�}����Đ�

		// �ő�R�}���𒴂��Ă�����ő�R�}���ɐݒ�
		if (m_nFrameCnt >= m_nFrameMax)
			m_nFrameCnt = m_nFrameMax - 1;
	}

	// �R�}���w�肪�Ȃ��ꍇ�͌��݂̃R�}����Đ�
}

/* ========================================
	��~�֐�
	-------------------------------------
	���e�F�A�j���[�V������~
========================================== */
void ComponentBillboardAnime::Stop()
{
	m_bAnimeEnd = true;	// �A�j���[�V�����I���t���O�𗧂Ă�
}

/* ========================================
	�t���[���ؑ֎��Ԑݒ�֐�
	-------------------------------------
	���e�F1�R�}�ɂ����鎞�Ԃ�ݒ�
========================================== */
void ComponentBillboardAnime::SetFrameSwitchTimeMax()
{
	m_fFrameSwitchTimeMax = m_fAnimeSpeed / m_nFrameMax;	// 1���[�v�ɂ����鎞�� / �ő�t���[����
}

/* ========================================
	�Q�b�^�[(�c���R�}��)�֐�
	-------------------------------------
	�ߒl�FVector2<int>	�c���R�}��
=========================================== */
Vector2<int> ComponentBillboardAnime::GetFrameColRow()
{
	return m_vFrameColRow;
}

/* ========================================
	�Q�b�^�[(���݃R�}��)�֐�
	-------------------------------------
	�ߒl�Fint	���݃R�}��
=========================================== */
int ComponentBillboardAnime::GetFrameCnt()
{
	return m_nFrameCnt;
}

/* ========================================
	�Q�b�^�[(�ő�R�}��)�֐�
	-------------------------------------
	�ߒl�Fint	�ő�R�}��
=========================================== */
int ComponentBillboardAnime::GetFrameMax()
{
	return m_nFrameMax;
}

/* ========================================
	�Q�b�^�[(�A�j���[�V�������x)�֐�
	-------------------------------------
	�ߒl�Ffloat	�A�j���[�V�������x
=========================================== */
float ComponentBillboardAnime::GetAnimeSpeed()
{
	return m_fAnimeSpeed;
}

/* ========================================
	�Q�b�^�[(�A�j���[�V�������[�v�t���O)�֐�
	-------------------------------------
	�ߒl�Fbool	�A�j���[�V�������[�v�t���O
=========================================== */
bool ComponentBillboardAnime::GetAnimeLoop()
{
	return m_bAnimeLoop;
}


/* ========================================
	�Q�b�^�[(�A�j���[�V�����I���t���O)�֐�
	-------------------------------------
	�ߒl�Fbool	�A�j���[�V�����I���t���O
=========================================== */
bool ComponentBillboardAnime::GetAnimeEnd()
{
	return m_bAnimeEnd;
}

/* ========================================
	�Z�b�^�[(�c���R�}��)�֐�
	-------------------------------------
	����1�FVector2<int>	�c���R�}��
=========================================== */
void ComponentBillboardAnime::SetFrameColRow(Vector2<int> frameColRow)
{
	m_vFrameColRow = frameColRow;

	m_nFrameMax = m_vFrameColRow.x * m_vFrameColRow.y;	// �ő�t���[�������v�Z

	float fUvWidth	= 1.0f / m_vFrameColRow.x;	// 1�R�}�̕�
	float fUvHeight = 1.0f / m_vFrameColRow.y;	// 1�R�}�̍���
	SetUvScale(Vector2<float>(fUvWidth, fUvHeight));	// UV�X�P�[�����Đݒ�
}

/* ========================================
	�Z�b�^�[(���݃R�}��)�֐�
	-------------------------------------
	����1�Fint	���݃R�}��
=========================================== */
void ComponentBillboardAnime::SetFrameCnt(int frameCnt)
{
	m_nFrameCnt = frameCnt;
}

/* ========================================
	�Z�b�^�[(�ő�R�}��)�֐�
	-------------------------------------
	����1�Fint	�ő�R�}��
=========================================== */
void ComponentBillboardAnime::SetFrameMax(int frameMax)
{
	m_nFrameMax = frameMax;
	SetFrameSwitchTimeMax();	// 1�R�}�ɂ����鎞�Ԃ��Đݒ�
}

/* ========================================
	�Z�b�^�[(�A�j���[�V�������x)�֐�
	-------------------------------------
	����1�Ffloat	�A�j���[�V�������x
=========================================== */
void ComponentBillboardAnime::SetAnimeSpeed(float animeSpeed)
{
	m_fAnimeSpeed = animeSpeed;
	SetFrameSwitchTimeMax();	// 1�R�}�ɂ����鎞�Ԃ��Đݒ�
}

/* ========================================
	�Z�b�^�[(�A�j���[�V�������[�v�t���O)�֐�
	-------------------------------------
	����1�Fbool	�A�j���[�V�������[�v�t���O
=========================================== */
void ComponentBillboardAnime::SetAnimeLoop(bool animeLoop)
{
	m_bAnimeLoop = animeLoop;
}

/* ========================================
	�Z�b�^�[(�A�j���[�V�����I���t���O)�֐�
	-------------------------------------
	����1�Fbool	�A�j���[�V�����I���t���O
=========================================== */
void ComponentBillboardAnime::SetAnimeEnd(bool animeEnd)
{
	m_bAnimeEnd = animeEnd;
}


#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentBillboardAnime::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupBillboardAnime = Item::CreateGroup("BillboardAnime");

	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("PosOffset", Item::Kind::Vector, &m_vPosOffset));	// �F
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("UVPos", Item::Kind::Vector2, &m_vUvPos));	// UV���W
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("UVScale", Item::Kind::Vector2, &m_vUvScale));	// UV�X�P�[��
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("Color", Item::Kind::Vector, &m_vColor));	// �F
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("Alpha", Item::Kind::Float, &m_fAlpha));	// ���ߓx

	pGroupBillboardAnime->AddGroupItem(InitTextureList());	// �e�N�X�`�����X�g

	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("FrameColRow", Item::Kind::Vector2, &m_vFrameColRow));	// �c���R�}��
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("FrameCnt", Item::Kind::Int, &m_nFrameCnt));	// ���݃R�}��
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("FrameMax", Item::Kind::Int, &m_nFrameMax));	// �ő�R�}��
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("AnimeSpeed", Item::Kind::Float, &m_fAnimeSpeed));	// �A�j���[�V�������x
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("AnimeLoop", Item::Kind::Bool, &m_bAnimeLoop));	// �A�j���[�V�������[�v�t���O
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("AnimeEnd", Item::Kind::Bool, &m_bAnimeEnd));	// �A�j���[�V�����I���t���O

	window.AddItem(pGroupBillboardAnime);

}


#endif // _DEBUG