/* ========================================
	DX22Base/
	------------------------------------
	�s�N�Z���V�F�[�_�[�pcpp
	------------------------------------
	PixelShader.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "PixelShader.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
=========================================== */
PixelShader::PixelShader()
	: Shader(Shader::Pixel)	// �V�F�[�_�[�̎�ނ��s�N�Z���V�F�[�_�ɐݒ�
	, m_pPS(nullptr)
{
}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F�������
=========================================== */
PixelShader::~PixelShader()
{
	SAFE_RELEASE(m_pPS);
}

/* ========================================
	�o�C���h�֐�
	-------------------------------------
	���e�F�s�N�Z���V�F�[�_�[�� GPU �Ƀo�C���h���A
		�@�`�揈���Ŏg�p�ł����Ԃɂ���
=========================================== */
void PixelShader::Bind(void)
{
	// �f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext* pContext = DirectXManager::GetContext();
	// �s�N�Z���V�F�[�_�[���o�C���h
	pContext->PSSetShader(m_pPS, nullptr, 0);

	// �萔�o�b�t�@�̐������o�C���h
	for (int i = 0; i < m_pBuffers.size(); ++i)
	{
		pContext->PSSetConstantBuffers(i, 1, &m_pBuffers[i]);
	}
	// �e�N�X�`���̐������o�C���h
	for (int i = 0; i < m_pTextures.size(); ++i)
	{
		pContext->PSSetShaderResources(i, 1, &m_pTextures[i]);
	}
}

/* ========================================
	�V�F�[�_�[�쐬�֐�
	-------------------------------------
	���e�F�V�F�[�_�[�̃o�C�g�R�[�h����s�N�Z���V�F�[�_�[���쐬�B
	-------------------------------------
	����1�FpData	�f�[�^
	����2�Fsize		�T�C�Y
=========================================== */
HRESULT PixelShader::MakeShader(void* pData, UINT size)
{
	return DirectXManager::GetDevice()->CreatePixelShader(pData, size, NULL, &m_pPS);
}