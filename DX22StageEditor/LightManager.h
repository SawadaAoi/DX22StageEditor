/* ========================================
	DX22Base/
	------------------------------------
	���C�g�Ǘ��p�w�b�_
	------------------------------------
	����:���C�g���Ǘ�����N���X(�K�X�؂�ւ���)
	------------------------------------
	LightManager.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectLight.h"	// ���C�g�I�u�W�F�N�g
#include <array>	// std::array

// =============== �O���錾 =======================
class SceneBase;
class ObjectBase;

// =============== �萔��` =====================
#define LIGHT_MNG_INST LightManager::GetInstance()	// instance�擾
#define GET_LIGHT LightManager::GetInstance().GetLight()	// ���C�g�擾

const int MAX_LIGHT_NUM = 10;	// ���C�g�ő吔

class LightManager
{
public:
	static LightManager& GetInstance();	// �C���X�^���X�擾
	void Init(SceneBase* pScene);		// �V�[���J�n���Ƀ��C�g������

	void AddLight(ObjectLight* pLight);		// ���C�g�ǉ�
	void RemoveLight(ObjectLight* pLight);	// ���C�g�폜

	// �Q�b�^�[
	ObjectLight* GetLight(int num);
	std::vector<ObjectLight*> GetLightList();
	int GetLightNum();

private:
	LightManager();	// �R���X�g���N�^	

	std::vector<ObjectLight*> m_pLightList;	// ���C�g���X�g

	SceneBase* m_pScene;	// �V�[���N���X�̃|�C���^
};

