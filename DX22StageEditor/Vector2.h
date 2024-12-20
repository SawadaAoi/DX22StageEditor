/* ========================================
    DX22Base/
    ------------------------------------
    2�����x�N�g���p�w�b�_
    ------------------------------------
	�����F2�����x�N�g���̃e���v���[�g�N���X
    ------------------------------------
    Vector2.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include <cmath>
#include <utility>
#include <DirectXMath.h>    // DirectX::XMFLOAT2

// =============== �N���X��` =====================
template <typename T>
class Vector2
{
public:
    // �R���X�g���N�^
	Vector2();                // �f�t�H���g�R���X�g���N�^
    Vector2(T x, T y);    // 2�����x�N�g���̏�����

    // ���Z�q�I�[�o�[���[�h =====================
    Vector2<T> operator+(const Vector2<T>& other) const;    // ���Z
    Vector2<T> operator-(const Vector2<T>& other) const;    // ���Z
    Vector2<T> operator*(const Vector2<T>& other) const;    // ��Z
    Vector2<T> operator*(T scalar) const;                   // �X�J���[��Z
    Vector2<T> operator/(const Vector2<T>& other) const;    // ���Z
    Vector2<T> operator/(T scalar) const;                   // �X�J���[���Z

    Vector2<T>& operator+=(const Vector2<T>& other);    // ���Z���
    Vector2<T>& operator-=(const Vector2<T>& other);    // ���Z���
    Vector2<T>& operator*=(const Vector2<T>& other);    // ��Z���
    Vector2<T>& operator*=(T scalar);                  // �X�J���[��Z���
    Vector2<T>& operator/=(const Vector2<T>& other);    // ���Z���
    Vector2<T>& operator/=(T scalar);                  // �X�J���[���Z���

    T operator[](int index) const;    // �Y�������Z�q

	// �P�����Z�q ==============================
	Vector2<T> operator+() const;    // �P���v���X
	Vector2<T> operator-() const;    // �P���}�C�i�X

    // ��r���Z�q ==============================
    bool operator==(const Vector2<T>& other) const;    // ������r
    bool operator!=(const Vector2<T>& other) const;    // �񓙉���r
	bool operator<(const Vector2<T>& other) const;    // �x�N�g���̒����Ŕ�r
	bool operator>(const Vector2<T>& other) const;    // �x�N�g���̒����Ŕ�r
	bool operator<=(const Vector2<T>& other) const;    // �x�N�g���̒����Ŕ�r
	bool operator>=(const Vector2<T>& other) const;    // �x�N�g���̒����Ŕ�r

    // �����o�֐� ==============================
    double  Length() const;        // �x�N�g���̒���
    T       LengthSq() const;           // �x�N�g���̒�����2��
    Vector2<T> Normalize() const;    // ���K��

    T Dot(const Vector2<T>& other) const;    // ����
	T Cross(const Vector2<T>& other) const;    // �O��

    T Distance(const Vector2<T>& other) const;    // 2�_�Ԃ̋���
	T DistanceSq(const Vector2<T>& other) const;    // 2�_�Ԃ̋�����2��

	double AngleRad(const Vector2<T>& other) const;    // ���W�A���p�x
	double AngleDeg(const Vector2<T>& other) const;    // �x�p�x
	double DirectionRad(const Vector2<T>& other) const;    // ����(���W�A��
	double DirectionDeg(const Vector2<T>& other) const;    // ����(�x)

	Vector2<T> Lerp(const Vector2<T>& other, float t) const;    // ���`���

	// DirectX::XMFLOAT2 �֕ϊ�
	DirectX::XMFLOAT2 ToXMFLOAT2() const;                       // Vector2 -> DirectX::XMFLOAT2
	Vector2<T> ToVector2(const DirectX::XMFLOAT2& other) const; // DirectX::XMFLOAT2 -> Vector2 


    // �ÓI�����o�֐� ==========================
    static Vector2<T> Zero();    // �[���x�N�g��
    static Vector2<T> One();     // 1�x�N�g��
	static Vector2<T> Up();      // ������x�N�g��
	static Vector2<T> Down();    // �������x�N�g��
	static Vector2<T> Right();   // �E�����x�N�g��
	static Vector2<T> Left();    // �������x�N�g��

public:
    T x;    // X���W
    T y;    // Y���W
};

#include "Vector2.inl"    // �C�����C���t�@�C��
