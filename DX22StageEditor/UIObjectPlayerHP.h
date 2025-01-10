/* ========================================
	CatRobotGame/
	------------------------------------
	UIオブジェクト(プレイヤーHP)用ヘッダ
	------------------------------------
	説明：プレイヤーのHPを表示するUIオブジェクト
	------------------------------------
	UIObjectPlayerHP.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "UIObjectBase.h"
#include <vector>

// =============== 前方宣言 =====================
class ObjectPlayer;

// =============== クラス定義 ===================
class UIObjectPlayerHP :
	public UIObjectBase
{
public:
	UIObjectPlayerHP(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	DEFINE_UI_OBJECT_TYPE(UIObjectPlayerHP)
private:
	void CreateHP();
private:
	std::vector<UIObjectBase*> m_pUIObjectHP;
	ObjectPlayer* m_pPlayer;
};

