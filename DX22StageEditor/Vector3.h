/* ========================================
	DX22Base/
	------------------------------------
	3�����x�N�g���p�w�b�_
	------------------------------------
	�����F3�����x�N�g���̃e���v���[�g�N���X
	------------------------------------
	Vector3.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include <cmath>
#include <utility>
#include <DirectXMath.h>	// DirectX::XMFLOAT3

// =============== �N���X��` =====================
template <typename T>
class Vector3
{
public:
	Vector3();	// �R���X�g���N�^
	Vector3(T x, T y, T z);	// �R���X�g���N�^

	// ���Z�q�I�[�o�[���[�h =====================
	Vector3<T> operator+(const Vector3<T>& other) const;		// ���Z
	Vector3<T> operator-(const Vector3<T>& other) const;		// ���Z
	Vector3<T> operator*(const Vector3<T>& other) const;		// ��Z
	Vector3<T> operator*(T scalar) const;						// ��Z(�X�J���[)
	Vector3<T> operator/(const Vector3<T>& other) const;		// ���Z
	Vector3<T> operator/(T scalar) const;						// ���Z(�X�J���[)

	Vector3<T>& operator+=(const Vector3<T>& other);		// ���Z���
	Vector3<T>& operator-=(const Vector3<T>& other);		// ���Z���
	Vector3<T>& operator*=(const Vector3<T>& other);		// ��Z���
	Vector3<T>& operator*=(T scalar);						// ��Z���(�X�J���[)
	Vector3<T>& operator/=(const Vector3<T>& other);		// ���Z���
	Vector3<T>& operator/=(T scalar);						// ���Z���(�X�J���[)

	T operator[](int index) const;	// �Y�������Z�q

	// ���Z�q�I�[�o�[���[�h(�P�����Z�q) =====================
	Vector3<T> operator+() const;	// �P���v���X
	Vector3<T> operator-() const;	// �P���}�C�i�X

	// ��r���Z�q ==============================
	bool operator==(const Vector3<T>& other) const;
	bool operator!=(const Vector3<T>& other) const;
	bool operator<(const Vector3<T>& other) const;	// �x�N�g���̒����Ŕ�r
	bool operator>(const Vector3<T>& other) const;	// �x�N�g���̒����Ŕ�r
	bool operator<=(const Vector3<T>& other) const;	// �x�N�g���̒����Ŕ�r
	bool operator>=(const Vector3<T>& other) const;	// �x�N�g���̒����Ŕ�r

	// �����o�֐� ==============================

	double	Length() const;					// �x�N�g���̒���
	T		LengthSq() const;				// �x�N�g���̒�����2��
	Vector3<T> GetNormalize() const;	// ���K��
	void Normalize();


	T Dot(const Vector3<T>& other) const;				// ����
	Vector3<T> Cross(const Vector3<T>& other) const;	// �O��

	T Distance(const Vector3<T>& other) const;		// 2�_�Ԃ̋���
	T DistanceSq(const Vector3<T>& other) const;	// 2�_�Ԃ̋�����2��

	double AngleRad(const Vector3<T>& other) const;		// 2�̃x�N�g�����Ȃ��p�x(���W�A��)
	double AngleDeg(const Vector3<T>& other) const;		// 2�̃x�N�g�����Ȃ��p�x(�x)
	double DirectionRadXZ(const Vector3<T>& other) const;	// �����x�N�g���ւ̕���(���W�A��)
	double DirectionDegXZ(const Vector3<T>& other) const;	// �����x�N�g���ւ̕���(�x)


	// ���`���
	static Vector3<T> Lerp(const Vector3<T>& start, const Vector3<T>& end, double t);	// �ÓI�ɌĂяo����
	Vector3<T> Lerp(const Vector3<T>& other, double t) const;							// �����o�֐���ύX����

	// DirectX::XMFLOAT3�ϊ�
	DirectX::XMFLOAT3 ToXMFLOAT3() const;					// Vector3 -> DirectX::XMFLOAT3
	Vector3<T>& operator=(const DirectX::XMFLOAT3& other);	// DirectX::XMFLOAT3 -> Vector3

	// DirectX::XMMATRIX�ϊ�
	static Vector3<float> FromMatrix_Translation(const DirectX::XMMATRIX matrix);	// �s�񂩂�ړ��������擾
	static Vector3<float> FromMatrix_Scale(const DirectX::XMMATRIX matrix);			// �s�񂩂�g�听�����擾
	static Vector3<float> FromMatrix_RotationEuler(const DirectX::XMMATRIX matrix);	// �s�񂩂��]�������擾

	// DirectX::XMVECTOR�ϊ�
	DirectX::XMVECTOR ToXMVECTOR() const;									// Vector3 -> DirectX::XMVECTOR
	static Vector3<float> FromXMVECTOR(const DirectX::XMVECTOR& vector);	// DirectX::XMVECTOR -> Vector3

	T GetMax() const;	// �ő�l
	T GetMin() const;	// �ŏ��l

	// �ÓI�����o�֐� ==========================
	static Vector3<T> Zero();		// �[���x�N�g��
	static Vector3<T> One();		// 1�x�N�g��
	static Vector3<T> Up();			// ������x�N�g��
	static Vector3<T> Down();		// �������x�N�g��
	static Vector3<T> Right();		// �E�����x�N�g��
	static Vector3<T> Left();		// �������x�N�g��
	static Vector3<T> Forward();	// �O�����x�N�g��
	static Vector3<T> Back();		// ������x�N�g��

public:
	T x;
	T y;
	T z;
};

#include "Vector3.inl"	// �C�����C���t�@�C��

