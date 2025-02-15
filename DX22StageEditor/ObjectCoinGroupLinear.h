/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�R�C���O���[�v(����)�p�w�b�_
	------------------------------------
	�����F�R�C���𒼐���ɐ�������N���X
		�@�q�I�u�W�F�N�g�Ƃ��ăR�C���𐶐�����
	------------------------------------
	ObjectCoinGroupLinear.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBase.h"

// =============== �O���錾 =======================
class ObjectCoin;

// =============== �N���X�̒�` ===================
class ObjectCoinGroupLinear :
    public ObjectBase
{
public:
	// �Z�[�u�f�[�^
	struct S_SaveData
	{
		int		nCoinNum;		// �R�C����
		float	fCoinSpace;		// �R�C���Ԋu
		bool	bUseGravity;	// �d�͎g�p�t���O
	};
public:
	ObjectCoinGroupLinear(SceneBase* pScene);
	void UpdateLocal() override;

	// �Q�b�^�[
	bool GetUseGravity();

	// �Z�b�^�[
	void SetUseGravity(bool bUseGravity);

	// �f�[�^�ۑ�
	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectCoinGroupLinear)
#ifdef _DEBUG
	void DebugLocal(DebugUI::Window& window);
#endif // DEBUG
private:
	void CreateCoin();
	void ResetCoin();
private:
	std::vector<ObjectCoin*> m_pCoinList;	// �R�C�����X�g
	int		m_nCoinNum;						// �R�C����
	float	m_fCoinSpace;					// �R�C���Ԋu

	bool m_bIsInitCreate;	// �����t���O
	bool m_bUseGravity;		// �d�͎g�p�t���O
};

