/* ========================================
	DX22Base/
	------------------------------------
	�e�N�X�`���pcpp
	------------------------------------
	Texture.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "Texture.h"
#include "DirectXManager.h"	// DirectXManager



/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
=========================================== */
Texture::Texture()
	: m_width(0), m_height(0)
	, m_pTex(nullptr)
	, m_pSRV(nullptr)
{
}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F���
=========================================== */
Texture::~Texture()
{
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pTex);
}

/* ========================================
	�R�s�[�R���X�g���N�^
	-------------------------------------
	���e�F�R�s�[�R���X�g���N�^
		�@���A�j�����f���̃f�B�[�v�R�s�[���p
=========================================== */
Texture::Texture(const Texture& other)
	: m_width(0)
	, m_height(0)
	, m_pTex(nullptr)
	, m_pSRV(nullptr)
{
	if (this == &other) return;

	m_width = other.m_width;
	m_height = other.m_height;

	// �e�N�X�`�������݂���ꍇ
	if (other.m_pTex)
	{
		// �e�N�X�`���̐ݒ���擾
		D3D11_TEXTURE2D_DESC desc;
		other.m_pTex->GetDesc(&desc);

		// �e�N�X�`���쐬
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = nullptr;
		data.SysMemPitch = desc.Width * 4;

		// �e�N�X�`������
		DirectXManager::GetDevice()->CreateTexture2D(&desc, nullptr, &m_pTex);

	}
	// �V�F�[�_���\�[�X�r���[�����݂���ꍇ
	if (other.m_pSRV) {
		ID3D11Resource* resource = nullptr;

		// ���� ShaderResourceView ���烊�\�[�X���擾
		other.m_pSRV->GetResource(&resource);

		// ���̃r���[�̐ݒ���擾
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		other.m_pSRV->GetDesc(&srvDesc);

		// �V���� ShaderResourceView ���쐬
		ID3D11Device* device = DirectXManager::GetDevice();
		if (device) {
			device->CreateShaderResourceView(resource, &srvDesc, &m_pSRV);
		}

		// ���\�[�X�̉��
		if (resource) {
			resource->Release();
		}
	}
}


/* ========================================
	�e�N�X�`���쐬�֐�
	-------------------------------------
	���e�F�摜�t�@�C������e�N�X�`�����쐬
	-------------------------------------
	�����FfileName		�t�@�C���p�X
	-------------------------------------
	�߂�l�FHRESULT		����
=========================================== */
HRESULT Texture::Create(const char* fileName)
{
	HRESULT hr = S_OK;

	// �����ϊ�
	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;
	MultiByteToWideChar(0, 0, fileName, -1, wPath, MAX_PATH);	// ���C�h�����ɕϊ�

	// �t�@�C���ʓǂݍ���
	DirectX::TexMetadata	mdata;	// ���^�f�[�^(�摜���)
	DirectX::ScratchImage	image;	// �摜�f�[�^

	// TGA�t�@�C��
	if (strstr(fileName, ".tga"))	
	{
		hr = DirectX::LoadFromTGAFile(wPath, &mdata, image);
	}
	// ����ȊO
	else
	{
		hr = DirectX::LoadFromWICFile(wPath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &mdata, image);

	}
	// �ǂݍ��ݎ��s
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// �V�F�[�_���\�[�X�r���[����
	hr = CreateShaderResourceView(
		DirectXManager::GetDevice(),	// �f�o�C�X
		image.GetImages(),				// �摜�f�[�^
		image.GetImageCount(),			// �摜��
		mdata,							// ���^�f�[�^
		&m_pSRV);						// �V�F�[�_���\�[�X�r���[

	// �ǂݍ��ݎ��s
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// �����A�c���擾
	m_width		= (UINT)mdata.width;
	m_height	= (UINT)mdata.height;


	return hr;
}


/* ========================================
	�e�N�X�`���쐬�֐�
	-------------------------------------
	���e�F�t�H�[�}�b�g���w�肵�ăe�N�X�`�����쐬
	-------------------------------------
	����1�Fformat		�t�H�[�}�b�g
	����2�Fwidth		����
	����3�Fheight		�c��
	����4�FpData		�����f�[�^
	-------------------------------------
	�߂�l�FHRESULT		����
=========================================== */
HRESULT Texture::Create(DXGI_FORMAT format, UINT width, UINT height, const void* pData)
{
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);	// �e�N�X�`���ݒ�
	return CreateResource(desc, pData);
}


