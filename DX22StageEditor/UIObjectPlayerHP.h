/* ========================================
	CatRobotGame/
	------------------------------------
	UI�I�u�W�F�N�g(�v���C���[HP)�p�w�b�_
	------------------------------------
	�����F�v���C���[��HP��\������UI�I�u�W�F�N�g
	------------------------------------
	UIObjectPlayerHP.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "UIObjectBase.h"
#include <vector>

// =============== �O���錾 =====================
class ObjectPlayer;

// =============== �N���X��` ===================
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

