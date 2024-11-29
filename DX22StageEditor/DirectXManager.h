/* ========================================
	DX22Base/
	------------------------------------
	DirectX�Ǘ��p�w�b�_
	------------------------------------
	����:DirextX�̊Ǘ����s��
	------------------------------------
	DirectXManager.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include <d3d11.h>					// DirectX11�̃w�b�_
#pragma comment(lib, "d3d11.lib")	// DirectX11�̃��C�u����

// =============== �萔��` =======================
#define SAFE_DELETE(p) do{if(p){delete p; p = nullptr;}}while(0)			// ����������}�N��
#define SAFE_DELETE_ARRAY(p) do{if(p){delete[] p; p = nullptr;}}while(0)	// �z�񃁃�������}�N��
#define SAFE_RELEASE(p) do{if(p){p->Release(); p = nullptr;}}while(0)		// ���\�[�X����}�N��

class RenderTarget;
class DepthStencil;

// =============== �N���X��` =====================
class DirectXManager
{
public:
	// �u�����h���[�h
	enum BlendMode
	{
		BLEND_NONE,			// �u�����h����
		BLEND_ALPHA,		// �A���t�@�u�����h
		BLEND_ADD,			// ���Z�u�����h
		BLEND_ADDALPHA,		// ���Z�A���t�@�u�����h
		BLEND_SUB,			// ���Z�u�����h
		BLEND_SCREEN,		// �X�N���[���u�����h
		BLEND_MAX
	};

	// �J�����O���[�h
	enum CullMode
	{
		CULL_NONE,		// �J�����O����
		CULL_FRONT,		// �O�ʃJ�����O
		CULL_BACK,		// ���ʃJ�����O
		CULL_MAX
	};

	// �T���v���[�X�e�[�g
	enum SamplerState
	{
		SAMPLER_LINEAR,	// ���`���
		SAMPLER_POINT,	// �ŋߖT���
		SAMPLER_MAX
	};

	// �[�x�X�e�[�g
	enum DepthState
	{
		DEPTH_ENABLE_WRITE_TEST,	// �[�x�������݂ƃe�X�g�L��
		DEPTH_ENABLE_TEST,			// �[�x�e�X�g�̂ݗL��
		DEPTH_DISABLE,				// �[�x����
		DEPTH_MAX
	};

public:

	static HRESULT InitDirectX();
	static void UninitDirectX();
	static void BeginDrawDirectX();
	static void BeginDrawDirectX(float* pClearColor);
	static void EndDrawDirectX();

	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetContext();
	static IDXGISwapChain* GetSwapChain();
	static RenderTarget* GetDefaultRTV();
	static DepthStencil* GetDefaultDSV();
	static void SetRenderTargets(UINT num, RenderTarget** ppViews, DepthStencil* pView);
	static void OnOffDepthStencilView(bool enable);

	static void SetCullingMode(CullMode cull);
	static void SetDepthTest(DepthState state);
	static void SetBlendMode(BlendMode blend);
	static void SetSamplerState(SamplerState state);

private:
	static HRESULT	InitDeviceAndSwapChain();
	static HRESULT	InitRenderTargetView();
	static HRESULT	InitDepthStencilView();
	static void		InitViewPort();
	static void		InitViewport(int width, int height);
	static HRESULT	InitRasterizerState();
	static HRESULT	InitBlendState();
	static HRESULT	InitDepthStencilState();
	static HRESULT	InitSamplerState();

	static inline ID3D11Device*				m_pDevice;		// �f�o�C�X
	static inline ID3D11DeviceContext*		m_pContext;		// �f�o�C�X�R���e�L�X�g
	static inline IDXGISwapChain*			m_pSwapChain;	// �X���b�v�`�F�C��
	static inline D3D_FEATURE_LEVEL			m_FeatureLevel;	// �t�B�[�`���[���x��

	static inline RenderTarget*				m_pRTV;			// �����_�[�^�[�Q�b�g
	static inline DepthStencil*				m_pDSV;			// �[�x�X�e���V��

	static inline ID3D11RasterizerState*				m_pRasterizerStates[CULL_MAX];			// ���X�^���C�U�X�e�[�g(�z��)
	static inline ID3D11BlendState*						m_pBlendState[BLEND_MAX];		// �u�����h�X�e�[�g
	static inline ID3D11SamplerState*					m_pSamplerState[SAMPLER_MAX];	// �T���v���[�X�e�[�g

	static inline ID3D11DepthStencilState*				m_pDepthStencilState[DEPTH_MAX];	// �[�x�X�e�[�g

	static inline D3D11_VIEWPORT						m_ViewPort;		// �r���[�|�[�g

};