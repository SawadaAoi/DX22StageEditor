/* ========================================
	DX22Base/
	------------------------------------
	DirectX�Ǘ��pcpp
	------------------------------------
	DirectXManager.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "DirectXManager.h"		// DirectX�Ǘ��p�w�b�_
#include "Texture.h"			// �e�N�X�`���Ǘ��p�w�b�_
#include "WindowAPI.h"
#include <map>
#include "RenderTarget.h"
#include "DepthStencil.h"

#include "DebugMenu.h"	// �f�o�b�O���j���[


// =============== �萔��` =======================
const float SCREEN_RESET_COLOR[4] = { 0.8f, 0.9f, 1.0f, 1.0f };	// ��ʃN���A�J���[


// �J�����O�ݒ�
D3D11_CULL_MODE CULLING_MODE[DirectXManager::CullMode::CULL_MAX] = {
	D3D11_CULL_NONE,	// �J�����O����
	D3D11_CULL_FRONT,	// �O�ʃJ�����O
	D3D11_CULL_BACK		// �w�ʃJ�����O
};

// �u�����h���[�h
D3D11_BLEND BLEND_MODE[DirectXManager::BlendMode::BLEND_MAX][2] = {
		{D3D11_BLEND_ONE,				D3D11_BLEND_ZERO			},	// �u�����h����
		{D3D11_BLEND_SRC_ALPHA,			D3D11_BLEND_INV_SRC_ALPHA	},	// �A���t�@�u�����h
		{D3D11_BLEND_ONE,				D3D11_BLEND_ONE				},	// ���Z�u�����h
		{D3D11_BLEND_SRC_ALPHA,			D3D11_BLEND_ONE				},	// ���Z�A���t�@�u�����h
		{D3D11_BLEND_ZERO,				D3D11_BLEND_INV_SRC_COLOR	},	// ���Z�u�����h
		{D3D11_BLEND_INV_DEST_COLOR,	D3D11_BLEND_ONE				},	// �X�N���[���u�����h
};

// �[�x�X�e�[�g�ݒ�
std::pair< D3D11_DEPTH_WRITE_MASK, bool> DEPTH_MASK_PATTERN[DirectXManager::DepthState::DEPTH_MAX] =
{
	{D3D11_DEPTH_WRITE_MASK_ALL,	 true},	// �[�x�������݂ƃe�X�g�L��(�f�t�H���g)
	{D3D11_DEPTH_WRITE_MASK_ZERO,	 true},	// �[�x�������ݖ����e�X�g�L��(���߃I�u�W�F�N�g�A�e�p)
	{D3D11_DEPTH_WRITE_MASK_ZERO,	false},	// �[�x�`�����ݖ����e�X�g����(2D�`��p)
};

// �T���v���[�X�e�[�g�p�t�B���^�����O
D3D11_FILTER SAMPLER_FILTER[] = {
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,	// �|�C���g�t�B���^�����O(�e�N�X�`���̍ł��߂��s�N�Z���̐F���g�p���܂��B�G�b�W���͂�����Ƃ��Ă���A�ڂ₯����ۂ�����܂���)
		D3D11_FILTER_MIN_MAG_MIP_POINT,		// �c�������Ƀ��j�A�t�B���^�����O���g�p���A�~�b�v�}�b�v���x���̓|�C���g�t�B���^�����O���g�p���܂��B
};

// �����l
const DirectXManager::CullMode			DEFAULT_CULLING_MODE	= DirectXManager::CULL_BACK;				// �J�����O���[�h
const DirectXManager::BlendMode			EFAULT_BLEND_MODE		= DirectXManager::BLEND_ALPHA;				// �u�����h���[�h
const DirectXManager::DepthState		DEFAULT_DEPTH_STATE		= DirectXManager::DEPTH_ENABLE_WRITE_TEST;	// �[�x�X�e�[�g
const DirectXManager::SamplerState		DEFAULT_SAMPLER_STATE	= DirectXManager::SAMPLER_LINEAR;			// �T���v���[�X�e�[�g


/* ========================================
	DirectX�������֐�
	-------------------------------------
	���e�FDirectX�̏��������s��
	-------------------------------------
	�ߒl�FHRESULT�^�̖߂�l
=========================================== */
HRESULT DirectXManager::InitDirectX()
{
	

	HRESULT	hr = E_FAIL;	// �߂�l

	// �f�o�C�X�ƃX���b�v�`�F�C���̏�����
	if (FAILED(hr = InitDeviceAndSwapChain()))
	{
		MessageBox(NULL, "�f�o�C�X�ƃX���b�v�`�F�C���̏������Ɏ��s���܂���", "�G���[", MB_OK);

		return hr;
	}

	// �����_�[�^�[�Q�b�g�r���[�̏�����
	if (FAILED(hr = InitRenderTargetView()))
	{
		MessageBox(NULL, "�����_�[�^�[�Q�b�g�r���[�̏������Ɏ��s���܂���", "�G���[", MB_OK);

		return hr;
	}
	
	// �[�x�X�e���V���r���[�̏�����
	if (FAILED(hr = InitDepthStencilView()))
	{
		MessageBox(NULL, "�[�x�X�e���V���r���[�̏������Ɏ��s���܂���", "�G���[", MB_OK);

		return hr;
	}

	// ���X�^���C�U�X�e�[�g�̏�����
	if (FAILED(hr = InitRasterizerState()))
	{
		MessageBox(NULL, "���X�^���C�U�X�e�[�g�̏������Ɏ��s���܂���", "�G���[", MB_OK);

		return hr;
	}

	// �r���[�|�[�g�̏�����
	InitViewPort();

	// �[�x�X�e���V���X�e�[�g�̏�����
	if (FAILED(hr = InitDepthStencilState()))
	{
		MessageBox(NULL, "�[�x�X�e���V���X�e�[�g�̏������Ɏ��s���܂���", "�G���[", MB_OK);

		return hr;
	}

	// �u�����h�X�e�[�g�̏�����
	if (FAILED(hr = InitBlendState()))
	{
		MessageBox(NULL, "�u�����h�X�e�[�g�̏������Ɏ��s���܂���", "�G���[", MB_OK);

		return hr;
	}

	// �T���v���[�X�e�[�g�̏�����
	if (FAILED(hr = InitSamplerState()))
	{
		MessageBox(NULL, "�T���v���[�X�e�[�g�̏������Ɏ��s���܂���", "�G���[", MB_OK);

		return hr;
	}

	return S_OK;
}

