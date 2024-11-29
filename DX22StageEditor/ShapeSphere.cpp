/* ========================================
	DX22Base/
	------------------------------------
	�}�`�`��(��)�pcpp
	------------------------------------
	ShapeSphere.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ShapeSphere.h"
#include "vector"
#include <DirectXMath.h>

// =============== �萔��` =======================
// �ʏ�
const int LONGITUDE_COUNT_NORMAL	= 20;	// �o�x�̐�(�c��)
const int LATITUDE_COUNT_NORMAL		= 20;	// �ܓx�̐�(����)
// ���C���[�t���[��
const int LONGITUDE_COUNT_WIRE		= 15;	// �o�x�̐�(�c��)
const int LATITUDE_COUNT_WIRE		= 15;	// �ܓx�̐�(����)

const float RADIUS					= 0.5f;	// ���a

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
=========================================== */
ShapeSphere::ShapeSphere()
{
	MakeMesh();	// ���b�V���̍쐬
}

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�FfPos		���W
	����2�FfSize	�T�C�Y
	����3�FfColor	�F
	����4�FeMode	�`�惂�[�h
=========================================== */
ShapeSphere::ShapeSphere(Vector3<float> fPos, Vector3<float> fSize, Vector3<float> fColor, E_DrawMode eMode)
{
	SetPosition(fPos);
	SetScale(fSize);
	SetColor(fColor);
	SetDrawMode(eMode);

	//MakeMesh();	// ���b�V���̍쐬
}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F�������
=========================================== */
ShapeSphere::~ShapeSphere()
{
}

/* ========================================
	���b�V���쐬�֐�
	-------------------------------------
	���e�F���b�V�����쐬����
=========================================== */
void ShapeSphere::MakeMesh()
{
	MakeMeshNormal();
	MakeMeshWire();
}

/* ========================================
	���b�V���쐬(�ʏ�)�֐�
	-------------------------------------
	���e�F���b�V�����쐬����(�ʏ�)
=========================================== */
void ShapeSphere::MakeMeshNormal()
{
	// ���̒��_�f�[�^�ƃC���f�b�N�X�f�[�^��vector�ō쐬
	std::vector<T_Vertex> vertices = CreateSphereVertices(LONGITUDE_COUNT_NORMAL, LONGITUDE_COUNT_NORMAL);
	std::vector<int> indices = CreateSphereIndices(LONGITUDE_COUNT_NORMAL, LONGITUDE_COUNT_NORMAL);

	// �o�b�t�@�̍쐬
	MeshBuffer::T_MeshData desc = {};
	desc.pVtx					= vertices.data();							// ���_�f�[�^
    desc.vtxCount				= static_cast<UINT>(vertices.size());		// ���_�̐�(std::size�͔z��̐��𐔂���)
	desc.vtxSize				= sizeof(T_Vertex);							// ���_�������̃f�[�^�T�C�Y
	desc.pIdx					= indices.data();										// �C���f�b�N�X�f�[�^(���_�̏���)
	desc.idxCount				= static_cast<UINT>(indices.size());		// �C���f�b�N�X�̐�
	desc.idxSize				= sizeof(int);								// �C���f�b�N�X�������̃f�[�^�T�C�Y
	desc.topology				= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	// �g�|���W�[�̐ݒ�(�O�p�`���X�g)
	m_pMeshBuffer[DRAW_NORMAL]	= new MeshBuffer(desc);						// ���b�V���o�b�t�@�̍쐬

}

/* ========================================
	���b�V���쐬(���C���t���[��)�֐�
	-------------------------------------
	���e�F���b�V�����쐬����(���C���t���[��)
=========================================== */
void ShapeSphere::MakeMeshWire()
{
	// ���̒��_�f�[�^�ƃC���f�b�N�X�f�[�^��vector�ō쐬
	std::vector<T_Vertex> vertices	= CreateSphereVertices(LATITUDE_COUNT_WIRE, LONGITUDE_COUNT_WIRE);
	std::vector<int> indices		= CreateSphereIndices(LATITUDE_COUNT_WIRE, LONGITUDE_COUNT_WIRE);

	// �o�b�t�@�̍쐬
	MeshBuffer::T_MeshData desc = {};
	desc.pVtx					= vertices.data();						// ���_�f�[�^
	desc.vtxCount				= static_cast<UINT>(vertices.size());	// ���_�̐�(std::size�͔z��̐��𐔂���)
	desc.vtxSize				= sizeof(T_Vertex);						// ���_�������̃f�[�^�T�C�Y
	desc.pIdx					= indices.data();									// �C���f�b�N�X�f�[�^(���_�̏���)
	desc.idxCount				= static_cast<UINT>(indices.size());	// �C���f�b�N�X�̐�
	desc.idxSize				= sizeof(int);							// �C���f�b�N�X�������̃f�[�^�T�C�Y
	desc.topology				= D3D11_PRIMITIVE_TOPOLOGY_LINELIST;	// �g�|���W�[�̐ݒ�(���_�̃y�A���ƂɓƗ�������)
	m_pMeshBuffer[DRAW_WIRE_FRAME]	= new MeshBuffer(desc);					// ���b�V���o�b�t�@�̍쐬

}


