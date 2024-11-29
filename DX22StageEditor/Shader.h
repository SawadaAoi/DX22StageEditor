/* ========================================
	DX22Base/
	------------------------------------
	�V�F�[�_�[�p�w�b�_
	------------------------------------
	����:�V�F�[�_�[�̊�{�N���X
	------------------------------------
	Shader.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "DirectXManager.h"
#include "Texture.h"
#include <string>
#include <map>
#include <vector>


// =============== �N���X��` =====================
class Shader
{
protected:
	// �V�F�[�_�[�̎��
	enum Kind
	{
		Vertex,	// ���_�V�F�[�_
		Pixel,	// �s�N�Z���V�F�[�_
	};
public:
	Shader(Kind kind);

	virtual ~Shader();
	HRESULT Load(const char* pFileName);	// �V�F�[�_�[�t�@�C��(*.cso)��ǂݍ��ޏ���
	HRESULT Compile(const char* pCode);		// �����񂩂�V�F�[�_���R���p�C��

	void WriteBuffer(UINT slot, const void* pData);	// �萔�̏�������
	void SetTexture(UINT slot, const Texture* tex);	// �e�N�X�`���̐ݒ�
	virtual void Bind(void) = 0;				// �V�F�[�_�[��`��Ɏg�p


private:
	HRESULT Make(void* pData, UINT size);

	Kind m_kind;
protected:
	// �V�F�[�_�[�t�@�C����ǂݍ��񂾌�A�V�F�[�_�[�̎�ޕʂɏ������s��
	virtual HRESULT MakeShader(void* pData, UINT size) = 0;

	std::vector<ID3D11Buffer*>				m_pBuffers;		// �萔�o�b�t�@
	std::vector<ID3D11ShaderResourceView*>	m_pTextures;	// �e�N�X�`�����\�[�X
};




