/* ========================================
	DX22Base/
	------------------------------------
	�}�`�`��pcpp
	------------------------------------
	Shape.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ShapeBase.h"
#include "ShaderManager.h"
#include "CameraManager.h"
#include "DirectXManager.h"
#include "TextureManager.h"	
#include "LightManager.h"
#include "ShadowManager.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F����
=========================================== */
ShapeBase::ShapeBase()
	: m_pMeshBuffer{ nullptr, nullptr }
	, m_pVS(nullptr)
	, m_pPS(nullptr)
	, m_WVP()
	, m_vPosition(0.0f, 0.0f, 0.0f)
	, m_vScale(1.0f, 1.0f, 1.0f)
	, m_qRotation({0.0f,0.0f,0.0f}, 1.0f)
	, m_fColor(1.0f, 1.0f, 1.0f)
	, m_eDrawMode(E_DrawMode::DRAW_NORMAL)
	, m_pTextures()
	, m_bIsTex(false)
	, m_bIsCulling(true)
	, m_fUvScale()
	, m_fUvOffset()
{
	// ���_�V�F�[�_�[�ǂݍ���
	m_pVS = GET_VS_DATA(VS_KEY::VS_SHAPE);
	// �s�N�Z���V�F�[�_�[�ǂݍ���
	m_pPS = GET_PS_DATA(PS_KEY::PS_SHAPE);

	m_pTextures.resize(1);
	m_fUvScale.resize(1);
	m_fUvOffset.resize(1);
}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F�������
=========================================== */
ShapeBase::~ShapeBase()
{
	SAFE_DELETE(m_pMeshBuffer[E_DrawMode::DRAW_NORMAL]);
	SAFE_DELETE(m_pMeshBuffer[E_DrawMode::DRAW_WIRE_FRAME]);
}


/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
=========================================== */
void ShapeBase::Draw()
{
	DirectXManager::OnOffDepthStencilView(true);	// �[�x�o�b�t�@�̗L����

	SetWVPMatrix();	// WVP�s��̐ݒ�

	// �萔�o�b�t�@(���[���h�A�r���[�A�v���W�F�N�V�����s��)�̏�������
	m_pVS->WriteBuffer(0, m_WVP);

	// Uv�X�P�[���A�I�t�Z�b�g�̐ݒ�
	DirectX::XMFLOAT4 fUvBuf = { m_fUvScale[0].x, m_fUvScale[0].y, m_fUvOffset[0].x, m_fUvOffset[0].y};
	m_pVS->WriteBuffer(1, &fUvBuf);


	// �萔�o�b�t�@(�F�A�\�����[�h�A�e�N�X�`���g�p�t���O)�̏�������
	float	fData[3] = { m_fColor.x, m_fColor.y, m_fColor.z };	// �F
	int		nData[2] = { m_eDrawMode, m_bIsTex };				// �\�����[�h�A�e�N�X�`���g�p�t���O
	m_pPS->WriteBuffer(0, fData);
	m_pPS->WriteBuffer(1, nData);
	m_pPS->SetTexture(0, m_pTextures[0]);	// �e�N�X�`���̐ݒ�

	// �V�F�[�_�[�̃o�C���h(�V�F�[�_�[�̐ݒ�� GPU �ɑ���)
	m_pVS->Bind();
	m_pPS->Bind();
	
	if (m_bIsCulling)
		DirectXManager::SetCullingMode(DirectXManager::CullMode::CULL_BACK);	// �J�����O�L��
	else
		DirectXManager::SetCullingMode(DirectXManager::CullMode::CULL_NONE);	// �J�����O����

	// ���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@��GPU�ɐݒ肵�A�`��
	m_pMeshBuffer[m_eDrawMode]->Draw();	
	// �J�����O�����ɖ߂�
	DirectXManager::SetCullingMode(DirectXManager::CullMode::CULL_BACK);	// �J�����O�L��
}


/* ========================================
	WVP�s��ݒ�֐�
	-------------------------------------
	���e�F���[���h�A�r���[�A�v���W�F�N�V�����s���ݒ肷��
=========================================== */
void ShapeBase::SetWVPMatrix()
{
	// ���[���h�s��̌v�Z
	DirectX::XMStoreFloat4x4(&m_WVP[0],												// Float4x4�ɕϊ����Ċi�[
		DirectX::XMMatrixTranspose(													// �]�n
			DirectX::XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z) *			// �g��k��
			m_qRotation.ToDirectXMatrix() *											// �N�H�[�^�j�I����]
			DirectX::XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z)				// �ړ�
		));

	m_WVP[1] = CAMERA_MNG_INST.GetActiveCamera()->GetViewMatrix();
	m_WVP[2] = CAMERA_MNG_INST.GetActiveCamera()->GetProjectionMatrix();

}


/* ========================================
	�Q�b�^�[(���W)�֐�
	-------------------------------------
	�ߒl�FVector3<float> fPos
=========================================== */
Vector3<float> ShapeBase::GetPosition()
{
	return m_vPosition;
}

