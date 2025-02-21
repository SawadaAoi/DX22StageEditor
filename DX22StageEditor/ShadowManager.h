/* ========================================
	CatRobotGame/
	------------------------------------
	�ۉe�Ǘ��p�w�b�_
	------------------------------------
	����:�I�u�W�F�N�g�̉��ɕ\������ۉe���Ǘ�����
	------------------------------------
	ShadowManager.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentShadow.h"
#include <array>	// std::array
#include <vector>	// std::vector

// =============== �O���錾 =======================
class SceneBase;
class ObjectBase;

// =============== �萔��` =====================
#define SHADOW_MNG_INST ShadowManager::GetInstance()	// instance�擾
#define GET_SHADOW ShadowManager::GetInstance().GetLight()	// ���C�g�擾


// =============== �N���X��` =====================
class ShadowManager
{
public:
	static ShadowManager& GetInstance();	// �C���X�^���X�擾
	void Init(SceneBase* pScene);		// �V�[���J�n���Ƀ��C�g������

	void AddShadow(ComponentShadow* pShadow);		// ���C�g�ǉ�
	void RemoveShadow(ComponentShadow* pShadow);	// ���C�g�폜

	// �Q�b�^�[
	ComponentShadow* GetShadow(int num);
	std::vector<ComponentShadow*> GetShadowList();
	int GetShadowNum();

private:
	ShadowManager();	// �R���X�g���N�^	
private:
	std::vector<ComponentShadow*> m_pShadowList;	// �e���X�g
	SceneBase* m_pScene;							// �V�[���N���X�̃|�C���^
};

