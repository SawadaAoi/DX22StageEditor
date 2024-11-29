/* ========================================
	DX22Base/
	------------------------------------
	�V�F�[�_�[�pcpp
	------------------------------------
	Shader.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "Shader.h"
#include <d3dcompiler.h>	// �V�F�[�_�[�̃R���p�C��
#include <stdio.h>			// �t�@�C���ǂݍ���

// =============== �����N =======================
#pragma comment(lib, "d3dcompiler.lib")	// �V�F�[�_�[�̃R���p�C��


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
=========================================== */
Shader::Shader(Kind kind)
	: m_kind(kind)
{
}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F�������
=========================================== */
Shader::~Shader()
{
	std::vector<ID3D11Buffer*>::iterator it = m_pBuffers.begin();
	while (it != m_pBuffers.end())
	{
		SAFE_RELEASE((*it));
		++it;
	}
}

/* ========================================
	�V�F�[�_�[�t�@�C��(*.cso)�Ǎ��֐�
	-------------------------------------
	���e�F
	-------------------------------------
	�����FpFileName	�t�@�C����
	-------------------------------------
	�߂�l�FHRESULT	����
=========================================== */
HRESULT Shader::Load(const char* pFileName)
{
	HRESULT hr = E_FAIL;

	// �t�@�C����ǂݍ���
	FILE* fp;
	fopen_s(&fp, pFileName, "rb");	// �o�C�i�����[�h�œǂݍ���
	if (!fp) { return hr; }			// �t�@�C�����J���Ȃ�������I��

	// �t�@�C���̃T�C�Y�𒲂ׂ�
	int fileSize = 0;
	fseek(fp, 0, SEEK_END);		// �t�@�C���̍Ō�܂ňړ�
	fileSize = ftell(fp);		// �t�@�C���̈ʒu���擾(=�t�@�C���T�C�Y)

	// �������ɓǂݍ���
	fseek(fp, 0, SEEK_SET);				// �t�@�C���̐擪�Ɉړ�
	char* pData = new char[fileSize];	// �t�@�C���T�C�Y���̃��������m��
	fread(pData, fileSize, 1, fp);		// �t�@�C����ǂݍ���
	fclose(fp);							// �t�@�C�������

	// �V�F�[�_�[�쐬
	hr = Make(pData, fileSize);
	
	// �I������
	if (pData) { delete[] pData; }
	return hr;
}

/* ========================================
	�V�F�[�_�[�R���p�C���֐�
	-------------------------------------
	���e�F
	-------------------------------------
	�����FpCode	�R�[�h
	-------------------------------------
	�߂�l�FHRESULT	����
=========================================== */
HRESULT Shader::Compile(const char *pCode)
{
    // �^�[�Q�b�g���X�g
    static const char *pTargetList[] = 
    {
        "vs_5_0",  // ���_�V�F�[�_�[�̃R���p�C���^�[�Q�b�g
        "ps_5_0"   // �s�N�Z���V�F�[�_�[�̃R���p�C���^�[�Q�b�g
    };

    HRESULT hr;
    ID3DBlob *pBlob = nullptr;  // �R���p�C�����ꂽ�V�F�[�_�[�R�[�h���i�[
    ID3DBlob *error = nullptr;  // �G���[���b�Z�[�W���i�[

    UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;  // �f�o�b�O�t���O���g�p(�f�o�b�O���/�œK�����X�L�b�v)

    hr = D3DCompile(pCode, strlen(pCode), nullptr, nullptr, nullptr,
        "main", pTargetList[m_kind], compileFlag, 0, &pBlob, &error);
    
    // �R���p�C�������s�����ꍇ�A�G���[�n���h�����O
    if (FAILED(hr)) { return hr; }

    // �V�F�[�_�쐬
    hr = Make(pBlob->GetBufferPointer(), (UINT)pBlob->GetBufferSize());

    // ���\�[�X�����
    SAFE_RELEASE(pBlob);
    SAFE_RELEASE(error);
    
    return hr;
}