/* ========================================
	�Q�b�^�[(�T�C�Y)�֐�
	-------------------------------------
	�ߒl�FVector3<float> fSize
=========================================== */
Vector3<float> ShapeBase::GetScale()
{
	return m_vScale;
}

/* ========================================
	�Q�b�^�[(��])�֐�
	-------------------------------------
	�ߒl�FQuaternion m_fRotation
=========================================== */
Quaternion ShapeBase::GetRotation()
{
	return m_qRotation;
}

/* ========================================
	�Q�b�^�[(�\�����[�h)�֐�
	-------------------------------------
	�ߒl�FE_DrawMode eMode
=========================================== */
Vector3<float> ShapeBase::GetColor()
{
	return m_fColor;
}

/* ========================================
	�Q�b�^�[(�\�����[�h)�֐�
	-------------------------------------
	�ߒl�FE_DrawMode eMode
=========================================== */
ShapeBase::E_DrawMode ShapeBase::GetDrawMode()
{
	return m_eDrawMode;
}

/* ========================================
	�Q�b�^�[(�e�N�X�`���g�p�t���O)�֐�
	-------------------------------------
	�ߒl�Fbool m_bIsTex
=========================================== */
bool ShapeBase::GetIsTexture()
{
	return m_bIsTex;
}

/* ========================================
	�Q�b�^�[(�J�����O�ݒ�)�֐�
	-------------------------------------
	�ߒl�Fbool m_bIsCulling
=========================================== */
bool ShapeBase::GetIsCulling()
{
	return m_bIsCulling;
}

/* ========================================
	�Q�b�^�[(Uv�X�P�[��)�֐�
	-------------------------------------
	�����Fint �z��ԍ�
	-------------------------------------
	�ߒl�FVector2<float> m_vUvScale
=========================================== */
Vector2<float> ShapeBase::GetUvScale(int nIndex)
{
	// �C���f�b�N�X���e�N�X�`���z��̃T�C�Y�𒴂��Ă�����0�ɃZ�b�g
	if (nIndex >= m_pTextures.size() || nIndex < 0)
	{
		nIndex = 0;
	}

	return m_fUvScale.at(nIndex);
}

/* ========================================
	�Q�b�^�[(Uv�X�P�[��)�֐�
	-------------------------------------
	�����Fint �z��ԍ�
	-------------------------------------
	�ߒl�FVector2<float> m_vUvScale
=========================================== */
Vector2<float> ShapeBase::GetUvOffset(int nIndex)
{
	// �C���f�b�N�X���e�N�X�`���z��̃T�C�Y�𒴂��Ă�����0�ɃZ�b�g
	if (nIndex >= m_pTextures.size() || nIndex < 0)
	{
		nIndex = 0;
	}

	return m_fUvOffset.at(nIndex);
}


/* ========================================
	�Z�b�^�[(���W)�֐�
	-------------------------------------
	�����FVector3<float> fPos
=========================================== */
void ShapeBase::SetPosition(Vector3<float> fPos)
{
	m_vPosition = fPos;
}

/* ========================================
	�Z�b�^�[(�T�C�Y)�֐�
	-------------------------------------
	�����FVector3<float> fSize
=========================================== */
void ShapeBase::SetScale(Vector3<float> fSize)
{
	m_vScale = fSize;
}

/* ========================================
	�Z�b�^�[(��])�֐�
	-------------------------------------
	�����FQuaternion fRot
=========================================== */
void ShapeBase::SetRotation(Quaternion fRot)
{
	m_qRotation = fRot;
}

/* ========================================
	�Z�b�^�[(�F)�֐�
	-------------------------------------
	�����FVector3<float> fColor
=========================================== */
void ShapeBase::SetColor(Vector3<float> fColor)
{
	m_fColor = fColor;
}

/* ========================================
	�Z�b�^�[(�\�����[�h)�֐�
	-------------------------------------
	�����FE_DrawMode eMode
=========================================== */
void ShapeBase::SetDrawMode(E_DrawMode eMode)
{
	m_eDrawMode = eMode;
}

/* ========================================
	�Z�b�^�[(�e�N�X�`��)�֐�
	-------------------------------------
	�����FTexture* pTexture
	�����Fint nIndex
=========================================== */
void ShapeBase::SetTexture(Texture* pTexture, int nIndex)
{
	// �C���f�b�N�X���e�N�X�`���z��̃T�C�Y�𒴂��Ă�����0�ɃZ�b�g
	if (nIndex >= m_pTextures.size() || nIndex < 0)
	{
		nIndex = 0;
	}
		
	m_pTextures[nIndex] = pTexture;
}

/* ========================================
	�Z�b�^�[(�e�N�X�`���g�p�t���O)�֐�
	-------------------------------------
	�����Fint bIsTex
=========================================== */
void ShapeBase::SetUseTexture(int bIsTex)
{
	m_bIsTex = bIsTex;
}


