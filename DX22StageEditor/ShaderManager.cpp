/* ========================================
	DX22Base/
	------------------------------------
	�V�F�[�_�[�Ǘ��pcpp
	------------------------------------
	ShaderManagaer.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ShaderManager.h"


// =============== �萔��` =======================
// ���_�V�F�[�_�[�ۑ���p�X
const std::unordered_map < CShaderManagaer::E_VS_KEY, std::string > VS_SHADER_PATH
{
	{VS_KEY::VS_SHAPE,			"Assets/shader/VS_Shape.cso"		},
	{VS_KEY::VS_LINE,			"Assets/shader/VS_Line.cso"			},
	{VS_KEY::VS_SPRITE,			"Assets/shader/VS_Sprite.cso"		},
	{VS_KEY::VS_MODEL_STATIC,	"Assets/shader/VS_ModelStatic.cso"	},
	{VS_KEY::VS_MODEL_ANIME,	"Assets/shader/VS_ModelAnime.cso"	},
};

// �s�N�Z���V�F�[�_�[�ۑ���p�X
const std::unordered_map < CShaderManagaer::E_PS_KEY, std::string > PS_SHADER_PATH
{
	{PS_KEY::PS_SHAPE,			"Assets/shader/PS_Shape.cso"		},
	{PS_KEY::PS_LINE,			"Assets/shader/PS_Line.cso"			},
	{PS_KEY::PS_SPRITE,			"Assets/shader/PS_Sprite.cso"		},
	{PS_KEY::PS_MODEL_STATIC,	"Assets/shader/PS_ModelStatic.cso"	},
	{PS_KEY::PS_MODEL_ANIME,	"Assets/shader/PS_ModelAnime.cso"	},
};


/* ========================================
	�C���X�^���X�擾�֐�
	-------------------------------------
	���e�F�C���X�^���X���擾����
	-------------------------------------
	�ߒl�F�C���X�^���X
=========================================== */
CShaderManagaer& CShaderManagaer::GetInstance()
{
	static CShaderManagaer instance;
	return instance;
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F�V�F�[�_�[��ǂݍ���
=========================================== */
void CShaderManagaer::Init()
{
	// �V�F�[�_�[�̎�ޕ�
	for (std::pair<E_VS_KEY, std::string> PathMap : VS_SHADER_PATH)
	{
		E_VS_KEY eNowKey = PathMap.first;	// ���݂̃L�[
		std::string sNowPath = PathMap.second;	// ���݂̕ۑ��ʒu�p�X

		// ���_�V�F�[�_�̓Ǎ�
		VertexShader* pVs = new VertexShader;
		if (FAILED(pVs->Load(sNowPath.c_str()))) {
			MessageBox(nullptr, sNowPath.c_str(), "Error", MB_OK);
		}

		m_pVsMap.insert(std::make_pair(eNowKey, pVs));	// ���f���f�[�^�z��ɒǉ�
	}

	// �V�F�[�_�[�̎�ޕ�
	for (std::pair<E_PS_KEY, std::string> PathMap : PS_SHADER_PATH)
	{
		E_PS_KEY eNowKey = PathMap.first;	// ���݂̃L�[
		std::string sNowPath = PathMap.second;	// ���݂̕ۑ��ʒu�p�X

		// �s�N�Z���V�F�[�_�[�̓Ǎ�
		PixelShader* pPs = new PixelShader;
		if (FAILED(pPs->Load(sNowPath.c_str()))) {
			MessageBox(nullptr, sNowPath.c_str(), "Error", MB_OK);
		}

		m_pPsMap.insert(std::make_pair(eNowKey, pPs));	// ���f���f�[�^�z��ɒǉ�
	}
}

/* ========================================
	���_�V�F�[�_�[�擾�֐�
	-------------------------------------
	���e�F�L�[�����ɊY���̃V�F�[�_�[��n��
	-------------------------------------
	����1�F�L�[
	-------------------------------------
	�ߒl�F�V�F�[�_�[�|�C���^
=========================================== */
VertexShader* CShaderManagaer::GetVs(CShaderManagaer::E_VS_KEY e_GetKey)
{
	return m_pVsMap.at(e_GetKey).get();
}

/* ========================================
	���_�V�F�[�_�[�擾�֐�
	-------------------------------------
	���e�F�L�[�����ɊY���̃V�F�[�_�[��n��
	-------------------------------------
	����1�F�L�[
	-------------------------------------
	�ߒl�F�V�F�[�_�[�|�C���^
=========================================== */
PixelShader* CShaderManagaer::GetPs(CShaderManagaer::E_PS_KEY e_GetKey)
{
	return m_pPsMap.at(e_GetKey).get();
}
