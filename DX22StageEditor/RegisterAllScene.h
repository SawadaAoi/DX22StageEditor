/* ========================================
	CatRobotGame/
	------------------------------------
	全シーン登録用ヘッダ
	------------------------------------
	説明：シーンの登録処理をまとめておく
	------------------------------------
	RegisterAllScene.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "SceneManager.h"

#include "SceneStageSaves.h"

/* ========================================
	シーン登録関数
	-------------------------------------
	内容：マップにシーンを登録する
=========================================== */
void SceneManager::RegisterAllScene()
{
	REGISTER_SCENE(SceneStageSave_1);
	REGISTER_SCENE(SceneStageSave_2);
	REGISTER_SCENE(SceneStageSave_3);
	REGISTER_SCENE(SceneStageSave_4);
	REGISTER_SCENE(SceneStageSave_5);
}
