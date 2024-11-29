/* ========================================
	DX22Base/
	------------------------------------
	���b�V���o�b�t�@�pcpp
	------------------------------------
	MeshBuffer.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "MeshBuffer.h"
#include <memory>

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
		�@���b�V���o�b�t�@������������
	-------------------------------------
	����1�Fconst T_MeshData& desc
=========================================== */
MeshBuffer::MeshBuffer(const T_MeshData& desc)
	: m_pVtxBuffer(NULL)
	, m_pIdxBuffer(NULL)
	, m_tMeshDesc{}
{
	HRESULT hr = E_FAIL;

	// ���_�o�b�t�@�̍쐬
	hr = CreateVertexBuffer(desc.pVtx, desc.vtxSize, desc.vtxCount, desc.bWrite);
	if (FAILED(hr))
	{
		OutputDebugStringA("CreateVertexBuffer �G���[: ���_�o�b�t�@�̍쐬�Ɏ��s���܂����B\n");
		return;
	}

	// �C���f�b�N�X�f�[�^�����݂���ꍇ�̓C���f�b�N�X�o�b�t�@���쐬
	if (desc.pIdx)
	{
		// �C���f�b�N�X�o�b�t�@�̍쐬
		hr = CreateIndexBuffer(desc.pIdx, desc.idxSize, desc.idxCount);
		if (FAILED(hr))
		{
			OutputDebugStringA("CreateIndexBuffer �G���[: �C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂����B\n");
			return;
		}
	}

	// ���b�V���f�[�^����������o�ɃR�s�[
	m_tMeshDesc = desc;


	// ���O������n���ꂽ�f�[�^����ŕύX����Ă��A�����ŕێ�����f�[�^���e�����󂯂Ȃ��悤�ɂ��邽��

	// ���_�f�[�^�̃��������R�s�[
	rsize_t vtxMemSize		= desc.vtxSize * desc.vtxCount;		// ���_�f�[�^�̃������T�C�Y
	void* pVtx				= new char[vtxMemSize];				// ���_�f�[�^�̃������m��
	memcpy_s(pVtx, vtxMemSize, desc.pVtx, vtxMemSize);			// ���_�f�[�^�̃������R�s�[
	m_tMeshDesc.pVtx		= pVtx;	

	// �C���f�b�N�X�f�[�^�̃��������R�s�[
	rsize_t idxMemSize		= desc.idxSize * desc.idxCount;		// �C���f�b�N�X�f�[�^�̃������T�C�Y
	void* pIdx				= new char[idxMemSize];				// �C���f�b�N�X�f�[�^�̃������m��
	memcpy_s(pIdx, idxMemSize, desc.pIdx, idxMemSize);			// �C���f�b�N�X�f�[�^�̃������R�s�[
	m_tMeshDesc.pIdx		= pIdx;	


}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F���
=========================================== */
MeshBuffer::~MeshBuffer()
{
	// ���I�Ɋm�ۂ��ꂽ���_�E�C���f�b�N�X�f�[�^�̃����������
	delete[] m_tMeshDesc.pIdx;
	delete[] m_tMeshDesc.pVtx;

	// DirectX�̃o�b�t�@�����
	SAFE_RELEASE(m_pIdxBuffer);
	SAFE_RELEASE(m_pVtxBuffer);
}


/* ========================================
	�R�s�[�R���X�g���N�^
	-------------------------------------
	���e�F�R�s�[�R���X�g���N�^
		�@���A�j�����f���̃f�B�[�v�R�s�[���p
=========================================== */
MeshBuffer::MeshBuffer(const MeshBuffer& other)
	: m_pVtxBuffer(NULL)
	, m_pIdxBuffer(NULL)
	, m_tMeshDesc{}
{
	// ���g�Ɠ����f�[�^�̓X���[
	if (this == &other) return;
	
	// ���_�o�b�t�@
	this->CreateVertexBuffer(other.m_tMeshDesc.pVtx, other.m_tMeshDesc.vtxSize, other.m_tMeshDesc.vtxCount, other.m_tMeshDesc.bWrite);

	// �C���f�b�N�X�o�b�t�@������ꍇ�̓R�s�[
	if (other.m_tMeshDesc.pIdx && other.m_tMeshDesc.idxSize > 0)
		this->CreateIndexBuffer(other.m_tMeshDesc.pIdx, other.m_tMeshDesc.idxSize, other.m_tMeshDesc.idxCount);	// �C���f�b�N�X�o�b�t�@
	

	// ���b�V���f�[�^�̃R�s�[ -------------------------------------
	this->m_tMeshDesc = other.m_tMeshDesc;
	
	// ���_�f�[�^�̃��������R�s�[
	rsize_t vtxMemSize = other.m_tMeshDesc.vtxSize * other.m_tMeshDesc.vtxCount;// ���_�f�[�^�̃������T�C�Y
	void* pVtx = new char[vtxMemSize];											// ���_�f�[�^�̃������m��
	memcpy_s(pVtx, vtxMemSize, other.m_tMeshDesc.pVtx, vtxMemSize);				// ���_�f�[�^�̃������R�s�[
	this->m_tMeshDesc.pVtx = pVtx;
	
	// �C���f�b�N�X�f�[�^�̃��������R�s�[
	rsize_t idxMemSize = other.m_tMeshDesc.idxSize * other.m_tMeshDesc.idxCount;// �C���f�b�N�X�f�[�^�̃������T�C�Y
	void* pIdx = new char[idxMemSize];											// �C���f�b�N�X�f�[�^�̃������m��
	memcpy_s(pIdx, idxMemSize, other.m_tMeshDesc.pIdx, idxMemSize);				// �C���f�b�N�X�f�[�^�̃������R�s�[
	this->m_tMeshDesc.pIdx = pIdx;

}

