/* ========================================
	DX22Base/
	------------------------------------
	���`��p�w�b�_
	------------------------------------
	�����F��̓_�����Ԑ���`�悷��
		�@ShapeBase���p�����Ă��Ȃ����A
		  �����@�\�����ׂ�Shape�𖼑O�Ɋ܂߂Ă���
	------------------------------------
	ShapeLine.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "Vector3.h"
#include "MeshBuffer.h"		// ���b�V���o�b�t�@
#include "VertexShader.h"	// ���_�V�F�[�_
#include "PixelShader.h"	// �s�N�Z���V�F�[�_
#include "Quaternion.h"


// =============== �N���X�̒�` ===================
class ShapeLine
{
public:
	// ���̒��_�\����
	struct T_LineVertex
	{
		float pos[3];	// ���W
		float color[4];	// ���̐F
	};
public:
	ShapeLine();
	ShapeLine(int DrawNum);
	ShapeLine(Vector3<float> start, Vector3<float> end,
		Vector3<float> color = Vector3<float>(1.0f, 1.0f, 1.0f));
	~ShapeLine();
	ShapeLine(const ShapeLine& other);
	void Draw();

	void AddLine(Vector3<float> start, Vector3<float> end,
		Vector3<float> color = Vector3<float>(1.0f, 1.0f, 1.0f));

	void UpdateLine(int nLineNum, Vector3<float> start, Vector3<float> end,
		Vector3<float> color = Vector3<float>(1.0f, 1.0f, 1.0f));

	// �Z�b�^�[
	void SetPos(Vector3<float> pos);
	void SetRotation(Quaternion rot);
	void SetScale(Vector3<float> scale);

private:
	void MakeLine();
	void SetWVP();

	MeshBuffer*		m_pMeshBuffer;	// ���b�V���o�b�t�@
	void*			m_pVertex;		// ���_�f�[�^
	PixelShader*	m_pPS;			// �s�N�Z���V�F�[�_
	VertexShader*	m_pVS;			// ���_�V�F�[�_

	int				m_nLineCnt;		// ���̐�
	int				m_nMaxLineCnt;	// �ő����

	Vector3<float>	m_vPos;			// ���W
	Quaternion		m_qRotation;		// ��]
	Vector3<float>	m_vScale;		// �X�P�[��

	DirectX::XMFLOAT4X4 m_WVP[3];	// ���[���h�A�r���[�A�v���W�F�N�V�����s��


};

