/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�����G�t�F�N�g)�p�w�b�_
	------------------------------------
	�����F�G�L�������j�󂳂ꂽ�ۂ̔����G�t�F�N�g
	------------------------------------
	ObjectExplosion.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectBase.h"

// =============== �O���錾 =======================
class ComponentBillboardAnime;

// =============== �N���X��` =====================
class ObjectExplosion :
    public ObjectBase
{
public:
    ObjectExplosion(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	// �Q�b�^�[
	ComponentBillboardAnime* GetCompBillboardAnime();

	DEFINE_OBJECT_TYPE(ObjectExplosion)
private:
	ComponentBillboardAnime* m_pCompBillboardAnime;
};