/* ========================================
	���_�o�b�t�@�쐬�֐�
	-------------------------------------
	���e�F���_�o�b�t�@���쐬����
	-------------------------------------
	����1�F���_�f�[�^
	����2�F���_�̃T�C�Y
	����3�F���_��
	����4�F���I�Ȓ��_�f�[�^�̏ꍇ��true(���_�����I�ɓ����I�u�W�F�N�g��)
	-------------------------------------
	�߂�l�FHRESULT
=========================================== */
HRESULT MeshBuffer::CreateVertexBuffer(const void* pVtx, UINT size, UINT count, bool isWrite)
{
	// �o�b�t�@�̏���ݒ�
	D3D11_BUFFER_DESC bufDesc;
	ZeroMemory(&bufDesc, sizeof(bufDesc));
	bufDesc.ByteWidth			= size * count;				// �o�b�t�@�̃T�C�Y (���_1�̃T�C�Y �~ ���_��)
	bufDesc.Usage				= D3D11_USAGE_DEFAULT;		// �f�t�H���g�g�p
	bufDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��Ďg�p
	bufDesc.MiscFlags			= 0;						// ���̑��̃t���O
	bufDesc.StructureByteStride = 0;						// �\�����o�b�t�@�̏ꍇ�̃T�C�Y


	// �������݉\�ȃo�b�t�@�̏ꍇ�́A���I�o�b�t�@�Ƃ��Đݒ�
	if (isWrite)
	{
		bufDesc.Usage			= D3D11_USAGE_DYNAMIC;		// ���I�o�b�t�@�ɐݒ�
		bufDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;	// CPU����̏������݂�����
	}

	// �����f�[�^�̐ݒ�
	D3D11_SUBRESOURCE_DATA subResource;
	ZeroMemory(&subResource, sizeof(subResource));
	subResource.pSysMem				= pVtx;	// ���_�f�[�^
	subResource.SysMemPitch			= 0;	// ��������1�s�̃T�C�Y
	subResource.SysMemSlicePitch	= 0;	// ��������1�X���C�X�̃T�C�Y

	// ���_�o�b�t�@���쐬
	HRESULT hr = DirectXManager::GetDevice()->CreateBuffer(&bufDesc, &subResource, &m_pVtxBuffer);

	return hr;
}

/* ========================================
	�C���f�b�N�X�o�b�t�@�쐬�֐�
	-------------------------------------
	���e�F�C���f�b�N�X�o�b�t�@���쐬����
		�@(�C���f�b�N�X�Ƃ́A���_�̔ԍ����w�肵�ĕ`�悷�邽�߂̃f�[�^)
	-------------------------------------
	����1�F�C���f�b�N�X�f�[�^
	����2�F�C���f�b�N�X�̃T�C�Y
	����3�F�C���f�b�N�X��
	-------------------------------------
	�߂�l�FHRESULT
=========================================== */
HRESULT MeshBuffer::CreateIndexBuffer(const void* pIdx, UINT size, UINT count)
{
	// �C���f�b�N�X�T�C�Y��2��4�����m�F
	//(�C���f�b�N�X�̃T�C�Y�͈�ʓI�� 16 �r�b�g�iUINT16�j�܂��� 32 �r�b�g�iUINT32�j���g�p�����)
	if (size != 2 && size != 4) {
		OutputDebugStringA("CreateIndexBuffer �G���[: �C���f�b�N�X �T�C�Y�������ł��B2 �o�C�g�܂��� 4 �o�C�g�ɂ���K�v������܂��B\n");
		return E_FAIL; // �G���[��Ԃ�
	}

	// �o�b�t�@���̐ݒ�
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth			= size * count;				// �C���f�b�N�X�̃T�C�Y�~��
	bufDesc.Usage				= D3D11_USAGE_DEFAULT;		// �f�t�H���g�g�p
	bufDesc.BindFlags			= D3D11_BIND_INDEX_BUFFER;	// �C���f�b�N�X�o�b�t�@�Ƃ��Ďg�p
	bufDesc.CPUAccessFlags		= 0;						// CPU�A�N�Z�X�Ȃ�
	bufDesc.MiscFlags			= 0;						// ���̑��̃t���O
	bufDesc.StructureByteStride = 0;						// �\�����o�b�t�@�̏ꍇ�̃T�C�Y

	// �����f�[�^�̐ݒ�
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem				= pIdx;	// �C���f�b�N�X�f�[�^
	subResource.SysMemPitch			= 0;	// ��������1�s�̃T�C�Y
	subResource.SysMemSlicePitch	= 0;	// ��������1�X���C�X�̃T�C�Y

	// �C���f�b�N�X�o�b�t�@���쐬
	HRESULT hr;
	hr = DirectXManager::GetDevice()->CreateBuffer(&bufDesc, &subResource, &m_pIdxBuffer);

	return hr;
}


