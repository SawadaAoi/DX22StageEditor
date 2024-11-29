/* ========================================
	DX22Base/
	------------------------------------
	�}�`�`��(�|���S��)�p�w�b�_
	------------------------------------
	�����F�|���S����`�悷��N���X
	------------------------------------
	ShapePlane.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ShapeBase.h"

// =============== �N���X��` ===================
class ShapePlane :
    public ShapeBase
{
public:
	ShapePlane();
	~ShapePlane() {};

	void MakeMesh() override;
	
private:
	void MakeMeshNormal();
	void MakeMeshWire();
};

