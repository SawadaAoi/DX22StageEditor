/* ========================================
	DX22Base/
	------------------------------------
	�s�N�Z���V�F�[�_�[�p�w�b�_
	------------------------------------
	�����F�s�N�Z���V�F�[�_�[�̃N���X
	------------------------------------
	PixelShader.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "Shader.h"


// =============== �N���X��` =====================
class PixelShader : public Shader
{
public:
	PixelShader();
	~PixelShader();
	void Bind(void);
protected:
	HRESULT MakeShader(void* pData, UINT size);

private:
	ID3D11PixelShader* m_pPS;	// �s�N�Z���V�F�[�_
};
