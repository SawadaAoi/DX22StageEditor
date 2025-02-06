/* ========================================
	CatRobotGame/
	------------------------------------
	�R���|�[�l���g(�ړ������G�t�F�N�g)�p�w�b�_
	------------------------------------
	�����F�ړ����̉��G�t�F�N�g���Ǘ�����
	------------------------------------
	ComponentSmokeEffect.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ComponentBase.h"

// =============== �O���錾 =======================
class SceneBase;

// =============== �N���X��` =====================
class ComponentSmokeEffect :
    public ComponentBase
{
public:
	ComponentSmokeEffect(ObjectBase* pOwner);
	void Init() override;
	void Update() override;

	// �Z�b�^�[
	void SetMoveDir(Vector3<float> vMoveDir);
	void SetCreatePosDist(Vector3<float> vCreatePosDist);
	void SetCreateInterval(float fCreateInterval);
	void SetLifeTime(float fLifeTime);
	void SetScale(float fScaleMin, float fScaleMax);
	void SetPosAdjust(float fPosAdjustMin, float fPosAdjustMax);

	DEFINE_COMPONENT_TYPE
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	void CreateSmoke();
	void DeleteSmoke();
private:
	SceneBase* m_pScene;

	std::vector<std::pair<ObjectBase*, float>> m_Smokes;
	Vector3<float> m_vMoveDir;	// �v���C���[�̈ړ�����

	float m_fCreateInterval;	// �����Ԋu
	float m_fCreateItvTimeCnt;	// �����Ԋu���ԃJ�E���g

	Vector3<float> m_vCreatePosDist;	// �����ʒu�̒���
	float m_fLifeTime;					// ���̎���

	float m_fScaleMin;	// ���̍ŏ��X�P�[��
	float m_fScaleMax;	// ���̍ő�X�P�[��

	float m_fPosAdjustMin;	// �ʒu�����̍ŏ��l
	float m_fPosAdjustMax;	// �ʒu�����̍ő�l

};

