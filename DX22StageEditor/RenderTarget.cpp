/* ========================================
	DX22Base/
	------------------------------------
	�����_�[�^�[�Q�b�g�pcpp
	------------------------------------
	RenderTarget.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "RenderTarget.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
========================================== */
RenderTarget::RenderTarget()
	: m_pRTV(nullptr)
{
}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F���
========================================== */
RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(m_pRTV);
}

/* ========================================
	�N���A�֐�
	-------------------------------------
	���e�F�����_�[�^�[�Q�b�g���N���A
========================================== */
void RenderTarget::Clear()
{
	static float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Clear(color);
}

/* ========================================
	�N���A�֐�
	-------------------------------------
	���e�F�����_�[�^�[�Q�b�g���N���A
	-------------------------------------
	�����Fcolor		�N���A�F
========================================== */
void RenderTarget::Clear(const float* color)
{
	DirectXManager::GetContext()->ClearRenderTargetView(m_pRTV, color);
}


/* ========================================
	�����_�[�^�[�Q�b�g�쐬�֐�
	-------------------------------------
	���e�F�t�H�[�}�b�g���w�肵��
		�@�����_�[�^�[�Q�b�g���쐬
	-------------------------------------
	����1�Fformat		�t�H�[�}�b�g
	����2�Fwidth		����
	����3�Fheight		�c��
	-------------------------------------
	�ߒl�FHRESULT		����
=========================================== */
HRESULT RenderTarget::Create(DXGI_FORMAT format, UINT width, UINT height)
{
	// �e�N�X�`���\���̍쐬
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);	

	// �����_�[�^�[�Q�b�g�Ƃ��Ďg�p
	desc.BindFlags	|= D3D11_BIND_RENDER_TARGET;	// �e�N�X�`���̎g�p��				

	return CreateResource(desc);	// ���\�[�X�쐬
}

/* ========================================
	�����_�[�^�[�Q�b�g�쐬�֐�
	-------------------------------------
	���e�F�X���b�v�`�F�C�����烌���_�[�^�[�Q�b�g���쐬
	-------------------------------------
	�ߒl�FHRESULT		����
=========================================== */
HRESULT RenderTarget::CreateFromScreen()
{
	HRESULT hr;	

	// �X���b�v�`�F�C���̃o�b�N�o�b�t�@�̃|�C���^���擾(m_pTex�ɐݒ�)
	hr = DirectXManager::GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pTex);

	// ���s
	if (FAILED(hr)) { return hr; }


	// �����_�[�^�[�Q�b�g�r���[�̏ڍאݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;		// RTV ���֘A�t�����郊�\�[�X�̃t�H�[�}�b�g
	rtvDesc.ViewDimension	= D3D11_RTV_DIMENSION_TEXTURE2D;	// �r���[�̎��� (1D, 2D, 3D)�B

	// �����_�[�^�[�Q�b�g�r���[���쐬
	hr = DirectXManager::GetDevice()->CreateRenderTargetView(m_pTex, &rtvDesc, &m_pRTV);

	if (FAILED(hr)) { return hr; }

	// �e�N�X�`���̏����擾���A���ƍ�����ݒ�
	D3D11_TEXTURE2D_DESC desc;
	m_pTex->GetDesc(&desc);
	m_width = desc.Width;
	m_height = desc.Height;
	
	return hr;
}

/* ========================================
	���\�[�X�쐬�֐�
	-------------------------------------
	���e�F���\�[�X�쐬
	-------------------------------------
	����1�Fdesc		�e�N�X�`���\����
	����2�FpData		�����f�[�^
	-------------------------------------
	�ߒl�FHRESULT		����
=========================================== */
HRESULT RenderTarget::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
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
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format			= desc.Format;						// �t�H�[�}�b�g
	rtvDesc.ViewDimension	= D3D11_RTV_DIMENSION_TEXTURE2D;	// �r���[�̎��� (1D, 2D, 3D)�B

	// �����_�[�^�[�Q�b�g�r���[����
	hr = DirectXManager::GetDevice()->CreateRenderTargetView(m_pTex, &rtvDesc, &m_pRTV);
	if (FAILED(hr)) { return hr; }

	// ���ƍ�����ݒ�
	m_width		= desc.Width;
	m_height	= desc.Height;

	return hr;

}


/* ========================================
	�Q�b�^�[(�r���[�擾)
	-------------------------------------
	�ߒl�FID3D11RenderTargetView*	�����_�[�^�[�Q�b�g�r���[
=========================================== */
ID3D11RenderTargetView* RenderTarget::GetView() const
{
	return m_pRTV;
}