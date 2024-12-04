/* ========================================
	DX22Base/
	------------------------------------
	�f�o�b�O�\�����ڗp�w�b�_
	------------------------------------
	�����F�f�o�b�O���j���[�̕\������(�`�F�b�N�{�b�N�X�A���l���́A�{�^���Ȃ�)
		�@���Ǘ�����N���X
	------------------------------------
	DebugItem.h
========================================== */
#ifdef _DEBUG	// �f�o�b�O���[�h�̂ݗL��
#pragma once

// =============== �C���N���[�h ===================
#include <functional>
#include <list>
#include <DirectXMath.h>
#include <string>

// =============== ���O��� ===================
namespace DebugUI {

	const int MAX_ITEM_TEXT = 260;	// ���ږ��̍ő啶����

// �\������
class Item
{
	friend class Menu;
public:
	// �R�[���o�b�N�֐�(�{�^��or�l�ύX��)
	// �����FisWrite	�������ݎ���(�{�^���̏ꍇ�͉�����)
	// �����Farg		����(�������񂾒l�A�{�^���̏ꍇ��nullptr)
	using Callback = std::function<void(bool isWrite, void* arg)>;

	// �R�[���o�b�N�֐�(�ꗗ�\��)
	// �����Farg	�I��	
	using ConstCallback = std::function<void(const void* arg)>;

	// ���ڂ��ێ�����l
	union Value {
		bool				bValid;
		int					nValue;
		float				fValue;
		DirectX::XMFLOAT3	fVector;
		DirectX::XMFLOAT2	fVector2;
		DirectX::XMFLOAT4	fColor;
		char				cPath[MAX_ITEM_TEXT];
		char				cText[MAX_ITEM_TEXT];


	};

	// ���ڎ��
	enum Kind {
		Label,		// ���ڂ݂̂̕\��
		Text,		// �e�L�X�g����
		Bool,		// �`�F�b�N�{�b�N�X
		Int,		// ��������
		Float,		// ��������
		Vector,		// �x�N�^�[����
		Vector2,	// �x�N�^�[����(2D)
		Color,		// �F����
		Path,		// �t�@�C���p�X�̎w��
		Command,	// �{�^��
		Group,		// �\�����ڂ��܂Ƃ߂�
		List,		// �ꗗ�\��
	};
protected:
	Item();
public:
	virtual ~Item();

	// ���ڐ���
	static Item* CreateValue(const char* name, Kind kind, bool isSave = false, bool isSameLine = false);
	static Item* CreateBind(const char* name, Kind kind, void* ptr, bool isSave = false, bool isSameLine = false);
	static Item* CreateCallBack(const char* name, Kind kind, Callback func, bool isSave = false, bool isSameLine = false);
	static Item* CreateGroup(const char* name);
	static Item* CreateList(const char* name, ConstCallback func = nullptr, bool isSave = false, bool bIsDropDown = false, bool isSameLine = false);

public:

	// �O���[�v�̐ݒ�
	void AddGroupItem(Item* item);
	Item& operator[](const char* name);

	// �ꗗ�\���̐ݒ�
	void AddListItem(const char* name);
	void InsertListItem(const char* name, int num);
	void RemoveListItem(const char* name);
	void RemoveListItem(const char* name, std::string exceptText);
	void RemoveListItemAll();

	// �����񂩂獀�ڂ̎�ʂ��擾
	static Kind StrToKind(std::string str);
	// ���ڂ̎�ʂ��當������擾
	static std::string KindToStr(Kind kind);


	// �Q�b�^�[
	const char* GetName() const;
	Kind GetKind() const;

	bool GetBool() const;
	int GetInt() const;
	float GetFloat() const;
	DirectX::XMFLOAT3 GetVector() const;
	DirectX::XMFLOAT2 GetVector2() const;
	DirectX::XMFLOAT4 GetColor() const;
	const char* GetPath() const;
	int GetListNo(const char* name) const;
	std::list<std::string> GetList() const;
	std::string GetListText(int num) const;
	std::string GetText() const;

	// �Z�b�^�[
	void SetBool(bool value);
	void SetInt(int value);
	void SetFloat(float value);
	void SetVector(DirectX::XMFLOAT3 value);
	void SetVector2(DirectX::XMFLOAT2 value);
	void SetColor(DirectX::XMFLOAT4 value);
	void SetPath(std::string value);
	void SetText(std::string value);
	void SetListNo(int value);

public:
	static inline Item* dummy;			// �w��f�[�^���擾�ł��Ȃ������ꍇ�̃_�~�[
	bool				m_bSave;		// �ۑ��t���O
	bool				m_bSameLine;	// �����s�ɕ\�����邩
private:
	std::string m_sName;	// ���ږ�
	Kind		m_eKind;	// ���ڎ��
	Value		m_uValue;	// ���͒l


};
using Items = std::list<Item*>;

// �\������(�P��)
class ItemValue : public Item
{
public:
	ItemValue() /*: m_uValue{} , m_bSave(false)*/ {}
	~ItemValue() {}
};


// �\������(�ϐ��R�Â�)
class ItemBind : public Item
{
public:
	ItemBind() : m_pBindVar(nullptr) {}
	~ItemBind() {}
public:
	void* m_pBindVar;	// �R�Â��ϐ�
};


// �\������(�R�[���o�b�N)
class ItemCallback : public Item
{
public:
	ItemCallback() : /*m_uValue{},*/ m_CallbackFunc(nullptr) {}
	~ItemCallback() {}
public:
	Callback	m_CallbackFunc;	// �R�[���o�b�N�֐�
};


// �\������(�O���[�v)
class ItemGroup : public Item
{
public:
	ItemGroup() {}
	~ItemGroup()
	{
		auto it = m_pItemGroup.begin();
		while (it != m_pItemGroup.end())
		{
			delete (*it);
			++it;
		}
	}
public:
	Items m_pItemGroup;	// �O���[�v���̍���
};



// �\������(���X�g)
class ItemList : public Item
{
public:
	ItemList() : m_sItemList(), m_bIsDropDown(false), m_CallbackFunc(nullptr) { m_sItemList.clear(); }
	~ItemList() {}
public:
	std::list<std::string>	m_sItemList;		// �A�C�e���ꗗ
	ConstCallback			m_CallbackFunc;		// ���ڑI�����̃R�[���o�b�N
	bool					m_bIsDropDown;			// �R���{�{�b�N�X��
};
}
#endif