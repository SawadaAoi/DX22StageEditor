/* ========================================
	DX22Base/
	------------------------------------
	�f�v�X�X�e���V���r���[�pcpp
	------------------------------------
	DepthStencil.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "DepthStencil.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
========================================== */
DepthStencil::DepthStencil()
	: m_pDSV(nullptr)
{
}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F���
========================================== */
DepthStencil::~DepthStencil()
{
	SAFE_RELEASE(m_pDSV);
}


/* ========================================
	�N���A�֐�
	-------------------------------------
	���e�F�f�v�X�X�e���V���r���[���N���A
========================================== */
void DepthStencil::Clear()
{
	DirectXManager::GetContext()->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}


/* ========================================
	�쐬�֐�
	-------------------------------------
	���e�F�f�v�X�X�e���V���r���[���쐬
	-------------------------------------
	����1�Fwidth		����
	����2�Fheight		�c��
	����3�FuseStencil	�X�e���V���g�p�t���O
	-------------------------------------
	�߂�l�FHRESULT	����
========================================== */
HRESULT DepthStencil::Create(UINT width, UINT height, bool useStencil)
{
	// https://docs.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-depth-stencil#compositing
	// �t�H�[�}�b�g���w�肵�āA�e�N�X�`���ݒ�\���̂��쐬
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(useStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS, width, height);

	// �X�e���V���g�p����
	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;		// �e�N�X�`���̎g�p��	

	// ����
	return CreateResource(desc);
}


/* ========================================
	���\�[�X�쐬�֐�
	-------------------------------------
	���e�F���\�[�X���쐬
	-------------------------------------
	����1�Fdesc		�e�N�X�`���ݒ�
	����2�FpData	�����f�[�^
	-------------------------------------
	�߂�l�FHRESULT	����
========================================== */
HRESULT DepthStencil::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	// �X�e���V���g�p����
	bool useStencil = (desc.Format == DXGI_FORMAT_R24G8_TYPELESS);

	HRESULT hr;

	// �e�N�X�`���쐬
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem		= pData;			// �e�N�X�`���f�[�^
	data.SysMemPitch	= desc.Width * 4;	// 1�s�̃o�C�g��

	// �e�N�X�`������
	hr = DirectXManager::GetDevice()->CreateTexture2D(&desc, pData ? &data : nullptr, &m_pTex);
	if (FAILED(hr)) { return hr; }

	// �ݒ�
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format			= useStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;	// �t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;											// �r���[�̎��� (1D, 2D, 3D)�B

	// �f�v�X�X�e���V���r���[����
	return DirectXManager::GetDevice()->CreateDepthStencilView(m_pTex, &dsvDesc, &m_pDSV);
}


/* ========================================
	�Q�b�^�[(�r���[�擾)
	-------------------------------------
	�ߒl�FID3D11DepthStencilView*		�r���[
=========================================== */
ID3D11DepthStencilView* DepthStencil::GetView() const
{
	return m_pDSV;
}