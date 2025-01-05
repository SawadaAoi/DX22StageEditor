/* ========================================
	DX22Base/
	------------------------------------
	�A�j���[�V�������f��(�A�j���[�V��������)�pcpp
	------------------------------------
	ModelAnimation.cpp
========================================== */


// =============== �C���N���[�h ===================
#include "ModelAnime.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#if _MSC_VER >= 1930
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "assimp-vc143-mt.lib")
#endif
#elif _MSC_VER >= 1920
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif
#elif _MSC_VER >= 1910
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc141-mtd.lib")
#else
#pragma comment(lib, "assimp-vc141-mt.lib")
#endif
#endif



/* ========================================
	�A�j���[�V�����ǉ��֐�
	-------------------------------------
	���e�F�A�j���[�V������Ǎ��A�ǉ�����
	-------------------------------------
	�����F�ǂݍ��ރA�j���[�V�����t�@�C���ւ̃p�X
	-------------------------------------
	�ߒl�F�����Ŋ��蓖�Ă�ꂽ�A�j���[�V�����ԍ�
=========================================== */
ModelAnime::AnimeNo ModelAnime::AddAnimation(const char* file)
{
#ifdef _DEBUG
	m_sErrorMessage = "";
#endif

	
	// assimp�̐ݒ�
	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_FlipUVs;
	if (m_LoadFlip == E_Flip::XFlip)  flag |= aiProcess_MakeLeftHanded;

	// assimp�œǂݍ���
	const aiScene* pScene = importer.ReadFile(file, flag);
	if (!pScene)
	{
#ifdef _DEBUG
		m_sErrorMessage += importer.GetErrorString();
#endif
		return ANIME_NONE;
	}

	// �A�j���[�V�����`�F�b�N
	if (!pScene->HasAnimations())
	{
#ifdef _DEBUG
		m_sErrorMessage += "no animation.";
#endif
		return ANIME_NONE;
	}

	// �A�j���[�V�����쐬
	MakeAnimation(pScene);

	
	m_AnimeList.back().sName = GetFBXDataName(file);

	// �A�j���ԍ���Ԃ�
	return static_cast<AnimeNo>(m_AnimeList.size() - 1);
}



/* ========================================
	�A�j���[�V�����Đ��֐�
	-------------------------------------
	���e�F�w�肵���A�j���[�V�������Đ�����
	-------------------------------------
	����1�F�Đ�����A�j���[�V�����ԍ�
	����2�F���[�v�Đ��t���O
	����3�F�Đ����x
=========================================== */
void ModelAnime::Play(AnimeNo no, bool loop, float speed)
{
	// �Đ��`�F�b�N
	if (!AnimeNoCheck(no)) { return; }
	if (m_nActiveNo == no) { return; }

	// �����A�j���[�V�������`�F�b�N
	if (no != PARAMETRIC_ANIME)
	{
		// �ʏ�̏�����
		InitAnime(no);
		m_AnimeList[no].isLoop = loop;
		m_AnimeList[no].speed = speed;
	}
	else
	{
		// �����A�j���[�V�����̌��ɂȂ��Ă���A�j���[�V������������
		InitAnime(m_nParametricAnimeNos[0]);
		InitAnime(m_nParametricAnimeNos[1]);
		m_AnimeList[m_nParametricAnimeNos[0]].isLoop = loop;
		m_AnimeList[m_nParametricAnimeNos[1]].isLoop = loop;
	}

	// �Đ��A�j���[�V�����̐ݒ�
	m_nActiveNo = no;
}


/* ========================================
	�A�j���[�V�����Đ�(�u�����h)�֐�
	-------------------------------------
	���e�F���ݍĐ����̃A�j������w�肵���A�j����
		�@���炩�ɑJ�ڂ���
	-------------------------------------
	����1�F�Đ�����A�j���[�V�����ԍ�
	����2�F�u�����h����
	����3�F���[�v�Đ��t���O
	����4�F�Đ����x
=========================================== */
void ModelAnime::PlayBlend(AnimeNo no, float blendTime, bool loop, float speed)
{
	// �Đ��`�F�b�N
	if (!AnimeNoCheck(no)) { return; }

	// �����A�j���[�V�������`�F�b�N
	if (no != PARAMETRIC_ANIME)
	{
		InitAnime(no);
		m_AnimeList[no].isLoop = loop;
		m_AnimeList[no].speed = speed;
	}
	else
	{
		// �����A�j���[�V�����̌��ɂȂ��Ă���A�j���[�V������������
		InitAnime(m_nParametricAnimeNos[0]);
		InitAnime(m_nParametricAnimeNos[1]);
		m_AnimeList[m_nParametricAnimeNos[0]].isLoop = loop;
		m_AnimeList[m_nParametricAnimeNos[1]].isLoop = loop;
	}

	// �u�����h�̐ݒ�
	m_fBlendTime		= 0.0f;
	m_fBlendTimeTotal	= blendTime;
	m_nBlendNo			= no;
}


