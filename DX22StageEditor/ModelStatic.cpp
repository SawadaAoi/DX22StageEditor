/* ========================================
	DX22Base/
	------------------------------------
	�ÓI���f���pcpp
	------------------------------------
	ModelStatic.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ModelStatic.h"
#include "DirectXTex/TextureLoad.h"
#include "LightManager.h"
#include "ShadowManager.h"
#include "ComponentTransform.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ShaderManager.h"
#include "CameraManager.h"

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
ModelStatic::ModelStatic()
	: m_pVS(nullptr)
	, m_pPS(nullptr)
{
	m_pVS = GET_VS_DATA(VS_KEY::VS_MODEL_STATIC);
	m_pPS = GET_PS_DATA(PS_KEY::PS_MODEL_STATIC);
}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F���
=========================================== */
ModelStatic::~ModelStatic()
{
	Reset();	// ���b�V���z��ƃ}�e���A���z������
}

/* ========================================
	�`�揈���֐�
	-------------------------------------
	���e�F�`��
=========================================== */
void ModelStatic::Draw()
{
	if (m_pPS == nullptr || m_pVS == nullptr) { return; }

	for (unsigned int i = 0; i < m_MeshList.size(); ++i)
	{
		m_pPS->SetTexture(0,m_MaterialList[m_MeshList[i].materialID].pTexture);
		m_pPS->Bind();
		m_pVS->Bind();

		m_MeshList[i].pMesh->Draw();
	}
}

/* ========================================
	���Z�b�g�֐�
	-------------------------------------
	���e�F���b�V���z��ƃ}�e���A���z������
=========================================== */
void ModelStatic::Reset()
{
	auto meshIt = m_MeshList.begin();
	while (meshIt != m_MeshList.end())
	{
		if(meshIt->pMesh) delete meshIt->pMesh;
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
	���b�V���Ǎ��֐�
	-------------------------------------
	���e�F���b�V���f�[�^��ǂݍ���
	-------------------------------------
	����1: const char*	���f���f�[�^�p�X
	����2: float		���f���̃T�C�Y
	����3: Flip			���]�ݒ�
	-------------------------------------
	�ߒl�Fbool	true:����	false:���s
=========================================== */
bool ModelStatic::Load(const char* file, float scale, E_Flip flip)
{
#ifdef _DEBUG
	m_sErrorMessage = "";
#endif
	Reset();	// ���b�V���z��ƃ}�e���A���z������

	// assimp�̐ݒ�
	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;				// �O�p�`��
	flag |= aiProcess_JoinIdenticalVertices;	// ���꒸�_�̌���
	flag |= aiProcess_Triangulate;				// �O�p�`��
	flag |= aiProcess_FlipUVs;					// UV���]
	flag |= aiProcess_PreTransformVertices;		// ���_�̕ϊ�

	// ���]�ݒ�
	if (flip == E_Flip::XFlip)  flag |= aiProcess_MakeLeftHanded;	// ������W�n

	// assimp�œǂݍ���
	const aiScene* pScene = importer.ReadFile(file, flag);
	if (!pScene) {
#ifdef _DEBUG
		m_sErrorMessage = importer.GetErrorString();
#endif
		return false;
	}

	// �f�B���N�g���̓ǂݎ��
	std::string directory = file;
	auto strIt = directory.begin();
	while (strIt != directory.end()) {
		if (*strIt == '/')
			*strIt = '\\';
		++strIt;
	}
	directory = directory.substr(0, directory.find_last_of('\\') + 1);

	// ���b�V���쐬
	MakeMesh(pScene, scale, flip);
	// �}�e���A���̍쐬
	MakeMaterial(pScene, directory);

	return true;
}


/* ========================================
	�Q�b�^�[(���b�V��)�֐�
	-------------------------------------
	�����F���b�V���ԍ�
	-------------------------------------
	�ߒl�FMesh*�@���b�V��
=========================================== */
const ModelStatic::T_Mesh* ModelStatic::GetMesh(unsigned int index)
{
	// �͈͊O�`�F�b�N
	if (index < GetMeshSize())
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
uint32_t ModelStatic::GetMeshSize()
{
	return static_cast<uint32_t>(m_MeshList.size());
}


/* ========================================
	�Z�b�^�[(���_�V�F�[�_�[)�֐�
	-------------------------------------
	�����FVertexShader*�@���_�V�F�[�_�[
=========================================== */
void ModelStatic::SetVertexShader(VertexShader* vs)
{
	m_pVS = vs;
}

/* ========================================
	�Z�b�^�[(�s�N�Z���V�F�[�_�[)�֐�
	-------------------------------------
	�����FPixelShader*�@�s�N�Z���V�F�[�_�[
=========================================== */
void ModelStatic::SetPixelShader(PixelShader* ps)
{
	m_pPS = ps;
}

/* ========================================
	�Z�b�^�[(���[���h�r���[�v���W�F�N�V�����s��)�֐�
	-------------------------------------
	�����FXMFLOAT4X4*�@�s��|�C���^(�z��)
=========================================== */
void ModelStatic::SetWVPMatrix(const DirectX::XMFLOAT4X4* matWVP)
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
void ModelStatic::SetLightMaterial(float fDiffuse, float fSpecular, float fAmbient, bool bUseLight)
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
void ModelStatic::SetCameraPos(Vector3<float> fCameraPos)
{
	m_pPS->WriteBuffer(1, &fCameraPos);
}

/* ========================================
	�Z�b�^�[(���C�g���)�֐�
	-------------------------------------
	�����Fstd::vector<ObjectLight*>	�V�[����̃��C�g���X�g
=========================================== */
void ModelStatic::SetLights(std::vector<ObjectLight*> lights)
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
		float			lightType = static_cast<float>(lights[i]->GetLightType());
		Vector3<float>	lightPos = lights[i]->GetPosition();
		Vector3<float>	lightColor = lights[i]->GetColor();
		Vector3<float>	lightDir = lights[i]->GetDirection();

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


#ifdef _DEBUG
/* ========================================
	�Q�b�^�[(�G���[���b�Z�[�W)�֐�
	-------------------------------------
	�ߒl�Fstring�@�G���[���b�Z�[�W
=========================================== */
std::string ModelStatic::GetError()
{
	return m_sErrorMessage;
}
#endif