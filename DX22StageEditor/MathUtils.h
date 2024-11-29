/* ========================================
	DX22Base/
	------------------------------------
	���w���[�e�B���e�B�p�w�b�_
	------------------------------------
	�����F���w�֘A�֗̕��Ȋ֐����`
	------------------------------------
	MathUtils.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include <cstdlib>	// rand�֐��p

// =============== ���O��� =====================
namespace MathUtils
{
	// =============== �萔 =====================
	const float PI = 3.14159265358979323846f;	// �~����
	const float EPSILON = 1.0e-5f;	// ���e�덷

	// =============== �֐� =====================
	// �p�x�����W�A���ɕϊ�
	inline float ToRadian(float degree)
	{
		return degree * PI / 180.0f;
	}

	// ���W�A�����p�x�ɕϊ�
	inline float ToDegree(float radian)
	{
		return radian * 180.0f / PI;
	}


	// �����_���Ȓl���擾(������)
	inline float Random(float min, float max)
	{
		return min + static_cast<float>(rand()) / (RAND_MAX + 1) * (max - min);
	}

	// �����_���Ȓl���擾(������)
	inline int Random(int min, int max)
	{
		return min + rand() % (max - min + 1);
	}
}