/* ========================================
	DirectX����֐�
	-------------------------------------
	���e�FDirectX�̊e�ϐ��̉�����s��
=========================================== */
void DirectXManager::UninitDirectX()
{
	SAFE_DELETE(m_pDSV);
	SAFE_DELETE(m_pRTV);

	for (int i = 0; i < SAMPLER_MAX; ++i)
		SAFE_RELEASE(m_pSamplerState[i]);

	for (int i = 0; i < BLEND_MAX; ++i)
		SAFE_RELEASE(m_pBlendState[i]);

	for (int i = 0; i < CULL_MAX; ++i)
		SAFE_RELEASE(m_pRasterizerStates[i]);

	for (int i = 0; i < DEPTH_MAX; ++i)
		SAFE_RELEASE(m_pDepthStencilState[i]);

	if (m_pContext)
		m_pContext->ClearState();
	SAFE_RELEASE(m_pContext);
	if (m_pSwapChain)
		m_pSwapChain->SetFullscreenState(false, NULL);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDevice);
}

/* ========================================
	�f�o�C�X�E�X���b�v�`�F�C���������֐�
	-------------------------------------
	���e�F�f�o�C�X�ƃX���b�v�`�F�C���̏��������s��
	-------------------------------------
	�ߒl�FHRESULT�^�̖߂�l
=========================================== */
HRESULT DirectXManager::InitDeviceAndSwapChain()
{
	HRESULT	hr = E_FAIL;	// �߂�l

	// �o�b�N�o�b�t�@�̐ݒ�
	DXGI_MODE_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Width					= WINDOW_INST.GetScreenSize().nWidth;	// �E�B���h�E�̕�
	bd.Height					= WINDOW_INST.GetScreenSize().nHeight;	// �E�B���h�E�̍���
	bd.RefreshRate.Numerator	= WINDOW_INST.GetRefreshRate();			// ���t���b�V�����[�g_���q
	bd.RefreshRate.Denominator	= 1;									// ���t���b�V�����[�g_����
	bd.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;			// �s�N�Z���t�H�[�}�b�g�t�H�[�}�b�g
	bd.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// �X�L�������C���I�[�_�[
	bd.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;		// �X�P�[�����O

	// �o�b�N�o�b�t�@�̃}���`�T���v�����O�Ɋւ���ݒ�
	DXGI_SAMPLE_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Count	= 1;	// �T���v�����O��
	sd.Quality	= 0;	// �T���v�����O�i��

	// �X���b�v�`�F�C���̐ݒ�
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferDesc		= bd;										// �o�b�N�o�b�t�@�̐ݒ�
	scd.SampleDesc		= sd;										// �}���`�T���v���̐�
	scd.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;			// �o�b�N�o�b�t�@�̎g�p���@
	scd.BufferCount		= 1;										// �o�b�N�o�b�t�@�̐�
	scd.OutputWindow	= WINDOW_INST.GetHwnd();					// �֘A�t����E�C���h�E
	scd.Windowed		= TRUE;										// �E�B���h�E���[�h
	scd.SwapEffect		= DXGI_SWAP_EFFECT_DISCARD;					// �o�b�t�@�̌������@(�t���b�v��̃o�b�t�@��j��)
	scd.Flags			= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// �t���O	


	// �h���C�o�̎��
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,   // GPU�ŕ`��
		D3D_DRIVER_TYPE_WARP,       // �����x(�ᑬ
		D3D_DRIVER_TYPE_REFERENCE   // CPU�ŕ`��
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes); // �h���C�o�̐�

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; // �f�o�b�O���[�h
#endif

	// �@�\���x��(�A�v���P�[�V������GPU�̋@�\�ɉ����ēK�؂ȃ��x���œ��삳����)
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,     // DirectX11.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_11_0,     // DirectX11�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_1,     // DirectX10.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_0,     // DirectX10�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_3,      // DirectX9.3�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_2,      // DirectX9.2�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_1       // Direct9.1�Ή�GPU���x��
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);	// �@�\���x���̐�


	// ���@�\�ȃh���C�o��D�悵�ăf�o�C�X�𐶐�
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
	{
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,							// �f�B�X�v���C�f�o�C�X�̃A�_�v�^�iNULL�̏ꍇ�ŏ��Ɍ��������A�_�v�^�j
			driverTypes[driverTypeIndex],	// �f�o�C�X�h���C�o�̃^�C�v
			NULL,							// �\�t�g�E�F�A���X�^���C�U���g�p����ꍇ�Ɏw�肷��
			createDeviceFlags,				// �f�o�C�X�t���O
			featureLevels,					// �@�\���x��
			numFeatureLevels,				// �@�\���x����
			D3D11_SDK_VERSION,				// SDK�o�[�W����
			&scd,							// �X���b�v�`�F�C���̐ݒ�
			&m_pSwapChain,					// IDXGIDwapChain�C���^�t�F�[�X    
			&m_pDevice,						// ID3D11Device�C���^�t�F�[�X
			&m_FeatureLevel,				// �T�|�[�g����Ă���@�\���x��
			&m_pContext						// �f�o�C�X�R���e�L�X�g
		);
		// �f�o�C�X�̐����ɐ��������烋�[�v�𔲂���
		if (SUCCEEDED(hr)) { break; }
	}

	return hr;
}