/* ========================================
	�������A�j���[�V�����ݒ�֐�
	-------------------------------------
	���e�F�������A�j���[�V������ݒ肷��
		�@�������A�j���[�V�����Đ���PARAMETRIC_ANIME���w�肷��
	-------------------------------------
	����1�F�������A�j���[�V����1
	����2�F�������A�j���[�V����2
=========================================== */
void ModelAnime::SetParametric(AnimeNo no1, AnimeNo no2)
{
	// �A�j���[�V�����`�F�b�N
	if (!AnimeNoCheck(no1)) { return; }
	if (!AnimeNoCheck(no2)) { return; }

	// �����ݒ�
	m_nParametricAnimeNos[0] = no1;
	m_nParametricAnimeNos[1] = no2;
	SetParametricBlend(0.0f);
}


/* ========================================
	�����A�j���[�V���������ݒ�֐�
	-------------------------------------
	���e�F�����A�j���[�V�����̍���������ݒ�
	-------------------------------------
	�����F��������
=========================================== */
void ModelAnime::SetParametricBlend(float blendRate)
{
	// �������A�j�����ݒ肳��Ă��邩�m�F
	if (m_nParametricAnimeNos[0] == ANIME_NONE || m_nParametricAnimeNos[1] == ANIME_NONE) return;

	// ���������ݒ�
	m_fParametricBlendRatio = blendRate;

	// �����Ɋ�Â��ăA�j���[�V�����̍Đ����x��ݒ�
	T_Animation& anime1 = m_AnimeList[m_nParametricAnimeNos[0]];
	T_Animation& anime2 = m_AnimeList[m_nParametricAnimeNos[1]];
	float blendTotalTime = anime1.totalTime * (1.0f - m_fParametricBlendRatio) + anime2.totalTime * m_fParametricBlendRatio;
	anime1.speed = anime1.totalTime / blendTotalTime;
	anime2.speed = anime2.totalTime / blendTotalTime;
}


/* ========================================
	�A�j���[�V�����Đ����Ԉʒu�ύX�֐�
	-------------------------------------
	���e�F�w�肵���A�j���[�V�����̍Đ����Ԉʒu��ύX
	-------------------------------------
	����1�F�ύX����A�j��
	����2�F�Đ����Ԉʒu
=========================================== */
void ModelAnime::SetAnimationTime(AnimeNo no, float time)
{
	// �A�j���[�V�����`�F�b�N
	if (!AnimeNoCheck(no)) { return; }

	// �Đ����ԕύX
	T_Animation& anime = m_AnimeList[no];
	anime.nowTime = time;

	// �Đ����Ԃ��A�j���[�V�����̑����Ԃ𒴂��Ă�����A���[�v������
	while (anime.nowTime >= anime.totalTime)
	{
		anime.nowTime -= anime.totalTime;
	}
}

/* ========================================
	�A�j���[�V�����Đ������ʒu�ύX�֐�
	-------------------------------------
	���e�F�w�肵���A�j���[�V�����̍Đ������ʒu��ύX
	-------------------------------------
	����1�F�ύX����A�j��
	����2�F�Đ������ʒu
=========================================== */
void ModelAnime::SetAnimationTimeRatio(AnimeNo no, float rate)
{
	// �A�j���[�V�����`�F�b�N
	if (!AnimeNoCheck(no)) { return; }

	// �Đ����ԕύX
	T_Animation& anime = m_AnimeList[no];
	anime.nowTime = anime.totalTime * rate;

	// �Đ����Ԃ��A�j���[�V�����̑����Ԃ𒴂��Ă�����A���[�v������
	while (anime.nowTime >= anime.totalTime)
	{
		anime.nowTime -= anime.totalTime;
	}
}

/* ========================================
	�A�j���[�V�����Đ����Ԏ擾�֐�
	-------------------------------------
	���e�F�A�j���[�V�����̌��ݍĐ����Ԃ��擾
	-------------------------------------
	�����F�擾����A�j���ԍ�
	-------------------------------------
	�ߒl�F�Đ�����
=========================================== */
float ModelAnime::GetAnimeNowTime(AnimeNo no)
{
	// �A�j���[�V�����`�F�b�N
	if (!AnimeNoCheck(no)) { return 0.0f; }

	T_Animation& anime = m_AnimeList[no];

	return anime.nowTime;
}

