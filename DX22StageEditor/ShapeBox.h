/* ========================================
	DX22Base/
	------------------------------------
	�}�`�`��(������)�p�w�b�_
	------------------------------------
	�����F�����̂�`�悷��N���X
	------------------------------------
	ShapeBox.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ShapeBase.h"

// =============== �N���X�̒�` ===================
class ShapeBox :
    public ShapeBase
{
public:
	ShapeBox();
	ShapeBox(Vector3<float> fPos, Vector3<float> fSize, Vector3<float> fColor, E_DrawMode eMode);
	~ShapeBox();
	void MakeMesh() override;

private:
	void MakeMeshNormal();
	void MakeMeshWire();
};