/* ========================================
	�����_�[�^�[�Q�b�g�r���[�������֐�
	-------------------------------------
	���e�F�����_�[�^�[�Q�b�g�r���[�̏��������s��
	-------------------------------------
	�ߒl�FHRESULT�^�̖߂�l
=========================================== */
HRESULT DirectXManager::InitRenderTargetView()
{
	HRESULT	hr;

	// �����_�[�^�[�Q�b�g�N���X�쐬
	m_pRTV = new RenderTarget();
	if (FAILED(hr = m_pRTV->CreateFromScreen()))
	{
		return hr;	// ���s
	}

	SetRenderTargets(1, &m_pRTV, nullptr);	// �����_�[�^�[�Q�b�g�̐ݒ�

	return hr;
}


/* ========================================
	�[�x�X�e���V���r���[�������֐�
	-------------------------------------
	���e�F�[�x�X�e���V���r���[�̏��������s��
	-------------------------------------
	�ߒl�FHRESULT�^�̖߂�l
=========================================== */
HRESULT DirectXManager::InitDepthStencilView()
{
	HRESULT	hr;

	// �[�x�X�e���V���r���[�N���X�쐬
	m_pDSV = new DepthStencil();

	// �[�x�X�e���V���r���[�̍쐬
	hr = m_pDSV->Create(m_pRTV->GetWidth(), m_pRTV->GetHeight(), false);
	
	
	return hr;
}