/* ========================================
	�萔�o�b�t�@�����֐�
	-------------------------------------
	���e�F�V�F�[�_�[�ɓn���f�[�^��
		�@�萔�o�b�t�@�ɏ������݁AGPU �ɑ���
	-------------------------------------
	����1�Fslot		�o�b�t�@�ԍ�
	����2�FpData	�������ރf�[�^
=========================================== */
void Shader::WriteBuffer(UINT slot, const void* pData)
{
	// �o�b�t�@�̃T�C�Y�𒴂��Ă��Ȃ����`�F�b�N
	if (slot < m_pBuffers.size())
	{
		DirectXManager::GetContext()->UpdateSubresource(m_pBuffers[slot], 0, nullptr, pData, 0, 0);
	}
}

/* ========================================
	�e�N�X�`���ݒ�֐�
	-------------------------------------
	���e�F�e�N�X�`����ݒ�
	-------------------------------------
	����1�Fslot		�e�N�X�`����ݒ肷��V�F�[�_�[�X���b�g�ԍ�
	����2�Ftex		�e�N�X�`��
=========================================== */
void Shader::SetTexture(UINT slot, const Texture* tex)
{
	// �e�N�X�`�����Ȃ����A�X���b�g�ԍ����s���Ȃ�I��
	if (!tex || slot >= m_pTextures.size()) { return; }
	
	// �e�N�X�`texture�̃��\�[�X���擾(=GPU ���e�N�X�`�����V�F�[�_�[�Ŏg�p�ł���悤�ɂ��邽��)
	ID3D11ShaderResourceView* pTex = tex->GetResource();

	m_pTextures[slot] = pTex;	// �e�N�X�`����ݒ�

	// �V�F�[�_�[�Ƀe�N�X�`����ݒ�
	switch (m_kind)
	{
	case Vertex:	DirectXManager::GetContext()->VSSetShaderResources(slot, 1, &pTex); break;
	case Pixel:		DirectXManager::GetContext()->PSSetShaderResources(slot, 1, &pTex); break;
	}
}

/* ========================================
	�V�F�[�_�[�쐬�֐�
	-------------------------------------
	���e�F�V�F�[�_�[�t�@�C���̃f�[�^���󂯎���āA
		�@���̒��g����͂��A�V�F�[�_�[�ɕK�v�ȃ��\�[�X
		 �i�萔�o�b�t�@��e�N�X�`�����\�[�X�j������
	-------------------------------------
	����1�FpData		�f�[�^
	����2�Fsize		�T�C�Y
	-------------------------------------
	�߂�l�FHRESULT	����
=========================================== */
HRESULT Shader::Make(void* pData, UINT size)
{
	HRESULT hr;
	ID3D11Device* pDevice = DirectXManager::GetDevice();

	// ��͗p�̃��t���N�V�����쐬
	ID3D11ShaderReflection* pReflection;
	hr = D3DReflect(pData, size, IID_PPV_ARGS(&pReflection));
	if (FAILED(hr)) { return hr; }

	// �萔�o�b�t�@�쐬
	D3D11_SHADER_DESC shaderDesc;
	pReflection->GetDesc(&shaderDesc);	// �V�F�[�_�[�̊�{�����擾

	m_pBuffers.resize(shaderDesc.ConstantBuffers, nullptr);	// �萔�o�b�t�@�̐������o�b�t�@���쐬

	// �萔�o�b�t�@�̍쐬
	for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
	{
		// �萔�o�b�t�@�̏��擾
		D3D11_SHADER_BUFFER_DESC shaderBufDesc;														
		ID3D11ShaderReflectionConstantBuffer* cbuf = pReflection->GetConstantBufferByIndex(i);
		cbuf->GetDesc(&shaderBufDesc);

		// �쐬����o�b�t�@�̏��
		D3D11_BUFFER_DESC bufDesc;
		ZeroMemory(&bufDesc, sizeof(bufDesc));
		bufDesc.ByteWidth	= shaderBufDesc.Size;			// �o�b�t�@�̃T�C�Y
		bufDesc.Usage		= D3D11_USAGE_DEFAULT;		// �o�b�t�@�̎g�p���@
		bufDesc.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;	// �o�b�t�@�̃o�C���h�t���O

		// �o�b�t�@�̍쐬
		hr = pDevice->CreateBuffer(&bufDesc, nullptr, &m_pBuffers[i]);
		if (FAILED(hr)) { return hr; }
	}

	// �e�N�X�`���̈�쐬
	m_pTextures.resize(shaderDesc.TextureNormalInstructions, nullptr);

	return MakeShader(pData, size);	// �V�F�[�_�[�쐬
}