/* ========================================
	�A�j���[�V�����g�[�^���Đ����Ԏ擾�֐�
	-------------------------------------
	���e�F�A�j���[�V�����̃g�[�^���Đ����Ԃ��擾
	-------------------------------------
	�����F�擾����A�j���ԍ�
	-------------------------------------
	�ߒl�F�g�[�^������
=========================================== */
float ModelAnime::GetAnimeTotalTime(AnimeNo no)
{
	// �A�j���[�V�����`�F�b�N
	if (!AnimeNoCheck(no)) { return 0.0f; }

	T_Animation& anime = m_AnimeList[no];

	return anime.totalTime;
}

/* ========================================
	�A�j���[�V�����Đ������擾�֐�
	-------------------------------------
	���e�F�A�j���[�V�����̍Đ��������擾
	-------------------------------------
	�����F�擾����A�j���ԍ�
	-------------------------------------
	�ߒl�F�Đ�����
=========================================== */
float ModelAnime::GetAnimePlaybackRatio(AnimeNo no)
{
	// �A�j���[�V�����`�F�b�N
	if (!AnimeNoCheck(no)) { return 0.0f; }

	T_Animation& anime = m_AnimeList[no];

	return anime.nowTime / anime.totalTime;
}


/* ========================================
	�A�j���[�V�����Đ����x�ύX�֐�
	-------------------------------------
	���e�F�w�肵���A�j���[�V�����̍Đ����x��ύX����
	-------------------------------------
	����1�F�ύX����A�j���ԍ�
	����2�F�V�����Đ����x
=========================================== */
void ModelAnime::SetAnimationSpeed(AnimeNo no, float speed)
{
	// �A�j���[�V�����`�F�b�N
	if (!AnimeNoCheck(no)) { return; }

	m_AnimeList[no].speed = speed;
}

/* ========================================
	�Đ��t���O�擾�֐�
	-------------------------------------
	���e�F�w�肵���A�j���[�V�������Đ������ǂ������擾
	-------------------------------------
	����1�F���ׂ�A�j���ԍ�
	����2�Ftrue = �Đ��� / false = ��~��
=========================================== */
bool ModelAnime::IsPlay(AnimeNo no)
{
	// �A�j���[�V�����`�F�b�N
	if (!AnimeNoCheck(no)) { return false; }

	// �p�����g���b�N�͍������̃A�j������ɔ��f
	if (no == PARAMETRIC_ANIME) { no = m_nParametricAnimeNos[0]; }

	// �Đ����Ԃ̔���
	if (m_AnimeList[no].totalTime < m_AnimeList[no].nowTime) { return false; }

	// ���ꂼ��̍Đ��ԍ��ɐݒ肳��Ă��邩�m�F
	if (m_nActiveNo == no) { return true; }
	if (m_nBlendNo == no) { return true; }
	if (m_nActiveNo == PARAMETRIC_ANIME || m_nBlendNo == PARAMETRIC_ANIME)
	{
		if (m_nParametricAnimeNos[0] == no) { return true; }
		if (m_nParametricAnimeNos[1] == no) { return true; }
	}

	// �Đ����łȂ�
	return false;
}


/* ========================================
	�A�j���ԍ��`�F�b�N�֐�
	-------------------------------------
	���e�F�w�肵���A�j���ԍ������������ǂ������m�F
	-------------------------------------
	�����F�m�F����A�j���ԍ�
	-------------------------------------
	�ߒl�Ftrue = ���� / false = �ُ�
=========================================== */
bool ModelAnime::AnimeNoCheck(AnimeNo no)
{
	// �p�����g���b�N�A�j���[�V�����m�F
	if (no == PARAMETRIC_ANIME)
	{
		// �p�����g���b�N�̃A�j���[�V�����������������ݒ肳��Ă��邩
		return
			m_nParametricAnimeNos[0] != ANIME_NONE &&
			m_nParametricAnimeNos[1] != ANIME_NONE;
	}
	else
	{
		// ���Ȃ��A�j���[�V�����ԍ����ǂ���
		return 0 <= no && no < m_AnimeList.size();
	}
}


