/* ========================================
	DX22/Dropin
	------------------------------------
	���f���f�[�^�Ǘ��p�w�b�_
	------------------------------------
	����:���f���f�[�^���܂Ƃ߂ēǂݍ���ŊǗ�����
	------------------------------------
	ModelDataManager.h
========================================== */
#pragma once


// =============== �C���N���[�h ===================
#include "ModelStatic.h"
#include "Shader.h"
#include <vector>
#include <unordered_map>
#include <memory>

// =============== �萔��` =======================
#define MODEL_STATIC_MNG_INST	ModelStaticManager::GetInstance()	// �C���X�^���X�擾

#define GET_MODEL_STATIC		ModelStaticManager::GetInstance().GetModelData	// ���f���擾
#define MODEL_KEY				ModelStaticManager::E_MODEL_KEY					// ���f���L�[

class ModelStaticManager
{
public:
	// ���f���f�[�^���
	enum E_MODEL_KEY
	{
		TEST_MODEL,
		PLAYER_CAT,
		SKYBOX_SUNNY,
		GOAL_NIKUKYU,
		MAX,
	};

	const std::unordered_map < ModelStaticManager::E_MODEL_KEY, std::string > MODEL_FBX_PATH
	{
		{MODEL_KEY::TEST_MODEL,		"Assets/Model/Static/Test/TestObject.fbx"},
		{MODEL_KEY::PLAYER_CAT,		"Assets/Model/Static/Player/Cat.fbx"},
		{MODEL_KEY::SKYBOX_SUNNY,	"Assets/Model/Static/Skybox/Sunny/SkyBox_Sunny.fbx"},
		{MODEL_KEY::GOAL_NIKUKYU,	"Assets/Model/Static/Goal/Goal_Nikukyu.fbx"},
	};

	// �e���f���f�[�^�傫���w��
	const std::unordered_map < ModelStaticManager::E_MODEL_KEY, float > MODEL_FBX_SCALE
	{
		{MODEL_KEY::TEST_MODEL,		1.0f},
		{MODEL_KEY::PLAYER_CAT,		0.6f},
		{MODEL_KEY::SKYBOX_SUNNY,	1.0f},
		{MODEL_KEY::GOAL_NIKUKYU,	0.4f},
	};

public:
	static ModelStaticManager& GetInstance();
	void Init();

	// �Q�b�^�[
	ModelStatic* GetModelData(E_MODEL_KEY e_GetKey);
	E_MODEL_KEY GetModelKey(ModelStatic* pModelData);

	std::unordered_map <E_MODEL_KEY, std::shared_ptr<ModelStatic>> GetModelDatas();
	std::unordered_map <E_MODEL_KEY, std::string> GetModelPaths();

private:
	ModelStaticManager();
private:
	std::unordered_map <E_MODEL_KEY, std::shared_ptr<ModelStatic>> m_apModelDatas;	// ���f���f�[�^���i�[���Ă���
	bool bIsInit;


};


