/* ========================================
	DX22Base/
	------------------------------------
	�O���b�h���`��p�w�b�_
	------------------------------------
	�����F�f�o�b�O�p��XZ�����̃O���b�h��
		�@XYZ������`�悷��
	------------------------------------
	GridLine.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ShapeLine.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <memory>

// =============== �N���X�̒�` ===================
class GridLine
{
public:
	static void Init();
	static void Draw();

private:
	static void MakeGrid();
	static void MakeGridAxis();

private:
	static inline std::unique_ptr<ShapeLine> m_pGridLine;	// �O���b�h��
	static inline std::unique_ptr<ShapeLine> m_pAxisLine;	// XYZ����
	
	static inline bool				m_bIsEnable;
	static inline float				m_fGridSize;
	static inline float				m_fGridMargin;
	static inline Vector3<float>	m_fGridColor;
	static inline bool				m_bIsAxis;
	
};