/* ========================================
	�r���[�|�[�g�������֐�
	-------------------------------------
	���e�F�r���[�|�[�g�̏��������s��
=========================================== */
void DirectXManager::InitViewPort()
{
	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width	= (float)WINDOW_INST.GetScreenSize().nWidth;
	vp.Height	= (float)WINDOW_INST.GetScreenSize().nHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_pContext->RSSetViewports(1, &vp);
}

/* ========================================
	�r���[�|�[�g�������֐�
	-------------------------------------
	���e�F�r���[�|�[�g�̏��������s��
	-------------------------------------
	����1�Fint�^�̕�
	����2�Fint�^�̍���
=========================================== */
void DirectXManager::InitViewport(int width, int height)
{
	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width	= (float)width;
	vp.Height	= (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_pContext->RSSetViewports(1, &vp);
}


/* ========================================
	���X�^���C�U�X�e�[�g�������֐�
	-------------------------------------
	���e�F���X�^���C�U�X�e�[�g�̏��������s��
	-------------------------------------
	�ߒl�FHRESULT�^�̖߂�l
=========================================== */
HRESULT DirectXManager::InitRasterizerState()
{
	HRESULT	hr;

	// ���X�^���C�U�X�e�[�g�̐ݒ�(�J�����O�ݒ�ȊO�͐�ɐݒ肵�Ă���)
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode					= D3D11_FILL_SOLID;	// �|���S���̓h��Ԃ�
	rd.FrontCounterClockwise	= FALSE;			// �t�����g�t�F�C�X�̎��v���
	rd.DepthBias				= 0;				// �[�x�o�C�A�X
	rd.DepthBiasClamp			= 0.0f;				// �[�x�o�C�A�X�N�����v
	rd.SlopeScaledDepthBias		= 0.0f;				// �X���[�v�X�P�[���[�x�o�C�A�X
	rd.DepthClipEnable			= TRUE;				// �[�x�N���b�s���O�L��
	rd.ScissorEnable			= FALSE;			// �V�U�[��`�L��
	rd.MultisampleEnable		= FALSE;			// �}���`�T���v�����O�L��
	rd.AntialiasedLineEnable	= FALSE;			// �A���`�G�C���A�X�L��



	// �J�����O���[�h�̎�ޕ��쐬
	for (int i = 0; i < CullMode::CULL_MAX; ++i)
	{
		// �J�����O���[�h��ݒ�
		rd.CullMode = CULLING_MODE[i];		
		// ���X�^���C�U�X�e�[�g�̍쐬
		hr = m_pDevice->CreateRasterizerState(&rd, &m_pRasterizerStates[i]);

		// �쐬�Ɏ��s������I��
		if (FAILED(hr)) { return hr; }
	}

	// ���X�^���C�U�X�e�[�g��ݒ�
	SetCullingMode(DEFAULT_CULLING_MODE);	// �f�t�H���g�͔w�ʃJ�����O

	return hr;
}


/* ========================================
	�u�����h�X�e�[�g�������֐�
	-------------------------------------
	���e�F�u�����h�X�e�[�g�̏��������s��
	-------------------------------------
	�ߒl�FHRESULT�^�̖߂�l
=========================================== */
HRESULT DirectXManager::InitBlendState()
{
	HRESULT	hr;

	//--- �A���t�@�u�����f�B���O
	// https://pgming-ctrl.com/directx11/blend/

	// �����_�[�^�[�Q�b�g���Ƃ̃u�����h�X�e�[�g�̐ݒ�
	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));
	rtbd.BlendEnable			= TRUE;							// �u�����h�L��
	rtbd.BlendOp				= D3D11_BLEND_OP_ADD;			// �u�����h���Z
	rtbd.SrcBlendAlpha			= D3D11_BLEND_ONE;				// �\�[�X�u�����h�A���t�@
	rtbd.DestBlendAlpha			= D3D11_BLEND_ZERO;				// �f�X�e�B�l�[�V�����u�����h�A���t�@
	rtbd.BlendOpAlpha			= D3D11_BLEND_OP_ADD;			// �u�����h���Z�A���t�@
	rtbd.RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;	// �����_�[�^�[�Q�b�g���C�g�}�X�N

	// �u�����h�X�e�[�g�̐ݒ�
	D3D11_BLEND_DESC bd = {};
	ZeroMemory(&bd, sizeof(bd));
	bd.AlphaToCoverageEnable	= TRUE;		// �A���t�@�E�J�o���b�W(���߉摜�̏d�Ȃ蓙)�@���d�v
	bd.IndependentBlendEnable	= FALSE;	// �Ɨ��u�����h�L��(�擪�̐ݒ��S�Ẵ����_�[�^�[�Q�b�g�ɓK�p)
	

	// �u�����h���[�h�̎�ޕ��쐬
	for (int i = 0; i < BLEND_MAX; ++i)
	{
		rtbd.SrcBlend	= BLEND_MODE[i][0];	// �\�[�X�u�����h
		rtbd.DestBlend	= BLEND_MODE[i][1];	// �f�X�e�B�l�[�V�����u�����h
		bd.RenderTarget[0] = rtbd;		// �����_�[�^�[�Q�b�g��ݒ�

		// �u�����h�X�e�[�g�̍쐬
		hr = m_pDevice->CreateBlendState(&bd, &m_pBlendState[i]);
		
		// �쐬�Ɏ��s������I��
		if (FAILED(hr)) { return hr; }
	}
	
	// �u�����h�X�e�[�g��ݒ�
	SetBlendMode(EFAULT_BLEND_MODE);	// �f�t�H���g�̓A���t�@�u�����h

	return hr;
}


