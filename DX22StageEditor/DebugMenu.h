/* ========================================
	DX22Base/
	------------------------------------
	�f�o�b�O���j���[�p�w�b�_
	------------------------------------
	�����Fimgui���g�p�����f�o�b�O���j���[
	------------------------------------
	DebugMenu.h
========================================== */

/**
 * 2024.04.08
 *	imgui�����A�e�L�X�g�t�@�C���ǂݍ��݁A�o�C���h�ǉ�
 * 2024.04.09
 *	Bind�������R�[���o�b�N�őΉ��A�f�[�^�̓ǂݏ�����ǉ��A���X�g�ǉ��A�V�[���̐؂�ւ�
 * 2024.04.10
 *	���f���̐؂�ւ��A�V�[���̐؂�ւ�
 * 2024.07.23
 * �@�\�����e���f�[�^����ǂݎ��̂ł͂Ȃ��A�v���O������Őݒ�ł���悤�ɕύX
 * �@GameObject��ύX���āAUnity���̃C���X�y�N�^�[�\�����s���悤�ɕύX
 */
#ifdef _DEBUG	// �f�o�b�O���[�h�̂ݗL��

#pragma once

 // =============== �C���N���[�h ===================
#include <string>
#include <vector>

#include "DebugWindow.h"	// �f�o�b�O�p�E�B���h�E
#include "DebugWindowSetting.h"

// =============== ���O��� ===================
namespace DebugUI {


class Menu
{
	friend class Window;
public:
	// �ۑ��f�[�^
	struct SaveData
	{
		Item::Kind kind;	// �\�����ڂ̎��
		std::string path;	// �t�@�C���p�X
		std::string value;	// �l
	};
private:
	Menu();
	~Menu();

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	// �E�B���h�E����
	static Window& Create(const char* name);
	// �E�B���h�E�擾
	static Window& Get(const char* name);
private:
	static void DrawImgui(Item* item);
	static void DataWrite(std::string& data, std::string path, Item* item);
	static void DataRead(std::string path, Item* item);

private:
	static inline std::vector<Window>		m_WindowList;		// �\���E�B���h�E�ꗗ
	static inline std::vector<SaveData>		m_SaveDataList;		// �ۑ��f�[�^
};



} // namespace debug
#endif // _DEBUG
