/* ========================================
	CatRobotGame/
	------------------------------------
	���X�|�[���p�w�b�_
	------------------------------------
	�����F�Փ˂����烊�X�|�[������I�u�W�F�N�g
	------------------------------------
	ObjectRespawn.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBase.h"

// =============== �O���錾 =======================
class ComponentCollisionAABB;

// =============== �N���X =======================
class ObjectRespawn :
    public ObjectBase
{
public:
	// ���X�|�[�����
	enum E_RespawnType
	{
		RT_RELOAD_SCENE,	// �V�[�����ēǂݍ���
		RT_POSITION,		// ���܂����ʒu���烊�X�|�[��
	};


	// �Z�[�u�f�[�^
	struct S_SaveData
	{
		E_RespawnType	eRespawnType;		// ���X�|�[�����
		Vector3<float>	vRespawnPosition;	// ���X�|�[���ʒu
	};

public:
	ObjectRespawn(SceneBase* pScene);

	void InitLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;

	// �f�[�^�ۑ�
	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	// �Q�b�^�[
	E_RespawnType GetRespawnType() const;
	Vector3<float> GetRespawnPosition() const;

	// �Z�b�^�[
	void SetRespawnType(E_RespawnType type);
	void SetRespawnPosition(const Vector3<float>& pos);

	DEFINE_OBJECT_TYPE(ObjectRespawn)

#ifdef _DEBUG
	void DebugLocal(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	E_RespawnType	m_RespawnType;			// ���X�|�[�����
	Vector3<float>	m_RespawnPosition;		// ���X�|�[���ʒu


	ComponentCollisionAABB* m_pCompColAABB;	// �����蔻��
};