/* ========================================
	�[�x�X�e���V���X�e�[�g�������֐�
	-------------------------------------
	���e�F�[�x�X�e���V���X�e�[�g�̏��������s��
	-------------------------------------
	�ߒl�FHRESULT�^�̖߂�l
=========================================== */
HRESULT DirectXManager::InitDepthStencilState()
{
	HRESULT	hr;

	//--- �[�x�e�X�g
	// https://tositeru.github.io/ImasaraDX11/part/ZBuffer-and-depth-stencil
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthFunc						= D3D11_COMPARISON_LESS_EQUAL;		// �[�x�e�X�g�֐�(�f�t�H���g��
	dsDesc.StencilReadMask					= D3D11_DEFAULT_STENCIL_READ_MASK;	// �X�e���V���ǂݎ��}�X�N
	dsDesc.StencilWriteMask					= D3D11_DEFAULT_STENCIL_WRITE_MASK;	// �X�e���V���������݃}�X�N
	dsDesc.FrontFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;			// �X�e���V���e�X�g���s���̏���
	dsDesc.FrontFace.StencilDepthFailOp		= D3D11_STENCIL_OP_KEEP;			// �X�e���V���e�X�g���s���̐[�x����
	dsDesc.FrontFace.StencilPassOp			= D3D11_STENCIL_OP_INCR;			// �X�e���V���e�X�g�������̏���
	dsDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_GREATER_EQUAL;	// �X�e���V���e�X�g�֐�
	dsDesc.BackFace							= dsDesc.FrontFace;					// �o�b�N�t�F�C�X�̐ݒ�
	
	for (int i = 0; i < DepthState::DEPTH_MAX; i++)
	{
		dsDesc.DepthWriteMask		= DEPTH_MASK_PATTERN[i].first;		// �[�x�������݃}�X�N�ݒ�
		dsDesc.DepthEnable			= DEPTH_MASK_PATTERN[i].second;	// �[�x�e�X�g�L��
		dsDesc.StencilEnable		= DEPTH_MASK_PATTERN[i].second;	// �X�e���V���e�X�g�L��

		// �[�x�X�e�[�g�̍쐬
		hr = m_pDevice->CreateDepthStencilState(&dsDesc, &m_pDepthStencilState[i]);

		// �쐬�Ɏ��s������I��
		if (FAILED(hr)) { return hr; }
	}

	// �[�x�X�e�[�g��ݒ�
	SetDepthTest(DEFAULT_DEPTH_STATE);	// �f�t�H���g�͐[�x�������݂ƃe�X�g�L��


	return hr;
}

