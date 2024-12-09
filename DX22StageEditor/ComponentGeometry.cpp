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

// =============== �萔��` =======================



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
	, m_pTexture(nullptr)
	, m_bIsTex(false)
{
	m_pTransform = m_pOwnerObj->GetComponent<ComponentTransform>();
	m_pTexture = GET_TEXTURE_DATA(TextureManager::E_TEX_KEY::TEST);	// �f�t�H���g�e�N�X�`��
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
		m_pShape->SetPosition(m_pTransform->GetWorldPosition());
		m_pShape->SetRotation(m_pTransform->GetWorldRotation());
		m_pShape->SetScale(m_pTransform->GetWorldScale());
	}

	// �e�N�X�`���ݒ�
	m_pShape->SetTexture(m_pTexture);
	m_pShape->SetUseTexture(m_bIsTex);
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
	�����F�e�N�X�`��
=========================================== */
void ComponentGeometry::SetTexture(Texture* pTexture)
{
	m_pTexture = pTexture;
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
	pGroupGeometry->AddGroupItem(Item::CreateCallBack("UseTexture", Item::Kind::Bool, [this](bool isWrite, void* arg)
	{
		bool* pIsTex = reinterpret_cast<bool*>(arg);
		if (isWrite)
			m_bIsTex = *pIsTex;
		else
			*pIsTex = m_bIsTex;

		// �e�N�X�`���g�p�t���O��ON�̏ꍇ�̂݃e�N�X�`�����X�g��\��
		if (!m_bIsTex)
		{
			WIN_OBJ_INFO["Geometry"]["Texture"].RemoveListItemAll();	// ���X�g���N���A
		}
		else
		{	// �e�N�X�`�����X�g�ɓo�^����Ă���摜�����X�g�ɒǉ�
			std::unordered_map < TextureManager::E_TEX_KEY, std::string > texPathMap = TEXTURE_MNG_INST.GetTexturePaths();
			for (auto& texPath : texPathMap)
			{
				WIN_OBJ_INFO["Geometry"]["Texture"].AddListItem(texPath.second.c_str());
			}
		}
	}));	// �e�N�X�`���g�p�t���O
	// �e�N�X�`�����X�g
	pGroupGeometry->AddGroupItem(InitTextureList());

	window.AddItem(pGroupGeometry);

	
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
DebugUI::Item* ComponentGeometry::InitTextureList()
{
	using namespace DebugUI;

	// �e�N�X�`�����X�g
	Item* pTextureList = Item::CreateList("Texture", [this](const void* arg)
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
		m_pTexture = texMap.find(eKey)->second.get();

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
		if (tex.second.get() == m_pTexture)
		{
			pTextureList->SetListNo(tex.first);
			break;
		}
	}
	
	return pTextureList;
}

#endif // _DEBUG