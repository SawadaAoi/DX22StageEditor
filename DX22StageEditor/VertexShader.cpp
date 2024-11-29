/* ========================================
	DX22Base/
	------------------------------------
	���_�V�F�[�_�[�pcpp
	------------------------------------
	VertexShader.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "VertexShader.h"
#include <d3dcompiler.h>	// �V�F�[�_�[�̃R���p�C��


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
=========================================== */
VertexShader::VertexShader()
	: Shader(Shader::Vertex)	// �V�F�[�_�[�̎�ނ𒸓_�V�F�[�_�ɐݒ�
	, m_pVS(nullptr)
	, m_pInputLayout(nullptr)
{
}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F�������
=========================================== */
VertexShader::~VertexShader()
{
	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pVS);
}

/* ========================================
	�o�C���h�֐�
	-------------------------------------
	���e�F���_�V�F�[�_�[�� GPU �Ƀo�C���h���A
		�@�`�揈���Ŏg�p�ł����Ԃɂ���
=========================================== */
void VertexShader::Bind(void)
{
	// �f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext* pContext = DirectXManager::GetContext();

	pContext->VSSetShader(m_pVS, NULL, 0);		// ���_�V�F�[�_�[���o�C���h
	pContext->IASetInputLayout(m_pInputLayout);	// �C���v�b�g���C�A�E�g���o�C���h

	// �萔�o�b�t�@�̐������o�C���h
	for (int i = 0; i < m_pBuffers.size(); ++i)
	{
		pContext->VSSetConstantBuffers(i, 1, &m_pBuffers[i]);
	}
	// �e�N�X�`���̐������o�C���h
	for (int i = 0; i < m_pTextures.size(); ++i)
	{
		pContext->VSSetShaderResources(i, 1, &m_pTextures[i]);
	}
}

/* ========================================
	�V�F�[�_�[�쐬�֐�
	-------------------------------------
	���e�F�V�F�[�_�[�̃o�C�g�R�[�h���璸�_�V�F�[�_�[���쐬�B
		  ���_�o�b�t�@�̓��̓f�[�^�`�����`����C���v�b�g���C�A�E�g���쐬
	-------------------------------------
	����1�FpData		�f�[�^
	����2�Fsize		�T�C�Y
	-------------------------------------
	�߂�l�F�G���[�R�[�h
=========================================== */
HRESULT VertexShader::MakeShader(void* pData, UINT size)
{
	HRESULT hr;
	ID3D11Device* pDevice = DirectXManager::GetDevice();	// �f�o�C�X�̎擾

	// �V�F�[�_�[�쐬
	hr = pDevice->CreateVertexShader(pData, size, NULL, &m_pVS);
	if (FAILED(hr)) { return hr; }

	/*
	�V�F�[�_�쐬���ɃV�F�[�_���t���N�V������ʂ��ăC���v�b�g���C�A�E�g���擾
	�Z�}���e�B�N�X�̔z�u�Ȃǂ��环�ʎq���쐬
	���ʎq���o�^�ρ��ė��p�A�Ȃ���ΐV�K�쐬
	https://blog.techlab-xe.net/dxc-shader-reflection/
	*/

	ID3D11ShaderReflection*			pReflection;	// �V�F�[�_���t���N�V����
	D3D11_SHADER_DESC				shaderDesc;		// �V�F�[�_�[�̏��
	D3D11_INPUT_ELEMENT_DESC*		pInputDesc;		// �C���v�b�g���C�A�E�g
	D3D11_SIGNATURE_PARAMETER_DESC	sigDesc;		// �V�O�l�`��

	DXGI_FORMAT formats[][4] =
	{
		{
			DXGI_FORMAT_R32_UINT,			// 1��32�r�b�g�����Ȃ�����
			DXGI_FORMAT_R32G32_UINT,		// 2��32�r�b�g�����Ȃ�����
			DXGI_FORMAT_R32G32B32_UINT,		// 3��32�r�b�g�����Ȃ�����
			DXGI_FORMAT_R32G32B32A32_UINT	// 4��32�r�b�g�����Ȃ�����
		},
		{
			DXGI_FORMAT_R32_SINT,
			DXGI_FORMAT_R32G32_SINT,
			DXGI_FORMAT_R32G32B32_SINT,
			DXGI_FORMAT_R32G32B32A32_SINT,
		}, 
		{
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_R32G32B32_FLOAT,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
		},
	};

	// �V�F�[�_���t���N�V�����̍쐬
	hr = D3DReflect(pData, size, IID_PPV_ARGS(&pReflection));	// �V�F�[�_�[�o�C�g�R�[�h����͂��āA���̏��𔽉f����֐�
	if (FAILED(hr)) { return hr; }

	pReflection->GetDesc(&shaderDesc);	// �V�F�[�_�[�̏ڍ׏��i�Ⴆ�΁A���̓p�����[�^�̐���\���j���擾
	pInputDesc = new D3D11_INPUT_ELEMENT_DESC[shaderDesc.InputParameters];	// InputParameters: ���̓p�����[�^�̐�

	// �C���v�b�g���C�A�E�g�̍쐬
	for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
	{
		// �V�F�[�_�[�����҂���e���̓p�����[�^�̃Z�}���e�B�N�X�iPOSITION, TEXCOORD, NORMAL �Ȃǁj�₻�̃C���f�b�N�X���擾
		pReflection->GetInputParameterDesc(i, &sigDesc);

		// �Z�}���e�B�N�X���A�Z�}���e�B�N�X�C���f�b�N�X���擾
		pInputDesc[i].SemanticName	= sigDesc.SemanticName;		// �Z�}���e�B�N�X��
		pInputDesc[i].SemanticIndex = sigDesc.SemanticIndex;	// �Z�}���e�B�N�X�C���f�b�N�X

		// ���_�t�H�[�}�b�g�̌���
		// http://marupeke296.com/TIPS_No17_Bit.html
		BYTE elementCount = sigDesc.Mask;
		// �R���|�[�l���g�����v�Z���ăt�H�[�}�b�g������
		elementCount = (elementCount & 0x05) + ((elementCount >> 1) & 0x05);	// 0x05 = 0101
		elementCount = (elementCount & 0x03) + ((elementCount >> 2) & 0x03);	// 0x03 = 0011

		// �R���|�[�l���g�^�C�v�ɂ���ăt�H�[�}�b�g������
		switch (sigDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			pInputDesc[i].Format = formats[0][elementCount - 1];
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			pInputDesc[i].Format = formats[1][elementCount - 1];
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			pInputDesc[i].Format = formats[2][elementCount - 1];
			break;
		}
		pInputDesc[i].InputSlot = 0;
		pInputDesc[i].AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
		pInputDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pInputDesc[i].InstanceDataStepRate = 0;
	}

	// �C���v�b�g���C�A�E�g�̍쐬
	hr = pDevice->CreateInputLayout(
		pInputDesc, shaderDesc.InputParameters,
		pData, size, &m_pInputLayout
	);

	delete[] pInputDesc;
	return hr;
}
