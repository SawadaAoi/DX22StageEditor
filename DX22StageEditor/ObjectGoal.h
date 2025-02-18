/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�S�[��)�p�w�b�_
	------------------------------------
	�����F�X�e�[�W�̃S�[��
		�@�v���C���[���ڐG����ƃN���A
	------------------------------------
	ObjectGoal.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectBase.h"

// =============== �O���錾 =======================
class ComponentModelStatic;

// =============== �N���X��` =====================
class ObjectGoal :
    public ObjectBase
{
public:
	// �Z�[�u�f�[�^
	struct S_SaveData
	{
		Vector3<float> vBasePos;	// ���W
	};
public:
	ObjectGoal(SceneBase* pScene);

	void InitLocal();
	void UpdateLocal();

	void OnCollisionEnter(ObjectBase* pHit) override;

	// �Q�b�^�[
	bool GetIsGoal();

	// �Z�b�^�[
	void SetIsGoal(bool bIsGoal);	// �f�[�^�ۑ�

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectGoal)
#ifdef _DEBUG
	void DebugLocal(DebugUI::Window& window);
#endif // DEBUG
private:
	ComponentModelStatic* m_pModel;	// ���f���R���|�[�l���g

	bool			m_bIsGoal;			// true: �S�[�� / false: ���S�[��
	float			m_fAnimeTimeCnt;	// �A�j���[�V�������ԃJ�E���g
	Vector3<float>	m_vBasePos;			// ����W

	bool m_bOnFloatAnime;				// �㉺�A�j���[�V�����t���O
};