/* ========================================
	�A�j���[�V�����������֐�
	-------------------------------------
	���e�F�w�肵���A�j���[�V����������������
	-------------------------------------
	�����F�A�j���ԍ�
=========================================== */
void ModelAnime::InitAnime(AnimeNo no)
{
	// �A�j���̐ݒ�Ȃ��A�p�����g���b�N�Őݒ肳��Ă���Ȃ珉�������Ȃ�
	if (no == ANIME_NONE || no == PARAMETRIC_ANIME) { return; }

	T_Animation& anime = m_AnimeList[no];
	anime.nowTime = 0.0f;
	anime.speed = 1.0f;
	anime.isLoop = false;
}


/* ========================================
	�A�j���[�V�����v�Z�֐�
	-------------------------------------
	���e�F�w�肵���A�j���[�V�����̎p�����v�Z����
	-------------------------------------
	����1�F�A�j���[�V�����̎��
	����2�F�A�j���[�V�����ԍ�
=========================================== */
void ModelAnime::CalcAnime(E_AnimeTransform kind, AnimeNo no)
{
	T_Animation& anime = m_AnimeList[no];
	Channels::iterator channelIt = anime.channels.begin();

	while (channelIt != anime.channels.end())
	{
		// ��v����{�[�����Ȃ���΃X�L�b�v
		Timeline& timeline = channelIt->timeline;
		if (channelIt->index == INDEX_NONE || timeline.empty())
		{
			++channelIt;
			continue;
		}

		//--- �Y���m�[�h�̎p�����A�j���[�V�����ōX�V
		T_Transform& transform = m_NodeTransform[kind][channelIt->index];
		if (timeline.size() <= 1)
		{
			// �L�[��������Ȃ��̂Œl�����̂܂܎g�p
			transform = channelIt->timeline[0];
		}
		else
		{
			Timeline::iterator startIt = timeline.begin();
			if (anime.nowTime <= startIt->first)
			{
				// �擪�L�[�����O�̎��ԂȂ�A�擪�̒l���g�p
				transform = startIt->second;
			}
			else if (timeline.rbegin()->first <= anime.nowTime)
			{
				// �ŏI�L�[������̎��ԂȂ�A�Ō�̒l���g�p
				transform = timeline.rbegin()->second;
			}
			else
			{
				// �w�肳�ꂽ���Ԃ�����2�̃L�[����A��Ԃ��ꂽ�l���v�Z
				Timeline::iterator nextIt = timeline.upper_bound(anime.nowTime);
				startIt = nextIt;
				--startIt;
				float rate = (anime.nowTime - startIt->first) / (nextIt->first - startIt->first);
				LerpTransform(&transform, startIt->second, nextIt->second, rate);
			}
		}

		++channelIt;
	}
}


/* ========================================
	�A�j���[�V�����X�V�֐�
	-------------------------------------
	���e�F�w�肵���A�j���[�V�����̍Đ����Ԃ��X�V
	-------------------------------------
	����1�F�A�j���[�V�����ԍ�
	����2�F�o�ߎ���
=========================================== */
void ModelAnime::UpdateAnime(AnimeNo no, float tick)
{
	if (no == PARAMETRIC_ANIME) { return; }

	T_Animation& anime = m_AnimeList[no];
	anime.nowTime += anime.speed * tick;
	if (anime.isLoop)
	{
		while (anime.nowTime >= anime.totalTime)
		{
			anime.nowTime -= anime.totalTime;
		}
	}
}


/* ========================================
	�{�[���v�Z�֐�
	-------------------------------------
	���e�F�{�[���̎p���s����v�Z
	-------------------------------------
	����1�F�{�[���ԍ�
	����2�F�e�m�[�h�̎p���s��
=========================================== */
void ModelAnime::CalcBones(NodeIndex index, const DirectX::XMMATRIX parent)
{
	//--- �A�j���[�V�������Ƃ̃p�����[�^������
	T_Transform transform;
	// �p�����g���b�N
	if (m_nActiveNo == PARAMETRIC_ANIME || m_nBlendNo == PARAMETRIC_ANIME)
	{
		LerpTransform(&transform, m_NodeTransform[PARAMETRIC0][index], m_NodeTransform[PARAMETRIC1][index], m_fParametricBlendRatio);
		if (m_nActiveNo == PARAMETRIC_ANIME)
		{
			m_NodeTransform[MAIN][index] = transform;
		}
		if (m_nBlendNo == PARAMETRIC_ANIME)
		{
			m_NodeTransform[BLEND][index] = transform;
		}
	}
	// �u�����h�A�j��
	if (m_nBlendNo != ANIME_NONE)
	{
		LerpTransform(&transform, m_NodeTransform[MAIN][index], m_NodeTransform[BLEND][index], m_fBlendTime / m_fBlendTimeTotal);
	}
	else
	{
		// ���C���A�j���̂�
		transform = m_NodeTransform[MAIN][index];
	}

	// �Y���m�[�h�̎p���s����v�Z
	T_Node& node = m_NodeList[index];
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&transform.position));
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&transform.quaternion));
	DirectX::XMMATRIX S = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&transform.scale));
	node.mat = (S * R * T) * parent;

	// �q�v�f�̎p�����X�V
	Children::iterator it = node.children.begin();
	while (it != node.children.end())
	{
		CalcBones(*it, node.mat);
		++it;
	}
}


