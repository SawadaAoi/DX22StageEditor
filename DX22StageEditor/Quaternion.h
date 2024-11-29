/* ========================================
	DX22Base/
	------------------------------------
	�N�H�[�^�j�I���p�w�b�_
	------------------------------------
	�����F3D��Ԃɂ������]��\�����邽�߂̃N���X�ł��B
		�@��]���Ɖ�]�p�x����ɃN�H�[�^�j�I���𐶐����A
		�@��]���s�����߂̃��\�b�h��񋟂��܂�
	------------------------------------
	Quaternion.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "Vector3.h"		// Vector3<float>
#include <DirectXMath.h>	// DirectX::XMFLOAT4, DirectX::XMMATRIX



// =============== �N���X��` =====================
class Quaternion
{
public:
	// �R���X�g���N�^
	Quaternion();
	Quaternion(Vector3<float> fAxis, float fAngle);	// �����Ǝ������珉����

	// ���Ɗp�x����N�H�[�^�j�I�����쐬
	static Quaternion FromAxisAngle(Vector3<float> fAxis, float fAngle);			// �񐳋K��
	static Quaternion FromAxisAngleNormalized(Vector3<float> fAxis, float fAngle);	// ���K��
	void SetFromAxisAngle(Vector3<float> fAxis, float fAngle);				// ���g��ύX(�񐳋K��)
	void SetFromAxisAngleNormalized(Vector3<float> fAxis, float fAngle);	// ���g��ύX(���K��)

	// ���K���֘A���\�b�h
	void Normalize();			// ���K��
	Quaternion GetNormalized();	// ���K��(�R�s�[��Ԃ�)

	// ��]
	void Rotate(Vector3<float> fAxis, float fAngle);	// ���Ɗp�x�����]
	Vector3<float> Rotate(Vector3<float> fVec);		// �x�N�g������]

	// �N�H�[�^�j�I���̕⊮
	static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);	// ���ʐ��`���
	static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);	// ���`���
	void Slerp(const Quaternion& target, float t);	// ���ʐ��`���(���g��ύX
	void Lerp(const Quaternion& target, float t);	// ���`���(���g��ύX)

	// �N�H�[�^�j�I���֘A���\�b�h
	Quaternion Conjugate() const;	// ����
	Quaternion Inverse() const;		// �t�N�H�[�^�j�I��

	Vector3<float> GetForwardVector() const;	// �O���x�N�g���擾
	Vector3<float> GetRightVector() const;		// �E���x�N�g���擾
	Vector3<float> GetUpVector() const;			// ����x�N�g���擾

	// �ϊ��֐�
	DirectX::XMMATRIX ToDirectXMatrix() const;			// DirectX�̍s��ɕϊ�
	DirectX::XMFLOAT4 ToDirectXFloat4() const;			// DirectX��XMFLOAT4�ɕϊ�
	DirectX::XMVECTOR ToXMVECTOR() const;				// DirectX��XMVECTOR�ɕϊ�
	Vector3<float>	  ToEulerAngle() const;				// �I�C���[�p�ɕϊ�
	static Quaternion FromDirectXMatrix(DirectX::XMMATRIX matrix);	// �s�񂩂�N�H�[�^�j�I���ɕϊ�
	static Quaternion FromEulerAngle(Vector3<float> fEulerAngle);	// �I�C���[�p����N�H�[�^�j�I���ɕϊ�
	static Quaternion FromXMVECTOR(DirectX::XMVECTOR vec);			// XMVECTOR����N�H�[�^�j�I���ɕϊ�

	// ���Z�q�I�[�o�[���[�h
	Quaternion operator+(const Quaternion& other) const;	// ���Z
	Quaternion operator-(const Quaternion& other) const;	// ���Z
	Quaternion operator*(const Quaternion& other) const;	// ��Z
	Quaternion operator*(float scalar) const;				// �X�J���[��Z
	Quaternion& operator*=(const Quaternion& other) ;		// �����Z
	bool operator==(const Quaternion& other) const;			// ����
	bool operator!=(const Quaternion& other) const;			// �񓙉�


	// �w�肵���������̊p�x���擾
	Vector3<float> GetAngleWorldXYZ() const;	// ���[���h���W�n�ł̊p�x�擾



public:
	Vector3<float> m_fAxis;	// ��]��(������)	�������̃x�N�g�����i�[����Ă���킯�ł͂Ȃ�
	float m_fAngle;			// ��]�p�x(������)	�������̉�]�p�x���i�[����Ă���킯�ł͂Ȃ�
};

