/* ========================================
	DX22Base/
	------------------------------------
	�A�j���[�V�������f���pcpp
	------------------------------------
	ModelAnime.cpp
========================================== */


// =============== �C���N���[�h ===================
#include "ModelAnime.h"
#include "ShaderManager.h"
#include "ShadowManager.h"

#include "LightManager.h"

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
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
=========================================== */
ModelAnime::ModelAnime()
	: m_fLoadScale(1.0f)
	, m_LoadFlip(None)
	, m_nActiveNo(ANIME_NONE)
	, m_nBlendNo(ANIME_NONE)
	, m_nParametricAnimeNos{ ANIME_NONE, ANIME_NONE }
	, m_fBlendTime(0.0f)
	, m_fBlendTimeTotal(0.0f)
	, m_fParametricBlendRatio(0.0f)
	, m_sModelName("")
#ifdef _DEBUG
	, m_pBoneLine(nullptr)
	, m_nBoneCnt(0)
#endif // _DEBUG
{

	m_pVS = GET_VS_DATA(VS_KEY::VS_MODEL_ANIME);
	m_pPS = GET_PS_DATA(PS_KEY::PS_MODEL_ANIME);
}


/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F���
=========================================== */
ModelAnime::~ModelAnime()
{
	Reset();
}

/* ========================================
	�R�s�[�R���X�g���N�^
	-------------------------------------
	���e�F�R�s�[�R���X�g���N�^
		�@���A�j�����f���̃f�B�[�v�R�s�[���p
=========================================== */
ModelAnime::ModelAnime(const ModelAnime& other)
	: m_fLoadScale(0.0f)
	, m_LoadFlip(E_Flip::None)
	, m_NodeList()
	, m_AnimeList()
	, m_pVS(nullptr)
	, m_pPS(nullptr)
	, m_nActiveNo(0)
	, m_nBlendNo(0)
	, m_nParametricAnimeNos{0,0}
	, m_fBlendTime(0.0f)
	, m_fBlendTimeTotal(0.0f)
	, m_fParametricBlendRatio(0.0f)
	, m_sModelName("")
#ifdef _DEBUG
	, m_pBoneLine(nullptr)
	, m_nBoneCnt(0)
#endif // _DEBUG
{
	if (this == &other)	return;

	this->m_fLoadScale				= other.m_fLoadScale;
	this->m_LoadFlip				= other.m_LoadFlip;
	this->m_NodeList				= other.m_NodeList;
	this->m_AnimeList				= other.m_AnimeList;
	this->m_pVS						= other.m_pVS;
	this->m_pPS						= other.m_pPS;
	this->m_nActiveNo				= other.m_nActiveNo;
	this->m_nBlendNo				= other.m_nBlendNo;
	this->m_nParametricAnimeNos[0]	= other.m_nParametricAnimeNos[0];
	this->m_nParametricAnimeNos[1]	= other.m_nParametricAnimeNos[1];
	this->m_fBlendTime				= other.m_fBlendTime;
	this->m_fBlendTimeTotal			= other.m_fBlendTimeTotal;
	this->m_fParametricBlendRatio	= other.m_fParametricBlendRatio;
	this->m_sModelName				= other.m_sModelName;
#ifdef _DEBUG
	this->m_pBoneLine = std::make_unique<ShapeLine>(*other.m_pBoneLine);
#endif // _DEBUG

	// �e�ϐ������|�C���^���f�B�[�v�R�s�[����
	
	// ���b�V���z��
	this->m_MeshList = other.m_MeshList;
	for (auto& mesh : this->m_MeshList)
	{
		mesh.pMesh = new MeshBuffer(*mesh.pMesh);
	}
	// �}�e���A���z��
	this->m_MaterialList = other.m_MaterialList;
	for (auto& mat : this->m_MaterialList)
	{
		mat.pTexture = new Texture(*mat.pTexture);
	}
	// �A�j���[�V�����ʕό`���

	for (int i = 0; i < MAX_TRANSFORM; ++i)
	{
		m_NodeTransform[i].resize(other.m_NodeTransform[i].size());
		for (int j = 0; j < other.m_NodeTransform[i].size(); ++j)
		{
			m_NodeTransform[i][j] = other.m_NodeTransform[i][j];
		}
	}
	

}


/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F���t���[���Ăяo���X�V����
	-------------------------------------
	�����F�O�t���[������̌o�ߎ���
=========================================== */
void ModelAnime::Update(float tick)
{
	// �A�j���[�V�����̍Đ��m�F
	if (m_nActiveNo == ANIME_NONE) { return; }

	//--- �A�j���[�V�����s��̍X�V
	// �p�����g���b�N
	if (m_nActiveNo == PARAMETRIC_ANIME || m_nBlendNo == PARAMETRIC_ANIME)
	{
		CalcAnime(PARAMETRIC0, m_nParametricAnimeNos[0]);
		CalcAnime(PARAMETRIC1, m_nParametricAnimeNos[1]);
	}
	// ���C���A�j��
	if (m_nActiveNo != ANIME_NONE && m_nActiveNo != PARAMETRIC_ANIME)
	{
		CalcAnime(MAIN, m_nActiveNo);
	}
	// �u�����h�A�j��
	if (m_nBlendNo != ANIME_NONE && m_nBlendNo != PARAMETRIC_ANIME)
	{
		CalcAnime(BLEND, m_nBlendNo);
	}

	// �A�j���[�V�����s��Ɋ�Â��č��s����X�V
	CalcBones(0, DirectX::XMMatrixScaling(m_fLoadScale, m_fLoadScale, m_fLoadScale));

	//--- �A�j���[�V�����̎��ԍX�V
	// ���C���A�j��
	UpdateAnime(m_nActiveNo, tick);
	// �u�����h�A�j��
	if (m_nBlendNo != ANIME_NONE)
	{
		UpdateAnime(m_nBlendNo, tick);
		m_fBlendTime += tick;
		if (m_fBlendTime >= m_fBlendTimeTotal)	// �u�����h�A�j���̏I������
		{
			// �u�����h�A�j���̎����I��
			m_fBlendTime = 0.0f;
			m_fBlendTimeTotal = 0.0f;
			m_nActiveNo = m_nBlendNo;
			m_nBlendNo = ANIME_NONE;
		}
	}
	// �p�����g���b�N
	if (m_nActiveNo == PARAMETRIC_ANIME || m_nBlendNo == PARAMETRIC_ANIME)
	{
		UpdateAnime(m_nParametricAnimeNos[0], tick);
		UpdateAnime(m_nParametricAnimeNos[1], tick);
	}
}

/* ========================================
	�`�揈���֐�
	-------------------------------------
	���e�F�`��
	-------------------------------------
	�����Fconst std::vector<UINT>*	�`�揇��
=========================================== */
void ModelAnime::Draw(const std::vector<UINT>* order)
{
	if (m_pPS == nullptr || m_pVS == nullptr) { return; }

	// �`�搔�ݒ�
	size_t drawNum = m_MeshList.size();

	// �`�揇�Ԏw�肪����ꍇ
	if (order)
	{
		drawNum = order->size();
	}

	// �`��
	for (UINT i = 0; i < drawNum; ++i)
	{
		// ���b�V���ԍ�
		UINT meshNo;

		// �`�揇�Ԏw�肪����ꍇ
		if (order)
		{
			meshNo = (*order)[i];	// �z�񂩂烁�b�V���ԍ��擾
		}
		else
		{
			meshNo = i;
		}

		
		// �`��R�[���o�b�N
		const T_Mesh*		pMesh	= this->GetMesh(meshNo);
		const T_Material& pMaterial	= *this->GetMaterial(pMesh->materialID);

		m_pPS->SetTexture(0, pMaterial.pTexture);	// �e�N�X�`���Z�b�g
		m_pPS->Bind();


		DirectX::XMFLOAT4X4 bones[200];
		for (int i = 0; i < pMesh->bones.size() && i < 200; ++i)
		{
			// ���̌v�Z�̓Q�[������[�u�X�L�����b�V���̎d�g�݁v���Q�l�ɂȂ�
			DirectX::XMStoreFloat4x4(&bones[i], DirectX::XMMatrixTranspose(
				pMesh->bones[i].invOffset *
				this->GetBone(pMesh->bones[i].index)
			));
		}
		m_pVS->WriteBuffer(1, bones);
		m_pVS->Bind();

		// �`��
		m_MeshList[meshNo].pMesh->Draw();
	}

}

/* ========================================
	���Z�b�g�֐�
	-------------------------------------
	���e�F���b�V���z��ƃ}�e���A���z������
=========================================== */
void ModelAnime::Reset()
{
	auto meshIt = m_MeshList.begin();
	while (meshIt != m_MeshList.end())
	{
		if (meshIt->pMesh)	delete meshIt->pMesh;
		++meshIt;
	}

	auto matIt = m_MaterialList.begin();
	while (matIt != m_MaterialList.end())
	{
		if (matIt->pTexture) delete matIt->pTexture;
		++matIt;
	}
}

/* ========================================
	�A�j�����f���f�[�^�Ǎ��֐�
	-------------------------------------
	���e�F�A�j�����f���f�[�^��ǂݍ���
	-------------------------------------
	����1: const char*	���f���f�[�^�p�X
	����2: float		���f���̃T�C�Y
	����3: Flip			���]�ݒ�
	-------------------------------------
	�ߒl�Fbool	true:����	false:���s
=========================================== */
bool ModelAnime::Load(const char* file, float scale, E_Flip flip)
{
#ifdef _DEBUG
	m_sErrorMessage = "";
#endif
	Reset();

	// assimp�̐ݒ�
	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_FlipUVs;
	if (flip == E_Flip::XFlip)  flag |= aiProcess_MakeLeftHanded;

	// assimp�œǂݍ���
	const aiScene* pScene = importer.ReadFile(file, flag);
	if (!pScene) {
#ifdef _DEBUG
		m_sErrorMessage = importer.GetErrorString();
#endif
		return false;
	}

	// �ǂݍ��ݎ��̐ݒ��ۑ�
	m_fLoadScale = scale;
	m_LoadFlip = flip;

	// �f�B���N�g���̓ǂݎ��
	std::string directory = file;
	auto strIt = directory.begin();
	while (strIt != directory.end()) {
		if (*strIt == '/')
			*strIt = '\\';
		++strIt;
	}
	directory = directory.substr(0, directory.find_last_of('\\') + 1);

	// �m�[�h�̍쐬
	MakeBoneNodes(pScene);
	// ���b�V���쐬
	MakeMesh(pScene, scale, flip);
	// �}�e���A���̍쐬
	MakeMaterial(pScene, directory);


#ifdef _DEBUG
	m_pBoneLine = std::make_unique<ShapeLine>(250);

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

		m_pBoneLine->AddLine(start, end, ColorVec3::RED);
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
#endif // _DEBUG


	m_sModelName = GetFBXDataName(file);

	return true;
}


/* ========================================
	FBX�f�[�^���擾�֐�
	-------------------------------------
	���e�F�t�@�C���p�X����FBX�f�[�^��(���f����A�j���[�V����)���擾
	-------------------------------------
	�����Fconst char*	�t�@�C���p�X
	-------------------------------------
	�ߒl�Fstd::string	FBX�f�[�^��
=========================================== */
std::string ModelAnime::GetFBXDataName(const char* file)
{
	std::string path = file;

	// �p�X���疖���̃t�@�C�������擾
	path = path.substr(path.find_last_of('/') + 1, path.size());
	// �g���q���폜
	const std::string extension = ".fbx";
	if (path.size() >= extension.size() &&
		path.rfind(extension) == path.size() - extension.size()) {
		// ".fbx" ���폜
		path.erase(path.size() - extension.size());
	}

	return path;
}



/* ========================================
	�Q�b�^�[(���b�V��)�֐�
	-------------------------------------
	�����F���b�V���ԍ�
	-------------------------------------
	�ߒl�FMesh*�@���b�V��
=========================================== */
const ModelAnime::T_Mesh* ModelAnime::GetMesh(unsigned int index)
{
	if (index < GetMeshNum())
	{
		return &m_MeshList[index];
	}
	return nullptr;
}


/* ========================================
	�Q�b�^�[(���b�V���z��T�C�Y)�֐�
	-------------------------------------
	�ߒl�Fuint32_t�@���b�V���z��T�C�Y
=========================================== */
uint32_t ModelAnime::GetMeshNum()
{
	return static_cast<uint32_t>(m_MeshList.size());
}

/* ========================================
	�Q�b�^�[(�}�e���A��)�֐�
	-------------------------------------
	�����F�}�e���A���ԍ�
	-------------------------------------
	�ߒl�FMaterial*�@�}�e���A��
=========================================== */
const ModelAnime::T_Material* ModelAnime::GetMaterial(unsigned int index)
{
	if (index < GetMaterialNum())
	{
		return &m_MaterialList[index];
	}
	return nullptr;
}

/* ========================================
	�Q�b�^�[(�}�e���A���z��T�C�Y)�֐�
	-------------------------------------
	�ߒl�Fuint32_t�@�}�e���A���z��T�C�Y
=========================================== */
uint32_t ModelAnime::GetMaterialNum()
{
	return static_cast<uint32_t>(m_MaterialList.size());
}


/* ========================================
	�Q�b�^�[(�A�j���[�V������ϊ��s��)�֐�
	-------------------------------------
	�����FNodeIndex	�{�[���ԍ�
	-------------------------------------
	�ߒl�FDirectX::XMMATRIX�@�Y���{�[���̕ϊ��s��
=========================================== */
DirectX::XMMATRIX ModelAnime::GetBone(NodeIndex index)
{
	if (index < m_NodeList.size())
	{
		return m_NodeList[index].mat;
	}
	return DirectX::XMMatrixIdentity();
}

/* ========================================
	�Q�b�^�[(�A�j���[�V�������z��)�֐�
	-------------------------------------
	�����FAnimeNo �A�j���ԍ�
	-------------------------------------
	�ߒl�FModelAnime::T_Animation*�@�Y���A�j���[�V�������
=========================================== */
const ModelAnime::T_Animation* ModelAnime::GetAnimation(AnimeNo no)
{
	if (AnimeNoCheck(no))
	{
		return &m_AnimeList[no];
	}
	return nullptr;
}

/* ========================================
	�Z�b�^�[(���_�V�F�[�_�[)�֐�
	-------------------------------------
	�����FVertexShader*�@���_�V�F�[�_�[
=========================================== */
void ModelAnime::SetVertexShader(VertexShader* vs)
{
	m_pVS = vs;
}

/* ========================================
	�Z�b�^�[(�s�N�Z���V�F�[�_�[)�֐�
	-------------------------------------
	�����FPixelShader*�@�s�N�Z���V�F�[�_�[
=========================================== */
void ModelAnime::SetPixelShader(PixelShader* ps)
{
	m_pPS = ps;
}

/* ========================================
	�Z�b�^�[(���[���h�r���[�v���W�F�N�V�����s��)�֐�
	-------------------------------------
	�����FXMFLOAT4X4*�@�s��|�C���^(�z��)
=========================================== */
void ModelAnime::SetWVPMatrix(const DirectX::XMFLOAT4X4* matWVP)
{
	m_pVS->WriteBuffer(0, matWVP);
}


/* ========================================
	�Z�b�^�[(���C�g�ݒ�(�}�e���A��))�֐�
	-------------------------------------
	����1�Ffloat	�g�U��
	����2�Ffloat	���ʔ��ˌ�
	����3�Ffloat	����
	����4�Fbool		���C�g�g�p�t���O
=========================================== */
void ModelAnime::SetLightMaterial(float fDiffuse, float fSpecular, float fAmbient, bool bUseLight)
{
	DirectX::XMFLOAT4 param[] =
	{
		{fDiffuse, fSpecular, fAmbient, bUseLight ? 1.0f : 0.0f}
	};

	m_pPS->WriteBuffer(0, param);
}

/* ========================================
	�Z�b�^�[(�J�������W)�֐�
	-------------------------------------
	����1�FVector3<float>	�J�������W
=========================================== */
void ModelAnime::SetCameraPos(Vector3<float> fCameraPos)
{
	m_pPS->WriteBuffer(1, &fCameraPos);
}

/* ========================================
	�Z�b�^�[(���C�g���)�֐�
	-------------------------------------
	�����Fstd::vector<ObjectLight*>	�V�[����̃��C�g���X�g
=========================================== */
void ModelAnime::SetLights(std::vector<ObjectLight*> lights)
{
	DirectX::XMFLOAT4 param[MAX_LIGHT_NUM][4];

	// ������
	for (int i = 0; i < MAX_LIGHT_NUM; i++)
	{
		param[i][0] = { 0.0f, 0.0f, 0.0f, 0.0f };
		param[i][1] = { 0.0f, 0.0f, 0.0f, 0.0f };
		param[i][2] = { 0.0f, 0.0f, 0.0f, 0.0f };
		param[i][3] = { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	// ���C�g�����Z�b�g
	for (int i = 0; i < lights.size(); ++i)
	{
		float			lightType	= static_cast<float>(lights[i]->GetLightType());
		Vector3<float>	lightPos	= lights[i]->GetPosition();
		Vector3<float>	lightColor	= lights[i]->GetColor();
		Vector3<float>	lightDir	= lights[i]->GetDirection();

		param[i][0] = { lightType, lightPos.x, lightPos.y, lightPos.z };			// ���C�g�^�C�v�A���C�g�ʒu
		param[i][1] = { lightColor.x, lightColor.y, lightColor.z, 1.0f };			// ���C�g�J���[
		param[i][2] = { lightDir.x, lightDir.y, lightDir.z, lights[i]->GetRange() };// ���C�g�����A���C�g�L���͈�
		param[i][3] = { lights[i]->GetAngle(), 0.0f, 0.0f, 0.0f };					// �X�|�b�g���C�g�p�x
	}
	// �ۉe�̏����Z�b�g
	int nParamNum = lights.size();
	std::vector<ComponentShadow*> shadows = SHADOW_MNG_INST.GetShadowList();
	for (int i = 0; i < shadows.size(); i++)
	{
		if (nParamNum >= MAX_LIGHT_NUM) break;	// ���C�g�A�e���v�����ő吔�𒴂�����I��

		Vector3<float>	vShadowPos = shadows.at(i)->GetPosition();

		param[nParamNum][0] = { 4.0f, vShadowPos.x, vShadowPos.y, vShadowPos.z };	// ���C�g�^�C�v(�e�͌Œ��4)�A�e�̊���W
		param[nParamNum][1] = { 0.0f, 0.0f, 0.0f, 0.0f };							// ���C�g�J���[(���g�p)
		param[nParamNum][2] = { 0.0f, 0.0f, 0.0f, shadows[i]->GetCircleSize() };	// ���C�g����(���g�p)�A���e�̉~�̑傫��
		param[nParamNum][3] = { 0.0f, 0.0f, 0.0f, 0.0f };							// �X�|�b�g���C�g�p�x(���g�p)

		nParamNum++;
	}

	m_pPS->WriteBuffer(2, param);
}