/* ========================================
	��Ԋ֐�
	-------------------------------------
	���e�F2�̎p������
	-------------------------------------
	����1�F��Ԍ��ʂ��i�[����ϐ�
	����2�F��Ԍ�A
	����3�F��Ԍ�B
	����4�F��ԗ�
=========================================== */
void ModelAnime::LerpTransform(T_Transform* pOut, const T_Transform& a, const T_Transform& b, float rate)
{
	DirectX::XMVECTOR vec[][2] = {
		{ DirectX::XMLoadFloat3(&a.position),	DirectX::XMLoadFloat3(&b.position) },
		{ DirectX::XMLoadFloat4(&a.quaternion),	DirectX::XMLoadFloat4(&b.quaternion) },
		{ DirectX::XMLoadFloat3(&a.scale),		DirectX::XMLoadFloat3(&b.scale) },
	};
	for (int i = 0; i < 3; ++i)
	{
		vec[i][0] = DirectX::XMVectorLerp(vec[i][0], vec[i][1], rate);
	}
	DirectX::XMStoreFloat3(&pOut->position, vec[0][0]);
	DirectX::XMStoreFloat4(&pOut->quaternion, vec[1][0]);
	DirectX::XMStoreFloat3(&pOut->scale, vec[2][0]);
}



/* ========================================
	�Q�b�^�[(�Đ����A�j���ԍ�)�֐�
	-------------------------------------
	�ߒl�Fint	�Đ����A�j���ԍ�
=========================================== */
ModelAnime::AnimeNo ModelAnime::GetPlayNo()
{
	return m_nActiveNo;
}

/* ========================================
	�Q�b�^�[(�Đ����u�����h�A�j���ԍ�)�֐�
	-------------------------------------
	�ߒl�Fint	�Đ����u�����h�A�j���ԍ�
=========================================== */
ModelAnime::AnimeNo ModelAnime::GetBlendNo()
{
	return m_nBlendNo;
}

/* ========================================
	�Q�b�^�[(���f����)�֐�
	-------------------------------------
	�ߒl�Fstd::string	���f����
=========================================== */
std::string ModelAnime::GetModelName()
{
	return m_sModelName;
}

#ifdef _DEBUG

/* ========================================
	�Q�b�^�[(�G���[���b�Z�[�W)�֐�
	-------------------------------------
	�ߒl�Fstd::string	�G���[���b�Z�[�W
=========================================== */
std::string ModelAnime::GetError()
{
	return m_sErrorMessage;
}


/* ========================================
	�{�[���f�o�b�O�`��֐�
	-------------------------------------
	���e�F�{�[����`�悷��
=========================================== */
void ModelAnime::DrawBone(Vector3<float> vPos, Quaternion qRot, Vector3<float> vScale)
{
	m_pBoneLine->SetPos(vPos);
	m_pBoneLine->SetRotation(qRot);
	m_pBoneLine->SetScale(vScale);

	m_nBoneCnt = 0;
	// �ċA����
	std::function<void(int, DirectX::XMFLOAT3)> FuncDrawBone =
		[&FuncDrawBone, this](int idx, DirectX::XMFLOAT3 parent)
	{
		// �e�m�[�h���猻�݈ʒu�܂ŕ`��
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, DirectX::XMVector3TransformCoord(DirectX::XMVectorZero(), m_NodeList[idx].mat));

		Vector3<float> start, end;
		start = parent;
		end = pos;

		m_pBoneLine->UpdateLine(m_nBoneCnt, start, end, ColorVec3::RED);
		++m_nBoneCnt;
		// �q�m�[�h�̕`��
		auto it = m_NodeList[idx].children.begin();
		while (it != m_NodeList[idx].children.end())
		{
			FuncDrawBone(*it, pos);
			++it;
		}
	};

	// �`����s
	FuncDrawBone(0, DirectX::XMFLOAT3());
	m_pBoneLine->Draw();
}

#endif