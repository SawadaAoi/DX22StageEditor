/* ========================================
	DX22Base/
	------------------------------------
	�}�`�`��p�w�b�_
	------------------------------------
	�����F���A�����́A�~���N���X�̊��N���X
	------------------------------------
	Shape.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "MeshBuffer.h"		// ���b�V���o�b�t�@
#include "VertexShader.h"	// ���_�V�F�[�_
#include "PixelShader.h"	// �s�N�Z���V�F�[�_
#include <DirectXMath.h>	// DirectXMath
#include "Vector3.h"
#include "Vector2.h"
#include "Quaternion.h"
#include "ObjectLight.h"

// =============== �N���X�̒�` ===================
class ShapeBase
{
public:
	// ���_�\����
	struct T_Vertex
	{
		Vector3<float> fPos;	// ���W
		Vector3<float> fNormal;	// �@��
		Vector2<float> fUv;		// �e�N�X�`�����W
	};

	// �\�����[�h
	enum E_DrawMode
	{
		DRAW_NORMAL,			// �ʏ�
		DRAW_WIRE_FRAME,		// ���C���[�t���[��
		DRAW_MAX
	};

public:
	ShapeBase();
	~ShapeBase();
	virtual void Draw();
	virtual void MakeMesh() = 0;	// �}�`�쐬
 

	// �Q�b�^�[
	Vector3<float>	GetPosition();
	Vector3<float>	GetScale();
	Quaternion		GetRotation();

	Vector3<float>	GetColor();
	E_DrawMode		GetDrawMode();

	bool GetIsTexture();
	bool GetIsCulling();
	Vector2<float> GetUvScale(int nIndex = 0);
	Vector2<float> GetUvOffset(int nIndex = 0);
	

	// �Z�b�^�[
	void SetPosition(Vector3<float> fPos);
	void SetScale(Vector3<float> fSize);
	void SetRotation(Quaternion qRot);

	void SetColor(Vector3<float> fColor);
	void SetDrawMode(E_DrawMode eMode);

	void SetTexture(Texture* pTexture, int nIndex = 0);
	void SetUseTexture(int bIsTex);

	void SetLightMaterial(float fDiffuse, float fSpecular, float fAmbient, bool bUseLight);		// ���C�g�p�����[�^�ݒ�(�s�N�Z���V�F�[�_�[�p)
	void SetCameraPos(Vector3<float> fCameraPos);												// �J�����ʒu�ݒ�(�s�N�Z���V�F�[�_�[�p)
	void SetLights(std::vector<ObjectLight*> lights);											// ���C�g�ݒ�(�s�N�Z���V�F�[�_�[�p)

	void SetIsCulling(bool bCulling);	// �J�����O�ݒ�

	void SetUvScale(Vector2<float> fScale, int nIndex = 0);
	void SetUvOffset(Vector2<float> fOffset, int nIndex = 0);

protected:
	void SetWVPMatrix();

protected:
	MeshBuffer*		m_pMeshBuffer[DRAW_MAX];	// ���b�V���o�b�t�@(���_�A�C���f�b�N�X)
	E_DrawMode		m_eDrawMode;				// �\�����[�h

	VertexShader*	m_pVS;			// ���_�V�F�[�_
	PixelShader*	m_pPS;			// �s�N�Z���V�F�[�_

	Vector3<float>	m_vPosition;	// ���W
	Quaternion		m_qRotation;	// ��](�N�H�[�^�j�I��)
	Vector3<float>	m_vScale;		// �T�C�Y


	std::vector<Texture*>	m_pTextures;	// �e�N�X�`���z��
	int						m_bIsTex;		// �e�N�X�`���g�p�t���O
	bool					m_bIsCulling;	// �J�����O�t���O(�J�����O���邩�ǂ���)

	std::vector<Vector2<float>>	m_fUvScale;		// �e�N�X�`�����W�X�P�[��
	std::vector<Vector2<float>>	m_fUvOffset;	// �e�N�X�`�����W�I�t�Z�b�g
		
	Vector3<float>			 m_fColor;		// �F	

	DirectX::XMFLOAT4X4		m_WVP[3];		// ���[���h�A�r���[�A�v���W�F�N�V�����s��

};