/* ========================================
	�T���v���[�X�e�[�g�������֐�
	-------------------------------------
	���e�F�T���v���[�X�e�[�g�̏��������s��
	-------------------------------------
	�ߒl�FHRESULT�^�̖߂�l
=========================================== */
HRESULT DirectXManager::InitSamplerState()
{
	HRESULT	hr;

	// �T���v���[
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;		// �A�h���XU
	samplerDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;		// �A�h���XV
	samplerDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;		// �A�h���XW
	samplerDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;			// ��r�֐�
	samplerDesc.MinLOD			= 0;								// �ŏ�LOD
	samplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;				// �ő�LOD
	samplerDesc.MaxAnisotropy	= 1;								// �ő�ٕ���
	samplerDesc.BorderColor[0]	= 0;								// �{�[�_�[�J���[
	samplerDesc.BorderColor[1]	= 0;								// �{�[�_�[�J���[
	samplerDesc.BorderColor[2]	= 0;								// �{�[�_�[�J���[
	samplerDesc.BorderColor[3]	= 0;								// �{�[�_�[�J���[

	for (int i = 0; i < SAMPLER_MAX; ++i)
	{
		// �t�B���^�����O�ݒ�
		samplerDesc.Filter = SAMPLER_FILTER[i];	

		// �T���v���[�X�e�[�g�̍쐬
		hr = m_pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState[i]);

		// �쐬�Ɏ��s������I��
		if (FAILED(hr)) { return hr; }
	}

	SetSamplerState(DEFAULT_SAMPLER_STATE);	// �f�t�H���g�͐��`���

	return hr;
}


/* ========================================
	DirectX�`��J�n�֐�
	-------------------------------------
	���e�F�`��J�n�������s��
=========================================== */
void DirectXManager::BeginDrawDirectX()
{
	float fClearColor[4];

#ifdef _DEBUG
	DirectX::XMFLOAT4 color = WIN_BASIC_SETTING["BackColor"].GetColor();
	fClearColor[0] = color.x;
	fClearColor[1] = color.y;
	fClearColor[2] = color.z;
	fClearColor[3] = color.w;
#else
	fClearColor[0] = SCREEN_RESET_COLOR[0];
	fClearColor[1] = SCREEN_RESET_COLOR[1];
	fClearColor[2] = SCREEN_RESET_COLOR[2];
	fClearColor[3] = SCREEN_RESET_COLOR[3];
#endif // _DEBUG

	m_pRTV->Clear(fClearColor);
	m_pDSV->Clear();

}

/* ========================================
	DirectX�`��J�n�֐�
	-------------------------------------
	���e�F�`��J�n�������s��
	-------------------------------------
	����1�Ffloat�^�z�� 
=========================================== */
void DirectXManager::BeginDrawDirectX(float* pClearColor)
{
	m_pRTV->Clear(pClearColor);
	m_pDSV->Clear();

}

/* ========================================
	DirectX�`��I���֐�
	-------------------------------------
	���e�F�`��I���������s��
=========================================== */
void DirectXManager::EndDrawDirectX()
{
	m_pSwapChain->Present(0, 0);
}



/* ========================================
	�����_�[�^�[�Q�b�g�ݒ�֐�
	-------------------------------------
	���e�F�����_�[�^�[�Q�b�g��ݒ肷��
	-------------------------------------
	����1�F�����_�[�^�[�Q�b�g�̐�
	����2�FRenderTarget**�^��ppViews(�z��)
	����3�FDepthStencil*�^��pView
=========================================== */
void DirectXManager::SetRenderTargets(UINT num, RenderTarget** ppRTViews, DepthStencil* pDepthStencilView)
{
	static ID3D11RenderTargetView* rtvs[4];

	if (num > 4) num = 4;	// �ő�4�܂�

	// �����_�[�^�[�Q�b�g�̐������擾
	for (UINT i = 0; i < num; ++i)
	{
		rtvs[i] = ppRTViews[i]->GetView();
	}

	// �����_�[�^�[�Q�b�g�̐ݒ�
	m_pContext->OMSetRenderTargets(
		num,														// �����_�[�^�[�Q�b�g�̐�
		rtvs,														// �����_�[�^�[�Q�b�g
		pDepthStencilView ? pDepthStencilView->GetView() : nullptr	// �[�x�X�e���V���r���[(�Ȃ��ꍇ��nullptr)
	);

}