/* ========================================
	�Z�b�^�[(���C�g�ݒ�(�}�e���A��))�֐�
	-------------------------------------
	����1�Ffloat	�g�U��
	����2�Ffloat	���ʔ��ˌ�
	����3�Ffloat	����
	����4�Fbool		���C�g�g�p�t���O
=========================================== */
void ShapeBase::SetLightMaterial(float fDiffuse, float fSpecular, float fAmbient, bool bUseLight)
{
	DirectX::XMFLOAT4 param[] =
	{
		{fDiffuse, fSpecular, fAmbient, bUseLight ? 1.0f : 0.0f}
	};

	m_pPS->WriteBuffer(2, param);
}

/* ========================================
	�Z�b�^�[(�J�������W)�֐�
	-------------------------------------
	����1�FVector3<float>	�J�������W
=========================================== */
void ShapeBase::SetCameraPos(Vector3<float> fCameraPos)
{
	m_pPS->WriteBuffer(3, &fCameraPos);
}

/* ========================================
	�Z�b�^�[(���C�g���)�֐�
	-------------------------------------
	�����Fstd::vector<ObjectLight*>	�V�[����̃��C�g���X�g
=========================================== */
void ShapeBase::SetLights(std::vector<ObjectLight*> lights)
{
	DirectX::XMFLOAT4 param[MAX_LIGHT_NUM][4];

	// ������
	for (int i = 0; i < MAX_LIGHT_NUM; i++)
	{
		param[i][0] = { 0.0f, 0.0f, 0.0f, 0.0f };
		param[i][1] = { 0.0f, 0.0f, 0.0f, 0.0f };
		param[i][2] = { 0.0f, 0.0f, 0.0f, 0.0f };
		param[i][3] = { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	// ���C�g�����Z�b�g
	for (int i = 0; i < lights.size(); ++i)
	{
		float			lightType = static_cast<float>(lights[i]->GetLightType());
		Vector3<float>	lightPos = lights[i]->GetPosition();
		Vector3<float>	lightColor = lights[i]->GetColor();
		Vector3<float>	lightDir = lights[i]->GetDirection();

		param[i][0] = { lightType, lightPos.x, lightPos.y, lightPos.z };			// ���C�g�^�C�v�A���C�g�ʒu
		param[i][1] = { lightColor.x, lightColor.y, lightColor.z, 1.0f };			// ���C�g�J���[
		param[i][2] = { lightDir.x, lightDir.y, lightDir.z, lights[i]->GetRange() };// ���C�g�����A���C�g�L���͈�
		param[i][3] = { lights[i]->GetAngle(), 0.0f, 0.0f, 0.0f };					// �X�|�b�g���C�g�p�x
	}

	// �ۉe�̏����Z�b�g
	int nParamNum = lights.size();
	std::vector<ComponentShadow*> shadows = SHADOW_MNG_INST.GetShadowList();
	for (int i = 0; i < shadows.size(); i++)
	{
		if (nParamNum >= MAX_LIGHT_NUM) break;	// ���C�g�A�e���v�����ő吔�𒴂�����I��

		Vector3<float>	vShadowPos = shadows.at(i)->GetPosition();

		param[nParamNum][0] = { 4.0f, vShadowPos.x, vShadowPos.y, vShadowPos.z };	// ���C�g�^�C�v(�e�͌Œ��4)�A�e�̊���W
		param[nParamNum][1] = { 0.0f, 0.0f, 0.0f, 0.0f };							// ���C�g�J���[(���g�p)
		param[nParamNum][2] = { 0.0f, 0.0f, 0.0f, shadows[i]->GetCircleSize() };	// ���C�g����(���g�p)�A���e�̉~�̑傫��
		param[nParamNum][3] = { 0.0f, 0.0f, 0.0f, 0.0f };							// �X�|�b�g���C�g�p�x(���g�p)

		nParamNum++;
	}

	m_pPS->WriteBuffer(4, param);
}

/* ========================================
	�Z�b�^�[(�J�����O�ݒ�)�֐�
	-------------------------------------
	�����Fbool bCulling
=========================================== */
void ShapeBase::SetIsCulling(bool bCulling)
{
	m_bIsCulling = bCulling;
}

/* ========================================
	�Z�b�^�[(Uv�I�t�Z�b�g)�֐�
	-------------------------------------
	����1�FVector2<float> fOffset
	����2�Fint nIndex
=========================================== */
void ShapeBase::SetUvScale(Vector2<float> fScale, int nIndex)
{
	// �C���f�b�N�X���e�N�X�`���z��̃T�C�Y�𒴂��Ă�����0�ɃZ�b�g
	if (nIndex >= m_pTextures.size() || nIndex < 0)
	{
		nIndex = 0;
	}

	m_fUvScale.at(nIndex) = fScale;
}

/* ========================================
	�Z�b�^�[(Uv�I�t�Z�b�g)�֐�
	-------------------------------------
	����1�FVector2<float> fOffset
	����2�Fint nIndex
=========================================== */
void ShapeBase::SetUvOffset(Vector2<float> fOffset, int nIndex)
{
	// �C���f�b�N�X���e�N�X�`���z��̃T�C�Y�𒴂��Ă�����0�ɃZ�b�g
	if (nIndex >= m_pTextures.size() || nIndex < 0)
	{
		nIndex = 0;
	}

	m_fUvOffset.at(nIndex) = fOffset;
}