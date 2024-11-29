/* ========================================
	DX22Base/
	------------------------------------
	���_�V�F�[�_�[�p�w�b�_
	------------------------------------
	�����F���_�V�F�[�_�[�̃N���X
	------------------------------------
	VertexShader.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "Shader.h"

// =============== �N���X��` =====================
class VertexShader : public Shader
{
public:
	VertexShader();
	~VertexShader();
	void Bind(void);	// �V�F�[�_�[��`��Ɏg�p
protected:
	HRESULT MakeShader(void* pData, UINT size);

private:
	ID3D11VertexShader*		m_pVS;			// ���_�V�F�[�_
	ID3D11InputLayout*		m_pInputLayout;	// �C���v�b�g���C�A�E�g
};