/* ========================================
	�[�x�X�e���V���r���[�I���I�t�֐�
	-------------------------------------
	���e�F�[�x�X�e���V���r���[�̗L��������
		�@�؂�ւ���
	-------------------------------------
	����1�Ftrue�Ȃ�L���Afalse�Ȃ疳��
=========================================== */
void DirectXManager::OnOffDepthStencilView(bool enable)
{
	static ID3D11RenderTargetView* pRTV = m_pRTV->GetView();	// �����_�[�^�[�Q�b�g�r���[

	// �[�x�X�e���V���r���[�̗L��������؂�ւ���
	if (enable)
	{
		m_pContext->OMSetRenderTargets(1, &pRTV, m_pDSV->GetView());
	}
	else
	{
		m_pContext->OMSetRenderTargets(1, &pRTV, nullptr);
	}
}

/* ========================================
	�J�����O���[�h�ݒ�֐�
	-------------------------------------
	���e�F�J�����O���[�h��ݒ肷��
	-------------------------------------
	����1�FCullMode�^��cull
=========================================== */
void DirectXManager::SetCullingMode(CullMode cull)
{
	// �J�����O���[�h���͈͊O�̏ꍇ�͏����𔲂���
	if (cull < 0 || cull >= CULL_MAX) return;

	// �����̒萔�ɑΉ����郉�X�^���C�U�X�e�[�g��ݒ�
	m_pContext->RSSetState(m_pRasterizerStates[cull]);
}

/* ========================================
	�[�x�e�X�g�ݒ�֐�
	-------------------------------------
	���e�F�[�x�e�X�g��ݒ肷��
	-------------------------------------
	����1�FDepthState�^��state
=========================================== */
void DirectXManager::SetDepthTest(DepthState state)
{
	// �[�x�X�e�[�g���͈͊O�̏ꍇ�͏����𔲂���
	if (state < 0 || state >= DEPTH_MAX) return;

	// �[�x�X�e���V���X�e�[�g�̐ݒ�
	m_pContext->OMSetDepthStencilState(m_pDepthStencilState[state], 0);
}

/* ========================================
	�u�����h���[�h�ݒ�֐�
	-------------------------------------
	���e�F�u�����h���[�h��ݒ肷��
	-------------------------------------
	����1�FBlendMode�^��blend
=========================================== */
void DirectXManager::SetBlendMode(BlendMode blend)
{
	// �u�����h���[�h���͈͊O�̏ꍇ�͏����𔲂���
	if (blend < 0 || blend >= BLEND_MAX) return;

	// �u�����h�t�@�N�^�[(�F�̍������@)�̐ݒ�
	FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };

	// �u�����h�X�e�[�g�̐ݒ�
	m_pContext->OMSetBlendState(m_pBlendState[blend], blendFactor, 0xffffffff);
}

/* ========================================
	�T���v���[�X�e�[�g�ݒ�֐�
	-------------------------------------
	���e�F�T���v���[�X�e�[�g��ݒ肷��
	-------------------------------------
	����1�FSamplerState�^��state
=========================================== */
void DirectXManager::SetSamplerState(SamplerState state)
{
	// �T���v���[�X�e�[�g���͈͊O�̏ꍇ�͏����𔲂���
	if (state < 0 || state >= SAMPLER_MAX) return;

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pContext->PSSetSamplers(0, 1, &m_pSamplerState[state]);
}


/* ========================================
	�Q�b�^�[(�f�o�C�X)�֐�
	-------------------------------------
	�ߒl�FID3D11Device*�^
=========================================== */
ID3D11Device* DirectXManager::GetDevice()
{
	return m_pDevice;
}

/* ========================================
	�Q�b�^�[(�f�o�C�X�R���e�L�X�g)�֐�
	-------------------------------------
	�ߒl�FID3D11DeviceContext*�^
=========================================== */
ID3D11DeviceContext* DirectXManager::GetContext()
{
	return m_pContext;
}

/* ========================================
	�Q�b�^�[(�X���b�v�`�F�C��)�֐�
	-------------------------------------
	�ߒl�FIDXGISwapChain*�^
=========================================== */
IDXGISwapChain* DirectXManager::GetSwapChain()
{
	return m_pSwapChain;
}

/* ========================================
	�Q�b�^�[(�����_�[�^�[�Q�b�g)�֐�
	-------------------------------------
	�ߒl�FRenderTarget*�^
=========================================== */
RenderTarget* DirectXManager::GetDefaultRTV()
{
	return m_pRTV;
}

/* ========================================
	�Q�b�^�[(�[�x�X�e���V��)�֐�
	-------------------------------------
	�ߒl�FDepthStencil*�^
=========================================== */
DepthStencil* DirectXManager::GetDefaultDSV()
{
	return m_pDSV;
}