/* ========================================
	CatRobotGame/
	------------------------------------
	�S�V�[���o�^�p�w�b�_
	------------------------------------
	�����F�V�[���̓o�^�������܂Ƃ߂Ă���
	------------------------------------
	RegisterAllScene.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "SceneManager.h"

#include "SceneStageSaves.h"

/* ========================================
	�V�[���o�^�֐�
	-------------------------------------
	���e�F�}�b�v�ɃV�[����o�^����
=========================================== */
void SceneManager::RegisterAllScene()
{
	REGISTER_SCENE(SceneStageSave_1);
	REGISTER_SCENE(SceneStageSave_2);
	REGISTER_SCENE(SceneStageSave_3);
	REGISTER_SCENE(SceneStageSave_4);
	REGISTER_SCENE(SceneStageSave_5);
}
