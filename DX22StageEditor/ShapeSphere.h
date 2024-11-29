/* ========================================
	DX22Base/
	------------------------------------
	�}�`�`��(��)�p�w�b�_
	------------------------------------
	�����F����`�悷��N���X
	------------------------------------
	ShapeSphere.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ShapeBase.h"
#include <vector>

class ShapeSphere :
    public ShapeBase
{
public:
	ShapeSphere();
	ShapeSphere(Vector3<float> fPos, Vector3<float> fSize, Vector3<float> fColor, E_DrawMode eMode);
	~ShapeSphere();
	void MakeMesh() override;
private:
	void MakeMeshNormal();
	void MakeMeshWire();

	// ���_�f�[�^�쐬
	std::vector<ShapeBase::T_Vertex> CreateSphereVertices(int latitudeCount, int longitudeCount);
	// �C���f�b�N�X�f�[�^�쐬
	std::vector<int> CreateSphereIndices(int latitudeCount, int longitudeCount);
};

