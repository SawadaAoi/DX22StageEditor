/* ========================================
	DX22Base/
	------------------------------------
	UI�摜�pcpp
	------------------------------------
	UIComponentSprite.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "UIComponentSprite.h"
#include "UIComponentTransform.h"
#include "UIObjectBase.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "MeshBuffer.h"


/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F�����I�u�W�F�N�g
=========================================== */
UIComponentSprite::UIComponentSprite(UIObjectBase* owner)
	: UIComponentBase(owner, UIOrderSprite)
	, m_pCompTran(nullptr)
	, m_pMesh(nullptr)
	, m_pTexture(nullptr)
	, m_pVS(nullptr)
	, m_pPS(nullptr)
	, m_vOffset(0.0f, 0.0f)
	, m_vUvPos(0.0f, 0.0f)
	, m_vUvScale(1.0f, 1.0f)
	, m_vColor(1.0f, 1.0f, 1.0f)
	, m_fAlpha(1.0f)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
=========================================== */
void UIComponentSprite::Init()
{
	m_pCompTran = m_pOwnerObj->GetComponent<UIComponentTransform>();

	InitMesh();

	m_pVS		= GET_VS_DATA(VS_KEY::VS_SPRITE);
	m_pPS		= GET_PS_DATA(PS_KEY::PS_SPRITE);
	m_pTexture	= GET_TEXTURE_DATA(TEX_KEY::TEST);	// �f�t�H���g�e�N�X�`��
}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
=========================================== */
void UIComponentSprite::Draw()
{
	if (!m_bIsVisible) return;

	DirectX::XMFLOAT4X4 matWVP[3];
	// ���[���h�s��
	matWVP[0] = CreateWorldMatrix();	
	// �r���[�s��
	DirectX::XMStoreFloat4x4(&matWVP[1], DirectX::XMMatrixIdentity());	// UI�̓r���[�s�񂪂Ȃ��̂ŒP�ʍs��
	// �v���W�F�N�V�����s��
	matWVP[2] = CAMERA_MNG_INST.GetActiveCamera()->GetProjectionMatrixUI();

	Vector2<float> vScale = m_pCompTran->GetWorldScale();
	// ���_�V�F�[�_�ɓn���p�����[�^
	DirectX::XMFLOAT4 param[3];
	param[0] = { m_vOffset.x, m_vOffset.y, vScale.x, vScale.y };		// �I�t�Z�b�g�A�X�P�[��
	param[1] = { m_vUvPos.x, m_vUvPos.y, m_vUvScale.x, m_vUvScale.y };	// UV���W�AUV�X�P�[��
	param[2] = { m_vColor.x, m_vColor.y, m_vColor.z, m_fAlpha };		// �F�A�����x

	//���_�V�F�[�_
	m_pVS->WriteBuffer(0, matWVP);
	m_pVS->WriteBuffer(1, param);
	m_pVS->Bind();

	// �s�N�Z���V�F�[�_
	m_pPS->SetTexture(0, m_pTexture);
	m_pPS->Bind();

	// ���b�V���`��
	m_pMesh->Draw();
}


/* ========================================
	���b�V���������֐�
	-------------------------------------
	���e�F���b�V���̏�����
=========================================== */
void UIComponentSprite::InitMesh()
{
	struct T_Vertex
	{
		float pos[3];
		float uv[2];
	};
	T_Vertex vtx[] = {
		{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}},	// ����
		{{ 0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}},	// �E��
		{{-0.5f,-0.5f, 0.0f}, {0.0f, 1.0f}},	// ����
		{{ 0.5f,-0.5f, 0.0f}, {1.0f, 1.0f}},	// �E��
	};

	// ���b�V��
	MeshBuffer::T_MeshData desc = {};
	desc.pVtx		= vtx;
	desc.vtxSize	= sizeof(T_Vertex);
	desc.vtxCount	= _countof(vtx);
	desc.topology	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	m_pMesh			= std::make_shared<MeshBuffer>(desc);
}

/* ========================================
	���[���h�s��쐬�֐�
	-------------------------------------
	���e�F���[���h�s����쐬����
	-------------------------------------
	�߂�l�F���[���h�s��
=========================================== */
DirectX::XMFLOAT4X4 UIComponentSprite::CreateWorldMatrix()
{
	DirectX::XMFLOAT4X4 mat;

	Vector2<float>	vPos	= m_pCompTran->GetWorldPosition();
	float			fRot	= m_pCompTran->GetWorldRotation();

	DirectX::XMStoreFloat4x4(&mat,								// Float4x4�ɕϊ����Ċi�[
		DirectX::XMMatrixTranspose(								// �]�n
			DirectX::XMMatrixRotationZ(fRot) *					// ��]
			DirectX::XMMatrixTranslation(vPos.x, vPos.y, 0.0f)	// �ړ�
		));

	return mat;
}

/* ========================================
	�Q�b�^�[(�e�N�X�`��)�֐�
	-------------------------------------
	�ߒl�F�e�N�X�`��
=========================================== */
Texture* UIComponentSprite::GetTexture()
{
	return m_pTexture;
}

/* ========================================
	�Q�b�^�[(�\���t���O)�֐�
	-------------------------------------
	�ߒl�F�\���t���O
=========================================== */
bool UIComponentSprite::GetIsVisible()
{
	return m_bIsVisible;
}

/* ========================================
	�Q�b�^�[(�I�t�Z�b�g)�֐�
	-------------------------------------
	�ߒl�F�I�t�Z�b�g
=========================================== */
Vector2<float> UIComponentSprite::GetOffset()
{
	return m_vOffset;
}

/* ========================================
	�Q�b�^�[(UV���W)�֐�
	-------------------------------------
	�ߒl�FUV���W
=========================================== */
Vector2<float> UIComponentSprite::GetUvPos()
{
	return m_vUvPos;
}