/* ========================================
	�e�N�X�`���\���̍쐬�֐�
	-------------------------------------
	���e�F�e�N�X�`���\���̂��쐬
	-------------------------------------
	����1�Fformat		�t�H�[�}�b�g
	����2�Fwidth		����
	����3�Fheight		�c��
	-------------------------------------
	�߂�l�FHRESULT		����
=========================================== */
D3D11_TEXTURE2D_DESC Texture::MakeTexDesc(DXGI_FORMAT format, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); 

	desc.Width				= width;						// �e�N�X�`���̕��i�s�N�Z���P�ʁj
	desc.Height				= height;						// �e�N�X�`���̍����i�s�N�Z���P�ʁj
	desc.MipLevels			= 1;							// �~�b�v�}�b�v�̃��x�����i1�ɐݒ肷��ƃ~�b�v�}�b�v�Ȃ��j
	desc.ArraySize			= 1;							// �e�N�X�`���z��̃T�C�Y�i1�Œʏ�̃e�N�X�`���j
	desc.Format				= format;						// �e�N�X�`���̃s�N�Z���t�H�[�}�b�g�i��: RGBA�Ȃǁj
	desc.SampleDesc.Count	= 1;							// �}���`�T���v�����O�̐��i1�ɐݒ肷��ƃT���v�����O�Ȃ��j
	desc.SampleDesc.Quality = 0;							// �}���`�T���v�����O�̕i���i0�ɐݒ肷��ƃT���v�����O�Ȃ��j
	desc.Usage				= D3D11_USAGE_DEFAULT;			// GPU�ɂ��f�t�H���g�̓ǂݏ������\
	desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;	// �V�F�[�_����A�N�Z�X�ł���悤�Ƀo�C���h
	desc.CPUAccessFlags		= 0;							// CPU����̃A�N�Z�X�͕s�v
	desc.MiscFlags			= 0;							// ���̑��̃t���O
	return desc;
}

/* ========================================
	�V�F�[�_�[���\�[�X�r���[�쐬�֐�
	-------------------------------------
	���e�F�e�N�X�`�����\�[�X���쐬
	-------------------------------------
	����1�Fdesc			�e�N�X�`���ݒ�
	����2�FpData		�����f�[�^
	-------------------------------------
	�߂�l�FHRESULT		����
=========================================== */
HRESULT Texture::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	HRESULT hr = E_FAIL;

	// �e�N�X�`���쐬
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem		= pData;			// �e�N�X�`���f�[�^
	data.SysMemPitch	= desc.Width * 4;	// 1�s�̃o�C�g��

	// �e�N�X�`������
	hr = DirectXManager::GetDevice()->CreateTexture2D(&desc, pData ? &data : nullptr, &m_pTex);
	if (FAILED(hr)) { return hr; }

	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	// �t�H�[�}�b�g�ϊ�(�V�F�[�_�����̃f�[�^�𐳂���������ׂ�)
	switch (desc.Format)
	{
	default:							srvDesc.Format = desc.Format;			break;	// ���̂܂�
	case	DXGI_FORMAT_R32_TYPELESS: 	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;	break;	// 32bit���������_
	}
	srvDesc.ViewDimension		= D3D11_SRV_DIMENSION_TEXTURE2D;	// 2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;								// �~�b�v�}�b�v���x����

	
	// �V�F�[�_���\�[�X�r���[����
	hr = DirectXManager::GetDevice()->CreateShaderResourceView(m_pTex, &srvDesc, &m_pSRV);

	// �ǂݍ��ݎ��s
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// �����A�c���擾
	m_width		= desc.Width;
	m_height	= desc.Height;
	
	
	return hr;
}



/* ========================================
	�Q�b�^�[(����)�֐�
	-------------------------------------
	�ߒl�FUINT		����
=========================================== */
UINT Texture::GetWidth() const
{
	return m_width;
}

/* ========================================
	�Q�b�^�[(�c��)�֐�
	-------------------------------------
	�ߒl�FUINT		�c��
=========================================== */
UINT Texture::GetHeight() const
{
	return m_height;
}

/* ========================================
	�Q�b�^�[(�V�F�[�_���\�[�X�r���[)�֐�
	-------------------------------------
	�ߒl�FID3D11ShaderResourceView*		�V�F�[�_���\�[�X�r���[
=========================================== */
ID3D11ShaderResourceView* Texture::GetResource() const
{
	return m_pSRV;
}
