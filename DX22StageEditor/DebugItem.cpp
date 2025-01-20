/* ========================================
	DX22Base/
	------------------------------------
	�f�o�b�O�\�����ڗpcpp
	------------------------------------
	DebugItem.cpp
========================================== */
#ifdef _DEBUG	// �f�o�b�O���[�h�̂ݗL��

// =============== �C���N���[�h ===================
#include "DebugItem.h"

// =============== ���O��� ===================
namespace DebugUI {

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
=========================================== */
Item::Item()
	: m_sName("None")
	, m_eKind(Kind::Label)
	, m_bSave(false)
	, m_uValue{}
	, m_bSameLine(false)
{

}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F�������
=========================================== */
Item::~Item()
{
}

/* ========================================
	���ڍ쐬(�P��)
	-------------------------------------
	���e�F�l��ێ����鍀�ڂ��쐬
	-------------------------------------
	�����Fconst char* name		���ږ�
	�����FKind kind				���ڎ��
	�����Fbool isSave			�ۑ����邩
	�����Fbool isSameLine		�����s�ɕ\�����邩
	-------------------------------------
	�߂�l�FItem* ���ڃ|�C���^
=========================================== */
Item* Item::CreateValue(const char* name, Kind kind, bool isSave, bool isSameLine)
{
	ItemValue* item		= new ItemValue;
	item->m_sName		= name;		
	item->m_eKind		= kind;
	item->m_bSave		= isSave;
	item->m_bSameLine	= isSameLine;
	return item;
}

/* ========================================
	���ڍ쐬(�ϐ��R�Â�)
	-------------------------------------
	���e�F�ϐ��ƕR�Â������ڂ��쐬
	-------------------------------------
	�����Fconst char* name		���ږ�
	�����FKind kind				���ڎ��
	�����Fvoid* ptr				�ϐ��|�C���^
	�����Fbool isSave			�ۑ����邩
	�����Fbool isSameLine		�����s�ɕ\�����邩
	-------------------------------------
	�߂�l�FItem* ���ڃ|�C���^
=========================================== */
Item* Item::CreateBind(const char* name, Kind kind, void* ptr, bool isSave, bool isSameLine)
{
	ItemBind* item		= new ItemBind;
	item->m_sName		= name;
	item->m_eKind		= kind;
	item->m_pBindVar	= ptr;
	item->m_bSave		= isSave;
	item->m_bSameLine = isSameLine;
	return item;
}


/* ========================================
	���ڍ쐬(�R�[���o�b�N)
	-------------------------------------
	���e�F�R�[���o�b�N�֐���R�Â������ڂ��쐬
	-------------------------------------
	�����Fconst char* name		���ږ�
	�����FKind kind				���ڎ��
	�����FCallback func			�R�[���o�b�N�֐�
	�����Fbool isSave			�ۑ����邩
	-------------------------------------
	�߂�l�FItem* ���ڃ|�C���^
=========================================== */
Item* Item::CreateCallBack(const char* name, Kind kind, Callback func, bool isSave, bool isSameLine)
{
	ItemCallback* item	= new ItemCallback;
	item->m_sName			= name;
	item->m_eKind			= kind;
	item->m_CallbackFunc	= func;
	item->m_bSave			= isSave;
	item->m_bSameLine = isSameLine;
	return item;
}


/* ========================================
	���ڍ쐬(�O���[�v)
	-------------------------------------
	���e�F�O���[�v���ڂ��쐬
	-------------------------------------
	�����Fconst char* name		���ږ�
	-------------------------------------
	�߂�l�FItem* ���ڃ|�C���^
=========================================== */
Item* Item::CreateGroup(const char* name)
{
	ItemGroup* item = new ItemGroup;
	item->m_sName		= name;
	item->m_eKind		= Item::Group;
	return item;
}

/* ========================================
	���ڍ쐬(���X�g)
	-------------------------------------
	���e�F���X�g���ڂ��쐬
	-------------------------------------
	����1�Fconst char* name		���ږ�
	����2�FConstCallback func	�R�[���o�b�N�֐�
	����3�Fbool isSave			�ۑ����邩
	����4�Fbool isDropDown		�\���`�����h���b�v�_�E���ɂ��邩
	����5�Fbool isSameLine		�����s�ɕ\�����邩
	����6�Fint nDispLineNum		�\���s��
	-------------------------------------
	�߂�l�FItem* ���ڃ|�C���^
=========================================== */
Item* Item::CreateList(const char* name, ConstCallback func, bool isSave, bool bIsDropDown, bool isSameLine, int nDispLineNum)
{
	ItemList* item			= new ItemList;
	item->m_sName			= name;
	item->m_eKind			= Item::List;
	item->m_CallbackFunc	= func;
	item->m_bSave			= isSave;
	item->m_uValue.nValue	= -1;		// �ŏ��͉����I�����Ă��Ȃ����
	item->m_bIsDropDown		= bIsDropDown;
	item->m_bSameLine		= isSameLine;
	item->m_nDispLineNum	= nDispLineNum;
	return item;
}

/* ========================================
	���X�g���ڒǉ��֐�
	-------------------------------------
	���e�F���X�g�ɕ\�����ڂ�ǉ�
	-------------------------------------
	����1�Fconst char* name		���ږ�
=========================================== */
void Item::AddListItem(const char* name)
{
	if (m_eKind != List) return;

	ItemList* pItemList = reinterpret_cast<ItemList*>(this);

	// �������O�̍��ڂ�����ꍇ�͒ǉ����Ȃ�
	if (std::find(pItemList->m_sItemList.begin(), pItemList->m_sItemList.end(), name) != pItemList->m_sItemList.end())
		return;

	reinterpret_cast<ItemList*>(this)->m_sItemList.push_back(name);
}

/* ========================================
	���X�g���ڑ}���֐�
	-------------------------------------
	���e�F���X�g�̎w�肵���ʒu�ɕ\�����ڂ�}��
	-------------------------------------
	����1�Fconst char* name		���ږ�
	����2�Fint num				�}���ʒu
=========================================== */
void Item::InsertListItem(const char* name, int num)
{
	if (m_eKind != List) return;
	ItemList* pItemList = reinterpret_cast<ItemList*>(this);
	std::list<std::string>::iterator itr = pItemList->m_sItemList.begin();

	// �}���ʒu�܂ňړ�
	for (int i = 0; i < num; ++i)
		++itr;

	pItemList->m_sItemList.insert(itr, name);
}

/* ========================================
	���X�g���ڍ폜�֐�
	-------------------------------------
	���e�F���X�g����\�����ڂ��폜
	-------------------------------------
	����1�Fconst char* name		���ږ�
=========================================== */
void Item::RemoveListItem(const char* name)
{
	if (m_eKind != List) return;
	reinterpret_cast<ItemList*>(this)->m_sItemList.remove(name);
}

/* ========================================
	���X�g���ڍ폜�֐�(�w�蕶���񏜋�)
	-------------------------------------
	���e�F���X�g����\�����ڂ��폜
		�@�������A���X�g�ꗗ����w��̕�������菜����������Ō���
		�@���I�u�W�F�N�g�ꗗ�Ɏg�p
	-------------------------------------
	����1�Fconst char* name		���ږ�
	����2�Fstd::string sExceptText	�������ɏ������镶����
=========================================== */
void Item::RemoveListItem(const char* name, std::string sExceptText)
{
	if (m_eKind != List) return;

	ItemList* pItemList = reinterpret_cast<ItemList*>(this);

	// ������
	std::string sSearchName = name;
	// �w��̕��������菜��
	if (sSearchName.find(sExceptText) != std::string::npos)
	{
		int nHeadTextCnt = sSearchName.find(sExceptText);
		sSearchName = sSearchName.substr(nHeadTextCnt + sExceptText.size());
	}

	// ���X�g�ꗗ������
	std::list<std::string>::iterator itr = pItemList->m_sItemList.begin();
	while (itr != pItemList->m_sItemList.end())
	{
		std::string sName = *itr;

		// �w��̕��������菜��
		if (sName.find(sExceptText) != std::string::npos) {
			// �擪����DebugUI::CHILD_NAME_HEDAD_TEXT�܂ł̕�����
			int nHeadTextCnt = sName.find(sExceptText);
			sName = sName.substr(nHeadTextCnt + sExceptText.size());
		}
		// �������ƈ�v������폜
		if (sSearchName == sName)
		{
			pItemList->m_sItemList.erase(itr);
			break;
		}
		++itr;
	}
}

/* ========================================
	���X�g���ڑS�폜�֐�
	-------------------------------------
	���e�F���X�g����S�Ă̕\�����ڂ��폜
=========================================== */
void Item::RemoveListItemAll()
{
	if (m_eKind != List) return;
	reinterpret_cast<ItemList*>(this)->m_sItemList.clear();
}


/* ========================================
	�O���[�v���ڒǉ��֐�
	-------------------------------------
	���e�F�O���[�v�ɕ\�����ڂ�ǉ�
	-------------------------------------
	����1�FItem* item		����
=========================================== */
void Item::AddGroupItem(Item* item)
{
	if (m_eKind != Group) return;
	static_cast<ItemGroup*>(this)->m_pItemGroup.push_back(item);
}

/* ========================================
	�z�񉉎Z�q(�O���[�v)
	-------------------------------------
	���e�F�w�薼�̃A�C�e�����擾
	-------------------------------------
	�����Fconst char* name ���ږ�
	-------------------------------------
	�߂�l�FItem& �A�C�e��
=========================================== */
Item& Item::operator[](const char* name)
{
	// �O���[�v�ȊO�̎擾�̓_�~�[��ԋp
	if (m_eKind != Group) return *dummy;

	// �O���[�v���̃A�C�e���ꗗ���擾
	auto items = static_cast<ItemGroup*>(this)->m_pItemGroup;

	// �A�C�e���ꗗ����w��A�C�e���̒T��
	auto it = std::find_if(items.begin(), items.end(),
		[&name](const Item* obj) {
		return strcmp(obj->GetName(), name) == 0;
	});

	// ���������A�C�e���̕ԋp
	if (it != items.end())
		return **it;

	// ������Ȃ�������_�~�[��ԋp
	return *dummy;
}


/* ========================================
	�ϊ�(�����񁨍��ڎ��)�֐�
	-------------------------------------
	���e�F�����񂩂獀�ڎ�ʂɕϊ�
	-------------------------------------
	�����Fstd::string ������
	-------------------------------------
	�߂�l�FKind kind ���ڎ��
=========================================== */
Item::Kind Item::StrToKind(std::string str)
{
	if (str == "Label")		return Item::Label;
	if (str == "Bool")		return Item::Bool;
	if (str == "Int")		return Item::Int;
	if (str == "Float")		return Item::Float;
	if (str == "Vector")	return Item::Vector;
	if (str == "Color")		return Item::Color;
	if (str == "Path")		return Item::Path;
	if (str == "Command")	return Item::Command;
	if (str == "Group")		return Item::Group;
	if (str == "List")		return Item::List;
	return Item::Label;
}

/* ========================================
	�ϊ�(���ڎ�ʁ�������)�֐�
	-------------------------------------
	���e�F���ڎ�ʂ��當����ɕϊ�
	-------------------------------------
	�����FKind kind ���ڎ��
	-------------------------------------
	�߂�l�Fstd::string ������
=========================================== */
std::string Item::KindToStr(Kind kind)
{
	switch (kind) {
	default:
	case Item::Label:		return "Label";
	case Item::Bool:		return "Bool";
	case Item::Int:			return "Int";
	case Item::Float:		return "Float";
	case Item::Vector:		return "Vector";
	case Item::Color:		return "Color";
	case Item::Path:		return "Path";
	case Item::Command:		return "Command";
	case Item::Group:		return "Group";
	case Item::List:		return "List";
	}
}

/* ========================================
	�Q�b�^�[(���ږ�)�֐�
	-------------------------------------
	�ߒl�Fconst char* ���ږ�
=========================================== */
const char* Item::GetName() const
{
	return m_sName.c_str();
}

/* ========================================
	�Q�b�^�[(���ڎ��)�֐�
	-------------------------------------
	�ߒl�FKind ���ڎ��
=========================================== */
Item::Kind Item::GetKind() const
{
	return m_eKind;
}

/* ========================================
	�Q�b�^�[(����(bool))�֐�
	-------------------------------------
	�ߒl�Fbool ���ڂ̒l
=========================================== */
bool Item::GetBool() const
{
	if (m_eKind == Bool)
		return reinterpret_cast<const ItemValue*>(this)->m_uValue.bValid;
	return false;
}

/* ========================================
	�Q�b�^�[(����(int))�֐�
	-------------------------------------
	�ߒl�Fint ���ڂ̒l
=========================================== */
int Item::GetInt() const
{
	if (m_eKind == Int|| m_eKind == List)
		return reinterpret_cast<const ItemValue*>(this)->m_uValue.nValue;
	/*else if (m_eKind == List)
		return static_cast<const ItemList*>(this)->m_nSelectNo;*/
	return 0;
}

/* ========================================
	�Q�b�^�[(����(float))�֐�
	-------------------------------------
	�ߒl�Ffloat ���ڂ̒l
=========================================== */
float Item::GetFloat() const
{
	if (m_eKind == Float)
		return reinterpret_cast<const ItemValue*>(this)->m_uValue.fValue;
	return 0.0f;
}

/* ========================================
	�Q�b�^�[(����(Vector))�֐�
	-------------------------------------
	�ߒl�FDirectX::XMFLOAT3 ���ڂ̒l
=========================================== */
DirectX::XMFLOAT3 Item::GetVector() const
{
	if (m_eKind == Vector)
		return reinterpret_cast<const ItemValue*>(this)->m_uValue.fVector;
	return DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

/* ========================================
	�Q�b�^�[(����(Vector2))�֐�
	-------------------------------------
	�ߒl�FDirectX::XMFLOAT2 ���ڂ̒l
=========================================== */
DirectX::XMFLOAT2 Item::GetVector2() const
{
	if (m_eKind == Vector2)
		return reinterpret_cast<const ItemValue*>(this)->m_uValue.fVector2;
	return DirectX::XMFLOAT2(0.0f, 0.0f);
}

/* ========================================
	�Q�b�^�[(����(Color))�֐�
	-------------------------------------
	�ߒl�FDirectX::XMFLOAT4 ���ڂ̒l
=========================================== */
DirectX::XMFLOAT4 Item::GetColor() const
{
	if (m_eKind == Color)
		return static_cast<const ItemValue*>(this)->m_uValue.fColor;
	return DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
}

/* ========================================
	�Q�b�^�[(����(Path))�֐�
	-------------------------------------
	�ߒl�Fconst char* ���ڂ̒l
=========================================== */
const char* Item::GetPath() const
{
	if (m_eKind == Path)
		return static_cast<const ItemValue*>(this)->m_uValue.cPath;
	return "";
}


/* ========================================
	���X�g�ԍ��擾�֐�
	-------------------------------------
	���e�F���X�g�����ږ��Ō������A���Ԗڂ����擾
	-------------------------------------
	�����Fconst char* name ���ږ�
	-------------------------------------
	�߂�l�Fint �ԍ�
=========================================== */
int Item::GetListNo(const char* name) const
{
	if (m_eKind == List)
	{
		const ItemList* pItemList = static_cast<const ItemList*>(this);
		// ���X�g������
		std::list<std::string>::const_iterator itr = std::find(pItemList->m_sItemList.begin(), pItemList->m_sItemList.end(), name);
		if (itr != pItemList->m_sItemList.end())
		{
			// ���������牽�Ԗڂ���Ԃ�
			return std::distance(pItemList->m_sItemList.begin(), itr);
		}
	}

	return -1;
}

/* ========================================
	���X�g�擾�֐�
	-------------------------------------
	�ߒl�Fstd::list<std::string> ���X�g
=========================================== */
std::list<std::string> Item::GetList() const
{
	if (m_eKind == List)
		return static_cast<const ItemList*>(this)->m_sItemList;
	return std::list<std::string>();
}

/* ========================================
	���X�g�e�L�X�g�擾�֐�
	-------------------------------------
	���e�F���X�g�̎w��Ԗڂ̃e�L�X�g���擾
	-------------------------------------
	�����Fint num �ԍ�
	-------------------------------------
	�߂�l�Fstd::string �e�L�X�g
=========================================== */
std::string Item::GetListText(int num) const
{
	if (m_eKind == List)
	{
		const ItemList* pItemList = static_cast<const ItemList*>(this);
		if (num < pItemList->m_sItemList.size())
		{
			auto itr = pItemList->m_sItemList.begin();
			std::advance(itr, num);	// �w��Ԗڂ܂Ői�߂�
			return *itr;
		}
	}
	return "";
}

/* ========================================
	�Q�b�^�[(����(Text))�֐�
	-------------------------------------
	�ߒl�Fconst char* ���ڂ̒l
=========================================== */
std::string Item::GetText() const
{
	if (m_eKind == Text)
		return static_cast<const ItemValue*>(this)->m_uValue.cText;
}

/* ========================================
	�Q�b�^�[(���X�g�\���s��)�֐�
	-------------------------------------
	�ߒl�Fint �\���s��
=========================================== */
int Item::GetDispLineNum() const
{
	if (m_eKind == List)
	{
		const ItemList* pItemList = static_cast<const ItemList*>(this);
		return pItemList->m_nDispLineNum;
	}
	return -1;
}

/* ========================================
	�Z�b�^�[(����(bool))�֐�
	-------------------------------------
	�����Fbool value �l
=========================================== */
void Item::SetBool(bool value)
{
	if (m_eKind == Bool)
		reinterpret_cast<ItemValue*>(this)->m_uValue.bValid = value;
}

/* ========================================
	�Z�b�^�[(����(int))�֐�
	-------------------------------------
	�����Fint value �l
=========================================== */
void Item::SetInt(int value)
{
	if (m_eKind == Int)
		reinterpret_cast<ItemValue*>(this)->m_uValue.nValue = value;
}

/* ========================================
	�Z�b�^�[(����(float))�֐�
	-------------------------------------
	�����Ffloat value �l
=========================================== */
void Item::SetFloat(float value)
{
	if (m_eKind == Float)
		reinterpret_cast<ItemValue*>(this)->m_uValue.fValue = value;
}

/* ========================================
	�Z�b�^�[(����(Vector))�֐�
	-------------------------------------
	�����FDirectX::XMFLOAT3 value �l
=========================================== */
void Item::SetVector(DirectX::XMFLOAT3 value)
{
	if (m_eKind == Vector)
		reinterpret_cast<ItemValue*>(this)->m_uValue.fVector = value;

}

/* ========================================
	�Z�b�^�[(����(Vector2))�֐�
	-------------------------------------
	�����FDirectX::XMFLOAT2 value �l
=========================================== */
void Item::SetVector2(DirectX::XMFLOAT2 value)
{
	if (m_eKind == Vector2)
		reinterpret_cast<ItemValue*>(this)->m_uValue.fVector2 = value;
}

/* ========================================
	�Z�b�^�[(����(Color))�֐�
	-------------------------------------
	�����FDirectX::XMFLOAT4 value �l
=========================================== */
void Item::SetColor(DirectX::XMFLOAT4 value)
{
	if (m_eKind == Color)
		reinterpret_cast<ItemValue*>(this)->m_uValue.fColor = value;
}

/* ========================================
	�Z�b�^�[(����(Path))�֐�
	-------------------------------------
	�����Fconst char* value �l
=========================================== */
void Item::SetPath(std::string value)
{
	if (m_eKind == Path)
	{
		std::strncpy(reinterpret_cast<ItemValue*>(this)->m_uValue.cPath, value.c_str(), value.size());
		reinterpret_cast<ItemValue*>(this)->m_uValue.cPath[value.size()] = '\0';	// �I�[����
	}
}

/* ========================================
	�Z�b�^�[(����(Text))�֐�
	-------------------------------------
	�����Fconst char* value �l
=========================================== */
void Item::SetText(std::string value)
{
	if (m_eKind == Text)
	{
		std::strncpy(reinterpret_cast<ItemValue*>(this)->m_uValue.cPath, value.c_str(), value.size());
		reinterpret_cast<ItemValue*>(this)->m_uValue.cPath[value.size()] = '\0';	// �I�[����
	}
}

/* ========================================
	�Z�b�^�[(���X�g�ԍ�)�֐�
	-------------------------------------
	���e�F���X�g�̑I��ԍ���ݒ�
	-------------------------------------
	�����Fint value �ԍ�
=========================================== */
void Item::SetListNo(int value)
{
	if (m_eKind == List)
		reinterpret_cast<ItemValue*>(this)->m_uValue.nValue = value;
}

/* ========================================
	�Z�b�^�[(�\���s��)�֐�
	-------------------------------------
	���e�F�\���s����ݒ�
	-------------------------------------
	�����Fint value �s��
=========================================== */
void Item::SetDispLineNum(int value)
{
	if (m_eKind == List)
		reinterpret_cast<ItemValue*>(this)->m_uValue.nValue = value;
}




}

#endif