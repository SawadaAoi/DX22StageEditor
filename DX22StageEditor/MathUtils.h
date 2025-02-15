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
#include <random>	// ���������p

// =============== ���O��� =====================
namespace MathUtils
{
	// =============== �萔 =====================
	constexpr float PI = 3.14159265358979323846f;	// �~����
	constexpr float EPSILON = 1.0e-5f;					// ���e�덷

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

	// �ėp�����_���l�����i�e���v���[�g�Łj
	template<typename T>
	inline T Random(T min, T max)
	{
		static thread_local std::mt19937 gen(std::random_device{}());
		if constexpr (std::is_integral<T>::value) {
			std::uniform_int_distribution<T> dist(min, max);
			return dist(gen);
		}
		else {
			std::uniform_real_distribution<T> dist(min, max);
			return dist(gen);
		}
	}

	// -�΁`�΂͈̔͂���0�`2�΂͈̔͂ɕϊ�
	inline double ConvertTo0To2Pi(double value)
	{
		// ���̒l�̏ꍇ�A2�΂������Đ��͈̔͂Ɉڂ�
		while (value < 0)
			value += 2 * PI;

		// 2�΂𒴂���l�̏ꍇ�A2�΂Ŋ������]������
		return fmod(value, 2 * PI);
	}

	// DirectX�̕���(���v���0���X�^�[�g)���O�p�֐��̕���(�����v���3���X�^�[�g)�ɕϊ�
	inline float ConvertDirectionDXToTrigonometric(float dxDirection)
	{
		dxDirection -= MathUtils::PI / 2.0f;	// 90�x��]
		dxDirection *= -1.0f;				// �t��]

		return dxDirection;
	}
}