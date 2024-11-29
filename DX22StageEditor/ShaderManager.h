/* ========================================
	DX22Base/
	------------------------------------
	�V�F�[�_�[�Ǘ��p�w�b�_
	------------------------------------
	�����F�V�F�[�_�[���܂Ƃ߂ēǂݍ���ŊǗ�����
	------------------------------------
	ShaderManagaer.h
========================================== */

// =============== �C���N���[�h ===================
#include "VertexShader.h"
#include "PixelShader.h"
#include <vector>
#include <unordered_map>
#include <memory>


// =============== �萔��` =======================
#define VS_KEY CShaderManagaer::E_VS_KEY	// �L�q�ȗ��̂���
#define PS_KEY CShaderManagaer::E_PS_KEY	// �L�q�ȗ��̂���

#define SHADER_INST CShaderManagaer::GetInstance()			// �L�q�ȗ��̂���
#define GET_VS_DATA CShaderManagaer::GetInstance().GetVs	// ���͏ȗ�
#define GET_PS_DATA CShaderManagaer::GetInstance().GetPs	// ���͏ȗ�

class CShaderManagaer
{
public:
	// ���_�V�F�[�_�[���
	enum E_VS_KEY
	{
		VS_SHAPE,				// �}�`�`��p
		VS_LINE,				// ���C���`��p
		VS_SPRITE,				// �X�v���C�g�`��p
		VS_MODEL_STATIC,	// �ÓI���f���p�f�t�H���g
		VS_MODEL_ANIME,		// �A�j���[�V�������f���p�f�t�H���g
	};

	// �s�N�Z���V�F�[�_�[���
	enum E_PS_KEY
	{
		PS_SHAPE,				// �}�`�`��p
		PS_LINE,				// ���C���`��p
		PS_SPRITE,				// �X�v���C�g�`��p
		PS_MODEL_STATIC,	// �ÓI���f���p�f�t�H���g
		PS_MODEL_ANIME,		// �A�j���[�V�������f���p�f�t�H���g
	};

public:
	static CShaderManagaer& GetInstance();	// �C���X�^���X�擾
	void Init();	// ������

	// �V�F�[�_�[�ǂݍ���
	VertexShader* GetVs(CShaderManagaer::E_VS_KEY e_GetKey);
	PixelShader* GetPs(CShaderManagaer::E_PS_KEY e_GetKey);

private:
	CShaderManagaer() {};	// �R���X�g���N�^

	// �V�F�[�_�[�f�[�^���i�[���Ă���
	std::unordered_map <E_VS_KEY, std::shared_ptr<VertexShader>> m_pVsMap;
	std::unordered_map <E_PS_KEY, std::shared_ptr<PixelShader>>	m_pPsMap;



};

