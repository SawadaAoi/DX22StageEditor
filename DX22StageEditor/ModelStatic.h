/* ========================================
	DX22Base/
	------------------------------------
	�ÓI���f���p�w�b�_
	------------------------------------
	�����F�ÓI���f�����������߂̃N���X
	------------------------------------
	ModelStatic.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include <DirectXMath.h>
#include <vector>
#include "VertexShader.h"
#include "PixelShader.h"
#include "MeshBuffer.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "ObjectLight.h"


// =============== �N���X��` =====================
class ModelStatic
{
public:
	enum E_Flip
	{
		None,	// DirectX����(���]����
		XFlip,	// Maya����
		ZFlip	// DirectX����(Maya����180��]�������
	};
public:
	// ���_�\����
	struct T_Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT4 color;
	};
	using Vertices = std::vector<T_Vertex>;		// ���_�z��
	using Indexs = std::vector<unsigned long>;	// �C���f�b�N�X�z��
	
	// �}�e���A���\����
	struct T_Material
	{
		DirectX::XMFLOAT4 diffuse;
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 specular;
		Texture* pTexture;
	};
	using Materials = std::vector<T_Material>;	// �}�e���A���z��

	// ���b�V���\����
	struct T_Mesh
	{
		Vertices				vertices;
		Indexs					indices;
		unsigned int			materialID;
		std::vector<uint32_t>	boneID;
		MeshBuffer*				pMesh;
	};
	using Meshes = std::vector<T_Mesh>;

public:
	ModelStatic();
	~ModelStatic();

	void Draw();
	void Reset();
	bool Load(const char* file, float scale = 1.0f, E_Flip flip = E_Flip::None);

	// �Q�b�^�[
	const T_Mesh* GetMesh(unsigned int index);
	unsigned int GetMeshSize();

	// �Z�b�^�[
	void SetVertexShader(VertexShader* vs);
	void SetPixelShader(PixelShader* ps);

	void SetWVPMatrix(const DirectX::XMFLOAT4X4* matWVP);	// ���[���h�s��̐ݒ�(���_�V�F�[�_�[�p)
	void SetLightMaterial(float fDiffuse, float fSpecular, float fAmbient, bool bUseLight);		// ���C�g�p�����[�^�ݒ�(�s�N�Z���V�F�[�_�[�p)
	void SetCameraPos(Vector3<float> fCameraPos);												// �J�����ʒu�ݒ�(�s�N�Z���V�F�[�_�[�p)
	void SetLights(std::vector<ObjectLight*> lights);											// ���C�g�ݒ�(�s�N�Z���V�F�[�_�[�p)

#ifdef _DEBUG
	static std::string GetError();
#endif



private:
	void MakeMesh(const void* ptr, float scale, E_Flip flip);
	void MakeMaterial(const void* ptr, std::string directory);
private:
#ifdef _DEBUG
	static inline std::string		m_sErrorMessage;
#endif

private:
	Meshes					m_MeshList;
	Materials				m_MaterialList;
	VertexShader*			m_pVS;
	PixelShader*			m_pPS;

};


