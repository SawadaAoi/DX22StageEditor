/* ========================================
	DX22Base/
	------------------------------------
	�A�j���[�V�������f���p�w�b�_
	------------------------------------
	�����F�A�j���[�V�������f�����������߂̃N���X
		�@���u�����h�Đ��́A�A�j��A���A�j��B�֊����ɑJ�ڂ���Đ����@
		�@�������Đ��́A�A�j��A�ƃA�j��B���������čĐ�����Đ����@
	------------------------------------
	ModelAnime.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include <DirectXMath.h>
#include <vector>
#include "VertexShader.h"
#include "PixelShader.h"
#include "MeshBuffer.h"
#include <functional>
#include "ShapeLine.h"
#include <memory>
#include "ColorVec3.h"
#include "ObjectLight.h"

#include <assimp/Importer.hpp>


// =============== �N���X��` =====================
class ModelAnime
{
public:
	// ���f�����]�ݒ�
	enum E_Flip
	{
		None,			// DirectX����(���]����
		XFlip,			// Maya����
		ZFlip,			// DirectX����(Maya����180��]�������
		ZFlipUseAnime,	// DirecX����(�A�j���[�V����������ꍇ
	};

private:
	// �A�j���[�V�����v�Z�̈�
	enum E_AnimeTransform
	{
		MAIN,			// �ʏ�Đ�
		BLEND,			// �u�����h�Đ�
		PARAMETRIC0,	// ����A
		PARAMETRIC1,	// ����B
		MAX_TRANSFORM
	};

public:
	// �^��`
	using NodeIndex = int;	// �{�[��(�K�w)�ԍ�
	using AnimeNo = int;	// �A�j���[�V�����ԍ�

	// �萔��`
	static const NodeIndex	INDEX_NONE = -1;		// �Y���m�[�h�Ȃ�
	static const AnimeNo	ANIME_NONE = -1;		// �Y���A�j���[�V�����Ȃ�
	static const AnimeNo	PARAMETRIC_ANIME = -2;	// �����A�j���[�V����

private:
	// �����^��`
	using Children = std::vector<NodeIndex>;	// �m�[�h�K�w���

	// �����萔��`
	static const UINT		MAX_BONE = 200;	// �P���b�V���̍ő�{�[����(������ύX����ꍇ.hlsl���̒�`���ύX����

	// �A�j���[�V�����̕ϊ����
	struct T_Transform
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT4	quaternion;
		DirectX::XMFLOAT3	scale;
	};
	using Key = std::pair<float, T_Transform>;		// ���� / �ϊ����
	using Timeline = std::map<float, T_Transform>;	// ���� / �ϊ����
	using Transforms = std::vector<T_Transform>;

	// �A�j���[�V�����ƃ{�[���̊֘A�t�����
	struct T_Channel
	{
		NodeIndex	index;		// �{�[���ԍ�
		Timeline	timeline;	// �ϊ����
	};
	using Channels = std::vector<T_Channel>;

	// �{�[�����
	struct T_Node
	{
		std::string			name;		// �{�[����
		NodeIndex			parent;		// �e�{�[��
		Children			children;	// �q�{�[��
		DirectX::XMMATRIX	mat;		// �ϊ��s��
	};
	using Nodes = std::vector<T_Node>;


public:
	// ���_���
	struct T_Vertex
	{
		DirectX::XMFLOAT3	pos;
		DirectX::XMFLOAT3	normal;
		DirectX::XMFLOAT2	uv;
		DirectX::XMFLOAT4	color;
		float				weight[4];
		unsigned int		index[4];
	};
	using Vertices = std::vector<T_Vertex>;
	using Indices = std::vector<unsigned long>;

	// ���_�̍��ό`���
	struct T_Bone
	{
		NodeIndex index;
		DirectX::XMMATRIX invOffset;
	};
	using Bones = std::vector<T_Bone>;

	// ���b�V��
	struct T_Mesh
	{
		Vertices		vertices;
		Indices			indices;
		unsigned int	materialID;
		Bones			bones;
		MeshBuffer*		pMesh;
	};
	using Meshes = std::vector<T_Mesh>;

	// �}�e���A�����
	struct T_Material
	{
		DirectX::XMFLOAT4	diffuse;	// �g�U��(���C���J���[
		DirectX::XMFLOAT4	ambient;	// ����(�A�̕����̃J���[
		DirectX::XMFLOAT4	specular;	// ���ʔ��ˌ�(�������镔���̃J���[
		Texture*			pTexture;	// �e�N�X�`��
	};
	using Materials = std::vector<T_Material>;

	// �A�j���[�V�������
	struct T_Animation
	{
		float		nowTime;	// ���݂̍Đ�����
		float		totalTime;	// �ő�Đ�����
		float		speed;		// �Đ����x
		bool		isLoop;		// ���[�v�w��
		Channels	channels;	// �ϊ����
		std::string sName;		// �A�j���[�V������
	};
	using Animations = std::vector<T_Animation>;

public:
	ModelAnime();
	~ModelAnime();
	ModelAnime(const ModelAnime& other);
	void Update(float tick);					// �A�j���[�V�����X�V
	void Draw(const std::vector<UINT>* order = nullptr);

	void Reset();
	bool Load(const char* file, float scale = 1.0f, E_Flip flip = E_Flip::None);

	// �Q�b�^�[
	const T_Mesh* GetMesh(unsigned int index);
	uint32_t GetMeshNum();
	const T_Material* GetMaterial(unsigned int index);
	uint32_t GetMaterialNum();
	DirectX::XMMATRIX GetBone(NodeIndex index);
	const T_Animation* GetAnimation(AnimeNo no);

	// �Z�b�^�[
	void SetVertexShader(VertexShader* vs);
	void SetPixelShader(PixelShader* ps);
	void SetWVPMatrix(const DirectX::XMFLOAT4X4* matWVP);										// ���[���h�s��̐ݒ�(���_�V�F�[�_�[�p)
	void SetLightMaterial(float fDiffuse, float fSpecular, float fAmbient, bool bUseLight);		// ���C�g�p�����[�^�ݒ�(�s�N�Z���V�F�[�_�[�p)
	void SetCameraPos(Vector3<float> fCameraPos);												// �J�����ʒu�ݒ�(�s�N�Z���V�F�[�_�[�p)
	void SetLights(std::vector<ObjectLight*> lights);											// ���C�g�ݒ�(�s�N�Z���V�F�[�_�[�p)

public:
	//--- �A�j���[�V����
	AnimeNo AddAnimation(const char* file);		// �A�j���[�V�����̒ǉ�

	void Play(AnimeNo no, bool loop, float speed = 1.0f);						// �Đ�
	void PlayBlend(AnimeNo no, float blendTime, bool loop, float speed = 1.0f);	// �u�����h�Đ�
	void SetParametric(AnimeNo no1, AnimeNo no2);								// �����ݒ�
	void SetParametricBlend(float blendRate);									// ���������ݒ�
	void SetAnimationTime(AnimeNo no, float time);								// ���ݍĐ����Ԃ�ύX
	void SetAnimationTimeRatio(AnimeNo no, float rate);						// �Đ�������ύX
	float GetAnimeNowTime(AnimeNo no);											// ���ݍĐ����Ԃ��擾
	float GetAnimeTotalTime(AnimeNo no);										// �A�j���[�V�����̑��Đ����Ԃ��擾
	float GetAnimePlaybackRatio(AnimeNo no);									// �A�j���[�V�����Đ��������擾
	void SetAnimationSpeed(AnimeNo no, float speed);							// �Đ����x��ύX

	bool IsPlay(AnimeNo no);	// �Đ������ǂ���
	AnimeNo GetPlayNo();		// �Đ����̃A�j���ԍ�
	AnimeNo GetBlendNo();		// �u�����h���̃A�j���ԍ�
	std::string GetModelName();	// ���f�����擾

#ifdef _DEBUG
	static std::string GetError();
	void DrawBone(Vector3<float> vPos = { 0.0f,0.0f,0.0f }, Quaternion qRot = {}, Vector3<float> vScale = {1.0f,1.0f,1.0f});
#endif


private:
	// �e�퐶��
	void MakeMesh(const void* ptr, float scale, E_Flip flip);
	void MakeMaterial(const void* ptr, std::string directory);
	void MakeAnimation(const void* ptr);
	void MakeBoneNodes(const void* ptr);
	void MakeWeight(const void* ptr, int meshIdx);

	// Assimp�s��ϊ�
	static DirectX::XMMATRIX GetMatrixFromAssimpMatrix(aiMatrix4x4 M);


	// �����v�Z
	bool AnimeNoCheck(AnimeNo no);
	void InitAnime(AnimeNo no);
	void CalcAnime(E_AnimeTransform kind, AnimeNo no);
	void UpdateAnime(AnimeNo no, float tick);
	void CalcBones(NodeIndex node, const DirectX::XMMATRIX parent);
	void LerpTransform(T_Transform* pOut, const T_Transform& a, const T_Transform& b, float rate);

	std::string GetFBXDataName(const char* file);
private:
#ifdef _DEBUG
	static inline std::string m_sErrorMessage;
	std::unique_ptr<ShapeLine> m_pBoneLine;
	int m_nBoneCnt;
#endif

private:
	float			m_fLoadScale;	// �Ǎ����ݒ�X�P�[��
	E_Flip			m_LoadFlip;		// �Ǎ����ݒ蔽�]

	Meshes			m_MeshList;		// ���b�V���z��
	Materials		m_MaterialList;	// �}�e���A���z��
	Nodes			m_NodeList;		// �K�w���
	Animations		m_AnimeList;		// �A�j���z��
	VertexShader*	m_pVS;			// �ݒ蒆�̒��_�V�F�[�_
	PixelShader*	m_pPS;			// �ݒ蒆�̃s�N�Z���V�F�[�_

	AnimeNo			m_nActiveNo;			// ���ݍĐ����̃A�j���ԍ�
	AnimeNo			m_nBlendNo;				// �u�����h�Đ����s���A�j���ԍ�
	AnimeNo			m_nParametricAnimeNos[2];	// �����Đ����s���A�j���ԍ�
	float			m_fBlendTime;			// ���݂̑J�ڌo�ߎ���
	float			m_fBlendTimeTotal;		// �A�j���J�ڂɂ����鍇�v����
	float			m_fParametricBlendRatio;			// �p�����g���b�N�̍Đ�����

	Transforms		m_NodeTransform[MAX_TRANSFORM];	// �A�j���[�V�����ʕό`���

	std::string		m_sModelName;	// �f�B���N�g���p�X
};
