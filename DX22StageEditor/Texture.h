/* ========================================
	DX22Base/
	------------------------------------
	�e�N�X�`���p�w�b�_
	------------------------------------
	�����F�e�N�X�`���������N���X
	------------------------------------
	Texture.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "DirectXManager.h"	// DirectXManager
#include "DirectXTex/TextureLoad.h"


// =============== �N���X��` =====================
class Texture
{
public:
	Texture();
	virtual ~Texture();
	// �R�s�[�R���X�g���N�^
	Texture(const Texture& other);


	HRESULT Create(const char* fileName);
	HRESULT Create(DXGI_FORMAT format, UINT width, UINT height, const void* pData = nullptr);


	// �Q�b�^�[
	UINT GetWidth() const;
	UINT GetHeight() const;
	ID3D11ShaderResourceView* GetResource() const;

protected:
	D3D11_TEXTURE2D_DESC MakeTexDesc(DXGI_FORMAT format, UINT width, UINT height);
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData);	// �p����ŏ�����ύX���邽�߁A���z�֐��ɂ���

protected:
	UINT						m_width;		// ����
	UINT						m_height;		// �c��
	ID3D11Texture2D*			m_pTex;			// �e�N�X�`��(�����_�[�^�[�Q�b�g�A�[�x�X�e���V���Ŏg�p)

private:
	ID3D11ShaderResourceView* m_pSRV;			// �V�F�[�_���\�[�X�r���[(�V�F�[�_�[�ɓn�����߂̃r���[)

};