/* ========================================
	�Q�b�^�[(UV�X�P�[��)�֐�
	-------------------------------------
	�ߒl�FUV�X�P�[��
=========================================== */
Vector2<float> UIComponentSprite::GetUvScale()
{
	return m_vUvScale;
}

/* ========================================
	�Q�b�^�[(�F)�֐�
	-------------------------------------
	�ߒl�F�F
=========================================== */
Vector3<float> UIComponentSprite::GetColor()
{
	return m_vColor;
}

/* ========================================
	�Q�b�^�[(���ߓx)�֐�
	-------------------------------------
	�ߒl�F���ߓx
=========================================== */
float UIComponentSprite::GetAlpha()
{
	return m_fAlpha;
}

/* ========================================
	�Z�b�^�[(�e�N�X�`��)�֐�
	-------------------------------------
	����1�F�e�N�X�`��
=========================================== */
void UIComponentSprite::SetTexture(Texture* pTexture)
{
	m_pTexture = pTexture;
}

/* ========================================
	�Z�b�^�[(�\���t���O)�֐�
	-------------------------------------
	����1�F�\���t���O
=========================================== */
void UIComponentSprite::SetIsVisible(bool visible)
{
	m_bIsVisible = visible;
}

/* ========================================
	�Z�b�^�[(�I�t�Z�b�g)�֐�
	-------------------------------------
	����1�F�I�t�Z�b�g
=========================================== */
void UIComponentSprite::SetOffset(Vector2<float> offset)
{
	m_vOffset = offset;
}

/* ========================================
	�Z�b�^�[(UV���W)�֐�
	-------------------------------------
	����1�FUV���W
=========================================== */
void UIComponentSprite::SetUvPos(Vector2<float> uvPos)
{
	m_vUvPos = uvPos;
}

/* ========================================
	�Z�b�^�[(UV�X�P�[��)�֐�
	-------------------------------------
	����1�FUV�X�P�[��
=========================================== */
void UIComponentSprite::SetUvScale(Vector2<float> uvScale)
{
	m_vUvScale = uvScale;
}

/* ========================================
	�Z�b�^�[(�F)�֐�
	-------------------------------------
	����1�F�F
=========================================== */
void UIComponentSprite::SetColor(Vector3<float> color)
{
	m_vColor = color;
}

/* ========================================
	�Z�b�^�[(���ߓx)�֐�
	-------------------------------------
	����1�F���ߓx
=========================================== */
void UIComponentSprite::SetAlpha(float alpha)
{
	m_fAlpha = alpha;
}


#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void UIComponentSprite::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupUISprite = Item::CreateGroup("UI_Sprite");

	pGroupUISprite->AddGroupItem(Item::CreateBind("Visible", Item::Kind::Bool, &m_bIsVisible));	// �\���t���O
	pGroupUISprite->AddGroupItem(Item::CreateBind("Offset",	Item::Kind::Vector2,&m_vOffset));	// �I�t�Z�b�g
	pGroupUISprite->AddGroupItem(Item::CreateBind("UVPos",	Item::Kind::Vector2,&m_vUvPos));	// UV���W
	pGroupUISprite->AddGroupItem(Item::CreateBind("UVScale",Item::Kind::Vector2,&m_vUvScale));	// UV�X�P�[��
	pGroupUISprite->AddGroupItem(Item::CreateBind("Color",	Item::Kind::Vector,	&m_vColor));	// �F
	pGroupUISprite->AddGroupItem(Item::CreateBind("Alpha",	Item::Kind::Float,	&m_fAlpha));	// ���ߓx
	
	pGroupUISprite->AddGroupItem(InitTextureList());	// �e�N�X�`�����X�g

	window.AddItem(pGroupUISprite);

}


/* ========================================
	�e�N�X�`�����X�g�������֐�
	-------------------------------------
	���e�F�e�N�X�`�����X�g�̏�����
	-------------------------------------
	�߂�l�FDebugUI::Item*	���X�g����
=========================================== */
DebugUI::Item* UIComponentSprite::InitTextureList()
{
	using namespace DebugUI;

	DebugUI::Item* pTexList = Item::CreateList("Texture", [this](const void* arg)
	{
		// �e�N�X�`���ύX
		std::string sTexName = reinterpret_cast<const char*>(arg);
		TextureManager::E_TEX_KEY eKey;

		std::unordered_map < TextureManager::E_TEX_KEY, std::string > texPathMap = TEXTURE_MNG_INST.GetTexturePaths();
		for (auto& texPath : texPathMap)
		{
			if (texPath.second == sTexName)
			{
				eKey = texPath.first;
				break;
			}
		}

		std::unordered_map<TextureManager::E_TEX_KEY, std::shared_ptr<Texture>> texMap = TEXTURE_MNG_INST.GetTextureDatas();

		m_pTexture = texMap.find(eKey)->second.get();


	}, false, true);

	// �o�^����Ă���e�N�X�`�������X�g�ɒǉ�
	std::unordered_map<TextureManager::E_TEX_KEY, std::string> texPathMap = TEXTURE_MNG_INST.GetTexturePaths();
	for (auto& texPath : texPathMap)
	{
		pTexList->AddListItem(texPath.second.c_str());
	}

	// ���݂̃e�N�X�`����I��
	std::unordered_map<TextureManager::E_TEX_KEY, std::shared_ptr<Texture>> texMap = TEXTURE_MNG_INST.GetTextureDatas();
	for (auto& tex : texMap)
	{
		if (tex.second.get() == m_pTexture)
		{
			pTexList->SetListNo(tex.first);
			break;
		}
	}

	return pTexList;
}
#endif // _DEBUG