/* ========================================
	�`��֐�
	-------------------------------------
	���e�F���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���g���ĕ`��
	-------------------------------------
	����1�Fcount	�`�悷�钸�_��(0�̏ꍇ�͑S�Ă̒��_��`��)
=========================================== */
void MeshBuffer::Draw(int count)
{
	// �f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* pContext = DirectXManager::GetContext();	

	UINT stride = m_tMeshDesc.vtxSize;  // ���_1���̃T�C�Y
	UINT offset = 0;					// �I�t�Z�b�g(���_�o�b�t�@���ł̊J�n�ʒu)

	// �g�|���W�[(�v���~�e�B�u�̎��)��ݒ�
	pContext->IASetPrimitiveTopology(m_tMeshDesc.topology);
	// ���_�o�b�t�@���Z�b�g
	pContext->IASetVertexBuffers(0, 1, &m_pVtxBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@������ꍇ�A�C���f�b�N�X�t���`��
	if (m_tMeshDesc.idxCount > 0)
	{
		DXGI_FORMAT format;
		ZeroMemory(&format, sizeof(format));
		// �C���f�b�N�X�T�C�Y�ɂ���ăt�H�[�}�b�g��ݒ�
		switch (m_tMeshDesc.idxSize)
		{
		case 4: format = DXGI_FORMAT_R32_UINT; break;
		case 2: format = DXGI_FORMAT_R16_UINT; break;
		}
		// �C���f�b�N�X�o�b�t�@���Z�b�g
		pContext->IASetIndexBuffer(m_pIdxBuffer, format, 0);
		pContext->DrawIndexed(count ? count : m_tMeshDesc.idxCount, 0, 0);  // �w�肳�ꂽ�����A�S�ẴC���f�b�N�X��`��
	}
	else
	{
		// �C���f�b�N�X�o�b�t�@���Ȃ��ꍇ�A���_�݂̂ŕ`��
		pContext->Draw(count ? count : m_tMeshDesc.vtxCount, 0);
	}

}

/* ========================================
	�V�K���_�f�[�^�����֐�
	-------------------------------------
	���e�F���_�f�[�^�̏�������
	-------------------------------------
	����1�F���_�o�b�t�@�̃|�C���^
	-------------------------------------
	�߂�l�FHRESULT
=========================================== */
HRESULT MeshBuffer::WriteVertexData(void* pVtx)
{
	// �|�C���^�������ȏꍇ�̓G���[��Ԃ�
	if (!pVtx) { return E_POINTER; }

	// ���I�o�b�t�@�łȂ��ꍇ�̓G���[��Ԃ�
	if (!m_tMeshDesc.bWrite) { return E_FAIL; }

	ID3D11DeviceContext* pContext = DirectXManager::GetContext();	// �f�o�C�X�R���e�L�X�g���擾
	D3D11_MAPPED_SUBRESOURCE mapResource;							// �}�b�v���\�[�X(���_�f�[�^���i�[�����)

	// ���_�o�b�t�@�Ƀf�[�^���}�b�s���O���ď�������
	HRESULT hr = pContext->Map(m_pVtxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	if (FAILED(hr)) {
		OutputDebugStringA("���_�o�b�t�@�̃}�b�s���O�Ɏ��s���܂���.\n");
		return hr;
	}

	// �V�������_�f�[�^���R�s�[
	rsize_t size = m_tMeshDesc.vtxCount * m_tMeshDesc.vtxSize;	// ���_�f�[�^�̃T�C�Y
	memcpy_s(mapResource.pData, size, pVtx, size);				// ���_�f�[�^�̃R�s�[(pVtx��mapResource.pData)

	pContext->Unmap(m_pVtxBuffer, 0);	// �}�b�v����

	return S_OK;
}

/* ========================================
	���b�V���f�[�^�擾�֐�
	-------------------------------------
	���e�F���b�V���f�[�^�̎擾
	-------------------------------------
	�߂�l�FT_MeshData
=========================================== */
MeshBuffer::T_MeshData MeshBuffer::GetDesc()
{
	return m_tMeshDesc;
}
