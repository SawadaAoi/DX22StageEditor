/* ========================================
	DX22Base/
	------------------------------------
	�}�`�\���R���|�[�l���g�pcpp
	------------------------------------
	ComponentGeometry.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ComponentGeometry.h"
#include "ComponentTransform.h"
#include "ObjectBase.h"
#include "ShapeBox.h"		// ������
#include "ShapeSphere.h"	// ��
#include "ShapePlane.h"		// ����
#include "TextureManager.h"
#include "LightManager.h"
#include "CameraManager.h"


// =============== �萔��` =======================
const int MAX_TEXTURE_NUM	= 6;	// �e�N�X�`���ő吔
const int MAX_UV_NUM		= 3;	// uv�ő吔(Z,X,Y)

// �f�o�b�O�p��UV�̔z��ԍ��Ǝ���
static std::unordered_map<int, std::string> mapUvName =
{
	{ 0, "Z" },
	{ 1, "X" },
	{ 2, "Y" },
};

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F�R���|�[�l���g�����I�u�W�F�N�g
=========================================== */
ComponentGeometry::ComponentGeometry(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderGeometry)
	, m_eType(TYPE_NONE)
	, m_pShape(nullptr)
	, m_pTransform(nullptr)
	, m_pTextures()
	, m_bIsTex(false)
	, m_bIsCulling(true)
	, m_fUvScale()
	, m_fUvOffset()
{
	m_pTransform = m_pOwnerObj->GetComponent<ComponentTransform>();

	m_pTextures.resize(MAX_TEXTURE_NUM);
	for (auto& tex : m_pTextures)
		tex = GET_TEXTURE_DATA(TextureManager::E_TEX_KEY::TEST);

	m_fUvOffset.resize(MAX_UV_NUM);
	for (auto& uv : m_fUvOffset)
		uv = { 0.0f, 0.0f };

	m_fUvScale.resize(MAX_UV_NUM);
	for (auto& scale : m_fUvScale)
		scale = { 1.0f, 1.0f };
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
=========================================== */
void ComponentGeometry::Init()
{
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void ComponentGeometry::Update()
{
	// �}�`�I�u�W�F�N�g����������Ă��Ȃ��ꍇ�͏������s��Ȃ�
	if (m_pShape == nullptr) return;

	// ���W�R���|�[�l���g���擾�ł����ꍇ�͍��W��ݒ�
	if (m_pTransform != nullptr)
	{
		// ���W�R���|�[�l���g���擾�ł����ꍇ�͍��W��ݒ�
		m_pShape->SetPosition(m_pTransform->GetPosition());
		m_pShape->SetRotation(m_pTransform->GetRotation());
		m_pShape->SetScale(m_pTransform->GetScale());
	}

	// �e�N�X�`���ݒ�
	// �����̂̏ꍇ��6�ʕ��̃e�N�X�`����ݒ�
	if (m_eType == TYPE_BOX)
	{
		for (int i = 0; i < MAX_TEXTURE_NUM; i++)
		{
			m_pShape->SetTexture(m_pTextures[i], i);
		}
		for (int i = 0; i < MAX_UV_NUM; i++)
		{
			m_pShape->SetUvScale(m_fUvScale[i], i);
			m_pShape->SetUvOffset(m_fUvOffset[i], i);
		}
	}
	// ���A���ʂ̏ꍇ��1�̃e�N�X�`����ݒ�
	else
	{
		m_pShape->SetTexture(m_pTextures[0]);
		m_pShape->SetUvScale(m_fUvScale[0]);
		m_pShape->SetUvOffset(m_fUvOffset[0]);
	}
	m_pShape->SetUseTexture(m_bIsTex);
	m_pShape->SetIsCulling(m_bIsCulling);
}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
=========================================== */
void ComponentGeometry::Draw()
{
	// �}�`�I�u�W�F�N�g����������Ă���ꍇ�͕`��
	if (m_pShape)
	{
		ObjectBase::T_LightParam lightParam = m_pOwnerObj->GetLightMaterial();
		m_pShape->SetLightMaterial(lightParam.fDiffuse, lightParam.fSpecular, lightParam.fAmbient, lightParam.bLightUse);					// ���C�g�p�����[
		m_pShape->SetLights(LIGHT_MNG_INST.GetLightList());	// ���C�g�ݒ�
		m_pShape->SetCameraPos(CAMERA_MNG_INST.GetActiveCamera()->GetComponent<ComponentTransform>()->GetPosition());	// �J�����ʒu�ݒ�

		m_pShape->Draw();
	}
}


/* ========================================
	�}�`�I�u�W�F�N�g�����֐�
	-------------------------------------
	���e�F�}�`�I�u�W�F�N�g�𐶐�
=========================================== */
void ComponentGeometry::CreateShape()
{
	// ���ɐ�������Ă���ꍇ�͉��
	if (m_pShape)
	{
		m_pShape.reset();
	}

	// �}�`�̎�ނɂ���Đ�������I�u�W�F�N�g��ύX
	switch (m_eType)
	{
		// �Ȃ�
	case TYPE_NONE:
		m_pShape = nullptr;
		break;
		// ������
	case TYPE_BOX:
		m_pShape = std::make_shared<ShapeBox>();
		break;
		// ��
	case TYPE_SPHERE:
		m_pShape = std::make_shared<ShapeSphere>();
		break;
	case TYPE_PLANE:
		m_pShape = std::make_shared<ShapePlane>();
		break;
	}
}

/* ========================================
	�Q�b�^�[(�}�`���)�֐�
	-------------------------------------
	�߂�l�F�}�`�̎��
=========================================== */
ComponentGeometry::E_ShapeType ComponentGeometry::GetShapeType() const
{
	return m_eType;
}

/* ========================================
	�Q�b�^�[(�e�N�X�`��)�֐�
	-------------------------------------
	�����Fint �z��ԍ�
	-------------------------------------
	�߂�l�F�e�N�X�`��
=========================================== */
Texture* ComponentGeometry::GetTexture(int nIndex) const
{
	// �����̂̏ꍇ��6�ʕ��̃e�N�X�`����Ԃ�
	if (m_eType == TYPE_BOX)
	{
		// �C���f�b�N�X���͈͓��̏ꍇ�̓e�N�X�`����Ԃ�
		if (nIndex >= 0 && nIndex < MAX_TEXTURE_NUM)
			return m_pTextures[nIndex];
		else
			return m_pTextures[0];
	}
	else
	{
		return m_pTextures[0];
	}
}

/* ========================================
	�Q�b�^�[(�e�N�X�`���g�p�t���O)�֐�
	-------------------------------------
	�߂�l�F�e�N�X�`���g�p�t���O
=========================================== */
bool ComponentGeometry::GetIsTex() const
{
	return m_bIsTex;
}

/* ========================================
	�Q�b�^�[(�J�����O�ݒ�)�֐�
	-------------------------------------
	�߂�l�F�J�����O�ݒ�
=========================================== */
bool ComponentGeometry::GetCulling() const
{
	return m_bIsCulling;
}

/* ========================================
	�Q�b�^�[(�e�N�X�`���I�t�Z�b�g)�֐�
	-------------------------------------
	�����Fint �z��ԍ�
	-------------------------------------
	�߂�l�F�e�N�X�`���I�t�Z�b�g
=========================================== */
Vector2<float> ComponentGeometry::GetUvScale(int nIndex) const
{
	// �����̂̏ꍇ��Z,X,Y�̃e�N�X�`���I�t�Z�b�g�Ԃ�
	if (m_eType == TYPE_BOX)
	{
		// �C���f�b�N�X���͈͓��̏ꍇ�̓e�N�X�`���I�t�Z�b�g�Ԃ�
		if (nIndex >= 0 && nIndex < MAX_UV_NUM)
			return m_fUvScale[nIndex];
		else
			return m_fUvScale[0];
	}
	else
	{
		return m_fUvScale[0];
	}
}

/* ========================================
	�Q�b�^�[(�e�N�X�`���X�P�[��)�֐�
	-------------------------------------
	�����Fint �z��ԍ�
	-------------------------------------
	�߂�l�F�e�N�X�`���X�P�[��
=========================================== */
Vector2<float> ComponentGeometry::GetUvOffset(int nIndex) const
{
	// �����̂̏ꍇ��Z,X,Y�̃e�N�X�`���X�P�[���Ԃ�
	if (m_eType == TYPE_BOX)
	{
		// �C���f�b�N�X���͈͓��̏ꍇ�̓e�N�X�`���X�P�[���Ԃ�
		if (nIndex >= 0 && nIndex < MAX_UV_NUM)
			return m_fUvOffset[nIndex];
		else
			return m_fUvOffset[0];
	}
	else
	{
		return m_fUvOffset[0];
	}
}

/* ========================================
	�Z�b�^�[(�}�`���)�֐�
	-------------------------------------
	�����F�}�`�̎��
=========================================== */
void ComponentGeometry::SetShapeType(E_ShapeType eType)
{
	m_eType = eType;
	CreateShape();	// �ݒ肵���l�Ő}�`�I�u�W�F�N�g�𐶐�

}

/* ========================================
	�Z�b�^�[(�e�N�X�`��)�֐�
	-------------------------------------
	����1�F�e�N�X�`��
	����2�Fint �z��ԍ�
=========================================== */
void ComponentGeometry::SetTexture(Texture* pTexture, int nIndex)
{
	// �����̂̏ꍇ��6�ʕ��̃e�N�X�`����ݒ�
	if (m_eType == TYPE_BOX)
	{
		// �C���f�b�N�X���͈͓��̏ꍇ�̓e�N�X�`����ݒ�
		if (nIndex >= 0 && nIndex < MAX_TEXTURE_NUM)
			m_pTextures[nIndex] = pTexture;
		else
			m_pTextures[0] = pTexture;
	}
	else
	{
		m_pTextures[0] = pTexture;
	}
}

/* ========================================
	�Z�b�^�[(�e�N�X�`��(�S��))�֐�
	-------------------------------------
	�����F�e�N�X�`��
=========================================== */
void ComponentGeometry::SetTextureAll(Texture* pTexture)
{
	if (m_eType == TYPE_BOX)
	{
		for (int i = 0; i < MAX_TEXTURE_NUM; i++)
		{
			m_pTextures[i] = pTexture;
		}
	}
	else
	{
		m_pTextures[0] = pTexture;
	}
}

/* ========================================
	�Z�b�^�[(�e�N�X�`���g�p�t���O)�֐�
	-------------------------------------
	�����F�e�N�X�`���g�p�t���O
=========================================== */
void ComponentGeometry::SetIsTex(bool bIsTex)
{
	m_bIsTex = bIsTex;
}

/* ========================================
	�Z�b�^�[(�J�����O�ݒ�)�֐�
	-------------------------------------
	�����F�J�����O�ݒ�
=========================================== */
void ComponentGeometry::SetCulling(bool bIsCulling)
{
	m_bIsCulling = bIsCulling;
}

/* ========================================
	�Z�b�^�[(�e�N�X�`���I�t�Z�b�g)�֐�
	-------------------------------------
	����1�F�e�N�X�`���I�t�Z�b�g
	����2�Fint �z��ԍ�
=========================================== */
void ComponentGeometry::SetUvScale(const Vector2<float>& scale, int nIndex)
{
	// �����̂̏ꍇ��Z,X,Y�̃e�N�X�`���I�t�Z�b�g�ݒ�
	if (m_eType == TYPE_BOX)
	{
		// �C���f�b�N�X���͈͓��̏ꍇ�̓e�N�X�`���I�t�Z�b�g�ݒ�
		if (nIndex >= 0 && nIndex < MAX_UV_NUM)
			m_fUvScale[nIndex] = scale;
		else
			m_fUvScale[0] = scale;
	}
	else
	{
		m_fUvScale[0] = scale;
	}
}

/* ========================================
	�Z�b�^�[(�e�N�X�`���X�P�[��)�֐�
	-------------------------------------
	����1�F�e�N�X�`���X�P�[��
	����2�Fint �z��ԍ�
=========================================== */
void ComponentGeometry::SetUvOffset(const Vector2<float>& offset, int nIndex)
{
	// �����̂̏ꍇ��Z,X,Y�̃e�N�X�`���X�P�[���ݒ�
	if (m_eType == TYPE_BOX)
	{
		// �C���f�b�N�X���͈͓��̏ꍇ�̓e�N�X�`���X�P�[���ݒ�
		if (nIndex >= 0 && nIndex < MAX_UV_NUM)
			m_fUvOffset[nIndex] = offset;
		else
			m_fUvOffset[0] = offset;
	}
	else
	{
		m_fUvOffset[0] = offset;
	}
}

#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentGeometry::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupGeometry = Item::CreateGroup("Geometry");
	// �}�`��ރ��X�g
	pGroupGeometry->AddGroupItem(InitShapeTypeList());	
	// �e�N�X�`���g�p�t���O
	pGroupGeometry->AddGroupItem(InitUseTexture());	// �e�N�X�`���g�p�t���O

	// 0�Ԗڂ̃e�N�X�`����S�R�s�[
	pGroupGeometry->AddGroupItem(InitTexAllCopy());	// �e�N�X�`���S�R�s�[

	// �e�N�X�`�����X�g
	int nTexNum = (m_eType == TYPE_BOX) ? MAX_TEXTURE_NUM : 1;	// �����̂̏ꍇ��6�ʕ��̃e�N�X�`�����X�g��\��
	for (int i = 0; i < nTexNum; i++)
		pGroupGeometry->AddGroupItem(InitTextureList(i));

	// �J�����O�ݒ�
	pGroupGeometry->AddGroupItem(Item::CreateBind("Culling", Item::Kind::Bool, &m_bIsCulling));

	window.AddItem(pGroupGeometry);

	// Uv�I�t�Z�b�g,�X�P�[��
	InitUv();

	
}

/* ========================================
	�}�`��ރ��X�g�������֐�
	-------------------------------------
	���e�F�}�`��ރ��X�g�̏�����
	-------------------------------------
	�߂�l�FDebugUI::Item*	���X�g����
=========================================== */
DebugUI::Item* ComponentGeometry::InitShapeTypeList()
{
	using namespace DebugUI;


	// �}�`�̎��
	std::unordered_map<std::string, E_ShapeType> mapShapeType =
	{
		{ "None",	TYPE_NONE },
		{ "Box",	TYPE_BOX },
		{ "Sphere", TYPE_SPHERE },
		{ "Plane",	TYPE_PLANE },
	};

	// �}�`�ꗗ���X�g
	Item* pShapeList = Item::CreateList("ShapeType", [this, mapShapeType](const void* arg)
	{	// ���ڃN���b�N�Ő}�`�ύX
		std::string sType = reinterpret_cast<const char*>(arg);
		SetShapeType(mapShapeType.at(sType));

		// �e�N�X�`���g�p�t���O��ON�̏ꍇ�̂݃e�N�X�`�����X�g��\��
		if (m_bIsTex)
		{
			// ���X�g���N���A
			for (int i = 0; i < MAX_TEXTURE_NUM; i++)
			{
				std::string sTexName = "Texture_" + std::to_string(i);
				WIN_OBJ_INFO["Geometry"][sTexName.c_str()].RemoveListItemAll();
			}

			// �����̂̏ꍇ��6�ʕ��̃e�N�X�`�����X�g��\��
			int nTexNum = (m_eType == TYPE_BOX) ? MAX_TEXTURE_NUM : 1;
			// �e�N�X�`�����X�g�ɓo�^����Ă���摜�����X�g�ɒǉ�
			std::unordered_map < TextureManager::E_TEX_KEY, std::string > texPathMap = TEXTURE_MNG_INST.GetTexturePaths();
			for (int i = 0; i < nTexNum; i++)
			{
				std::string sTexName = "Texture_" + std::to_string(i);
				for (auto& texPath : texPathMap)
				{
					WIN_OBJ_INFO["Geometry"][sTexName.c_str()].AddListItem(texPath.second.c_str());
				}
			}

			RemoveUv();	// Uv�I�t�Z�b�g,�X�P�[�����폜
			InitUv();	// Uv�I�t�Z�b�g,�X�P�[����������

		}


	}, false, true);

	// ���X�g���ڒǉ�
	for (auto& shape : mapShapeType)
	{
		pShapeList->AddListItem(shape.first.c_str());
	}
	pShapeList->SetListNo(static_cast<int>(m_eType));	// �I�𒆂̐}�`��ݒ�

	return pShapeList;
}


/* ========================================
	�e�N�X�`�����X�g�������֐�
	-------------------------------------
	���e�F�e�N�X�`�����X�g�̏�����
	-------------------------------------
	�߂�l�FDebugUI::Item*	���X�g����
=========================================== */
DebugUI::Item* ComponentGeometry::InitTextureList(int nIdx)
{
	using namespace DebugUI;

	// �e�N�X�`�����X�g
	Item* pTextureList = Item::CreateList(("Texture_" + std::to_string(nIdx)).c_str(), [this, nIdx](const void* arg)
	{
		// ���X�g�̕\������(�摜�p�X)����摜�L�[���擾
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
		// �摜�L�[����e�N�X�`�����擾
		std::unordered_map<TextureManager::E_TEX_KEY, std::shared_ptr<Texture>> texMap = TEXTURE_MNG_INST.GetTextureDatas();
		m_pTextures[nIdx] = texMap.find(eKey)->second.get();

	}, false, true);	// �v���_�E��

	// �o�^����Ă���e�N�X�`�������X�g�ɒǉ�
	std::unordered_map < TextureManager::E_TEX_KEY, std::string > texPathMap = TEXTURE_MNG_INST.GetTexturePaths();
	for (auto& texPath : texPathMap)
	{
		pTextureList->AddListItem(texPath.second.c_str());
	}

	// ���ݐݒ肳��Ă���摜���烊�X�g�̕\�����ڂ�ݒ�
	std::unordered_map<TextureManager::E_TEX_KEY, std::shared_ptr<Texture>> texMap = TEXTURE_MNG_INST.GetTextureDatas();
	for (auto& tex : texMap)
	{
		if (tex.second.get() == m_pTextures[nIdx])
		{
			pTextureList->SetListNo(tex.first);
			break;
		}
	}
	
	return pTextureList;
}

/* ========================================
	�e�N�X�`���g�p�t���O�������֐�
	-------------------------------------
	���e�F�e�N�X�`���g�p�t���O�̏�����
	-------------------------------------
	�߂�l�FDebugUI::Item*	���X�g����
=========================================== */
DebugUI::Item* ComponentGeometry::InitUseTexture()
{
	using namespace DebugUI;

	Item* pUseTexture = Item::CreateCallBack("UseTexture", Item::Kind::Bool, [this](bool isWrite, void* arg)
	{
		bool* pIsTex = reinterpret_cast<bool*>(arg);
		if (isWrite)
		{
			m_bIsTex = *pIsTex;
			// OFF�̏ꍇ�̓e�N�X�`�����X�g�A�J�����O�ݒ�AUv�I�t�Z�b�g,�X�P�[�����\��
			if (!m_bIsTex)
			{
				// �e�N�X�`�����X�g���N���A
				int nTexNum = (m_eType == TYPE_BOX) ? MAX_TEXTURE_NUM : 1;
				for (int i = 0; i < nTexNum; i++)
				{
					std::string sTexName = "Texture_" + std::to_string(i);
					WIN_OBJ_INFO["Geometry"][sTexName.c_str()].RemoveListItemAll();
				}
				// �J�����O�ݒ���폜
				WIN_OBJ_INFO["Geometry"].RemoveGroupItem("Culling");	
				// Uv�I�t�Z�b�g,�X�P�[�����폜
				RemoveUv();	

			}
			// �e�N�X�`���g�p�t���O��ON�̏ꍇ�e�N�X�`�����X�g�A�J�����O�ݒ�AUv�I�t�Z�b�g,�X�P�[����\��
			else
			{
				// �e�N�X�`�����X�g�̕\��
				int nTexNum = (m_eType == TYPE_BOX) ? MAX_TEXTURE_NUM : 1;
				// �e�N�X�`�����X�g�ɓo�^����Ă���摜�����X�g�ɒǉ�
				std::unordered_map < TextureManager::E_TEX_KEY, std::string > texPathMap = TEXTURE_MNG_INST.GetTexturePaths();
				for (int i = 0; i < nTexNum; i++)
				{
					std::string sTexName = "Texture_" + std::to_string(i);
					for (auto& texPath : texPathMap)
					{
						WIN_OBJ_INFO["Geometry"][sTexName.c_str()].AddListItem(texPath.second.c_str());
					}
				}
				// �J�����O�ݒ�
				WIN_OBJ_INFO["Geometry"].AddGroupItem(Item::CreateBind("Culling", Item::Kind::Bool, &m_bIsCulling));
				// Uv�I�t�Z�b�g,�X�P�[����������
				InitUv();	
			}
		}
		else
		{
			*pIsTex = m_bIsTex;
		}
	});

	return pUseTexture;
}

/* ========================================
	�e�N�X�`���S�R�s�[�������֐�
	-------------------------------------
	���e�F�e�N�X�`���S�R�s�[�̏�����
	-------------------------------------
	�߂�l�FDebugUI::Item*	���X�g����
=========================================== */
DebugUI::Item* ComponentGeometry::InitTexAllCopy()
{
	using namespace DebugUI;

	Item* pInitTexAllCopy = Item::CreateCallBack("TexAllCopy_0", Item::Kind::Command, [this](bool isWrite, void* arg)
	{
		// ���ݐݒ肳��Ă���摜���烊�X�g�̕\�����ڂ�ݒ�
		std::unordered_map<TextureManager::E_TEX_KEY, std::shared_ptr<Texture>> texMap = TEXTURE_MNG_INST.GetTextureDatas();
		for (int i = 0; i < MAX_TEXTURE_NUM; i++)
		{
			m_pTextures[i] = m_pTextures[0];

			// ���ݐݒ肳��Ă���摜���烊�X�g�̕\�����ڂ�ݒ�
			std::string sTexName = "Texture_" + std::to_string(i);	// ���ږ����擾
			for (auto& tex : texMap)
			{
				if (tex.second.get() == m_pTextures[i])
				{
					WIN_OBJ_INFO["Geometry"][sTexName.c_str()].SetListNo(tex.first);
					break;
				}
			}
		}
	}, false, true);

		
	return pInitTexAllCopy;
}

/* ========================================
	Uv�I�t�Z�b�g,�X�P�[���������֐�
	-------------------------------------
	���e�FUv�I�t�Z�b�g,�X�P�[���̏�����
=========================================== */
void ComponentGeometry::InitUv()
{
	using namespace DebugUI;

	// �����̂̏ꍇ(6�ʕ��̃e�N�X�`���I�t�Z�b�g�A�X�P�[����ݒ�)
	if (m_eType == TYPE_BOX)
	{
		for (int i = 0; i < MAX_UV_NUM; i++)
		{
			std::string sUvScaleName = "UvScale_" + mapUvName[i];
			std::string sUvOffsetName = "UvOffset_" + mapUvName[i];
			// �e�N�X�`���X�P�[��
			WIN_OBJ_INFO["Geometry"].AddGroupItem(Item::CreateBind(sUvScaleName.c_str(), Item::Kind::Vector2, &m_fUvScale[i]));
			// �e�N�X�`���I�t�Z�b�g
			WIN_OBJ_INFO["Geometry"].AddGroupItem(Item::CreateBind(sUvOffsetName.c_str(), Item::Kind::Vector2, &m_fUvOffset[i]));
		}
	}
	// ���A���ʂ̏ꍇ(1�̃e�N�X�`���I�t�Z�b�g�A�X�P�[����ݒ�)
	else
	{
		// �e�N�X�`���X�P�[��
		WIN_OBJ_INFO["Geometry"].AddGroupItem(Item::CreateBind("UvScale", Item::Kind::Vector2, &m_fUvScale[0]));
		// �e�N�X�`���I�t�Z�b�g
		WIN_OBJ_INFO["Geometry"].AddGroupItem(Item::CreateBind("UvOffset", Item::Kind::Vector2, &m_fUvOffset[0]));
	}
}

/* ========================================
	Uv�I�t�Z�b�g,�X�P�[���폜�֐�
	-------------------------------------
	���e�FUv�I�t�Z�b�g,�X�P�[���̍폜
=========================================== */
void ComponentGeometry::RemoveUv()
{
	using namespace DebugUI;

	// �����̂̏ꍇ(6�ʕ�)�Ƌ��A���ʂ̏�(1��)�̗��p�^�[���̍��ڂ��폜
	for (int i = 0; i < MAX_UV_NUM; i++)
	{
		std::string sUvScaleName = "UvScale_" + mapUvName[i];
		std::string sUvOffsetName = "UvOffset_" + mapUvName[i];
		WIN_OBJ_INFO["Geometry"].RemoveGroupItem(sUvScaleName.c_str());
		WIN_OBJ_INFO["Geometry"].RemoveGroupItem(sUvOffsetName.c_str());
	}

	// �e�N�X�`���X�P�[��
	WIN_OBJ_INFO["Geometry"].RemoveGroupItem("UvScale");
	// �e�N�X�`���I�t�Z�b�g
	WIN_OBJ_INFO["Geometry"].RemoveGroupItem("UvOffset");
}

#endif // _DEBUG