/* ========================================
	���_�o�b�t�@�쐬�֐�
	-------------------------------------
	���e�F�ܓx�A�o�x�̐����狅�̒��_�f�[�^���쐬
	-------------------------------------
	����1�FlatitudeCount	�ܓx�̐�
	����2�FlongitudeCount	�o�x�̐�
	-------------------------------------
	�ߒl�Fvector<T_Vertex>	���_�f�[�^
=========================================== */
std::vector<ShapeBase::T_Vertex> ShapeSphere::CreateSphereVertices(int latitudeCount, int longitudeCount)
{
	std::vector<T_Vertex> vertices; // ���_�̔z��

	// �ܓx�����̃��[�v
	for (int lat = 0; lat <= latitudeCount; ++lat) {
		// �ܓx�Ɋ�Â��p�x���v�Z
		float theta = lat * DirectX::XM_PI / latitudeCount; // �ܓx�̊p�x
		float sinTheta = sin(theta);						// sin(��)
		float cosTheta = cos(theta);						// cos(��)

		// �o�x�����̃��[�v
		for (int lon = 0; lon <= longitudeCount; ++lon) {
			// �o�x�Ɋ�Â��p�x���v�Z
			float phi = lon * 2 * DirectX::XM_PI / longitudeCount; // �o�x�̊p�x
			float sinPhi = sin(phi); // sin(��)
			float cosPhi = cos(phi); // cos(��)

			// ���̒��_�ʒu���v�Z
			Vector3<float> position;
			position.x = RADIUS * cosPhi * sinTheta; // x���W
			position.y = RADIUS * cosTheta;			 // y���W
			position.z = RADIUS * sinPhi * sinTheta; // z���W

			// �e�N�X�`�����W���v�Z (u, v)
			Vector2<float> texCoord;
			texCoord.x = static_cast<float>(lon) / longitudeCount; // u���W
			texCoord.y = static_cast<float>(lat) / latitudeCount; // v���W

			// �@�����v�Z (���_�ʒu�𐳋K��)
			Vector3<float> normal = position.GetNormalize();  // ���_�ʒu�𐳋K�����Ė@�����v�Z

			// ���_��z��ɒǉ�
			vertices.push_back({ position, normal, texCoord });
		}
	}


	return vertices;
}


/* ========================================
	�C���f�b�N�X�o�b�t�@�쐬�֐�
	-------------------------------------
	���e�F�ܓx�A�o�x�̐����狅�̃C���f�b�N�X�f�[�^���쐬
	-------------------------------------
	����1�FlatitudeCount	�ܓx�̐�
	����2�FlongitudeCount	�o�x�̐�
	-------------------------------------
	�ߒl�Fvector<int>	�C���f�b�N�X�f�[�^
=========================================== */
std::vector<int> ShapeSphere::CreateSphereIndices(int latitudeCount, int longitudeCount)
{
	std::vector<int> indices; // �C���f�b�N�X�̔z��

	// �ܓx�����̃��[�v
	for (int lat = 0; lat < latitudeCount; ++lat) 
	{
		// �o�x�����̃��[�v
		for (int lon = 0; lon < longitudeCount; ++lon) 
		{

			// ���݂̒��_�̃C���f�b�N�X���v�Z
			int first = (lat * (longitudeCount + 1)) + lon; // ���݂̈ܓx�̍ŏ��̒��_
			int second = first + longitudeCount + 1; // ���̈ܓx�̍ŏ��̒��_

			// �O�p�`���`�����邽�߂̃C���f�b�N�X��ǉ��i���Ԃ��t�Ɂj
			indices.push_back(first);		// ���݂̈ܓx�̌��݂̌o�x�̒��_
			indices.push_back(first + 1);	// ���݂̈ܓx�̎��̌o�x�̒��_
			indices.push_back(second);		// ���̈ܓx�̌��݂̌o�x�̒��_

			indices.push_back(second);		// ���̈ܓx�̌��݂̌o�x�̒��_
			indices.push_back(first + 1);	// ���݂̈ܓx�̎��̌o�x�̒��_
			indices.push_back(second + 1);	// ���̈ܓx�̎��̌o�x�̒��_
		}
	}

	return indices;
}
