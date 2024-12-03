/* ========================================
	DX22Base/
	------------------------------------
	�f�o�b�O�E�B���h�E�pcpp
	------------------------------------
	DebugWindow.cpp
========================================== */
#ifdef _DEBUG	// �f�o�b�O���[�h�̂ݗL��

// =============== �C���N���[�h ===================
#include "DebugWindow.h"
#include "DebugMenu.h"

// =============== ���O��� ===================
namespace DebugUI {

// �ÓI�����o�ϐ��̏�����
Window	Window::dummy;

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
=========================================== */
Window::Window()
	: enable(false), name("")
{
}

/* ========================================
	�z�񉉎Z�q
	-------------------------------------
	���e�F�w�薼�̃A�C�e�����擾
	-------------------------------------
	�����Fconst char* name ���ږ�
	-------------------------------------
	�߂�l�FItem& �A�C�e��
=========================================== */
Item& Window::operator[](const char* name)
{
	auto it = FindItem(name);
	if (it == items.end())
		return *Item::dummy;
	return **it;
}

/* ========================================
	�A�C�e���ǉ�
	-------------------------------------
	���e�F�E�B���h�E�֕\�����ڂ�ǉ�
	-------------------------------------
	�����FItem* item �A�C�e��
=========================================== */
void Window::AddItem(Item* item)
{
	// �����̍��ڂƓ����̃A�C�e�����Ȃ����T��
	auto it = FindItem(item->GetName());
	if (it != items.end()) return;

	// �ۑ��f�[�^�̏ꍇ�́A�ǉ����ɕۑ��ς݂̃f�[�^�ŏ㏑��
	Menu::DataRead(name + "/", item);

	// �\�����ڂ̒ǉ�
	items.push_back(item);
}

/* ========================================
	�A�C�e���폜
	-------------------------------------
	���e�F�E�B���h�E����\�����ڂ��폜
	-------------------------------------
	�����Fconst char* name ���ږ�
=========================================== */
void Window::RemoveItem(const char* name)
{
	// �����̍��ڂƓ����̃A�C�e�����Ȃ����T��
	auto it = FindItem(name);
	if (it == items.end()) return;

	delete* it;			// ���������
	items.erase(it);	// ���X�g����폜
}


/* ========================================
	�A�C�e���S�폜
	-------------------------------------
	���e�F�E�B���h�E�̑S�\�����ڂ��폜
=========================================== */
void Window::Clear()
{
	auto it = items.begin();
	while (it != items.end())
	{
		delete* it;
		++it;
	}
	items.clear();
}
/*
* @brief �A�C�e���̒T��
*/
Items::iterator Window::FindItem(const char* name)
{
	return std::find_if(items.begin(), items.end(),
		[&name](const Item* obj) {
		return strcmp(obj->GetName(), name) == 0;
	});
}

/* ========================================
	�Z�b�^�[(�L���t���O)�֐�
	-------------------------------------
	�����Fbool enable �L���t���O
=========================================== */
void Window::SetEnable(bool enable)
{
	this->enable = enable;
}

}

#endif