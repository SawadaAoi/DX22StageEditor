/* ========================================
	DX22Base/
	------------------------------------
	�f�o�b�O�E�B���h�E�p�w�b�_
	------------------------------------
	�����Fimgui���g�p�����f�o�b�O�E�B���h�E
		�@�쐬������DebugMenu�ōs��
	------------------------------------
	DebugWindow.h
========================================== */
#ifdef _DEBUG	// �f�o�b�O���[�h�̂ݗL��
#pragma once

// =============== �C���N���[�h ===================
#include "DebugItem.h"
#include <string>


// =============== ���O��� ===================
namespace DebugUI {

class Window
{
	friend class Menu;
public:
	Window();
	Item& operator[](const char* name);

	// �A�C�e���ǉ�
	void AddItem(Item* item);
	// �A�C�e���폜
	void RemoveItem(const char* name);
	// �S�A�C�e���폜
	void Clear();

private:
	// �A�C�e���̒T��
	Items::iterator FindItem(const char* name);

public:
	static Window	dummy;	// �w��E�B���h�E���擾�ł��Ȃ������Ƃ��̃_�~�[
private:
	bool			enable;	// �\���t���O
	std::string		name;	// �E�B���h�E��
	Items			items;	// �\������
};
}
#endif
