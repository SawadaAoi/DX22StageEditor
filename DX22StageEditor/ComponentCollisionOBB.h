/* ========================================
	DX22Base/
	------------------------------------
	�Փ˔���R���|�[�l���g(OBB)�p�w�b�_
	------------------------------------
	�����F�I�u�W�F�N�g�̉�]�ɍ��킹��
		�@��]����Փ˔���`��
	------------------------------------
	ComponentCollisionOBB.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentCollisionBase.h"

// =============== �N���X��` =====================
class ComponentCollisionOBB :
    public ComponentCollisionBase
{
public:
	// ���̎��(T_OBB��vAxis, fExtent�����₷�����邽��)
	enum E_Axis
	{
		AXIS_X = 0,
		AXIS_Y,
		AXIS_Z,
	};

	// OBB�\����(�Փ˔���Ɏg���l���܂Ƃ߂�)
	struct T_OBB
	{
		Vector3<float>	vCenter;		// ���S���W
		Vector3<float>	vAxis[3];		// ��
		float			fExtent[3];		// �e���̒���(Vector3����Ȃ��̂�for���Ŏg������)
		std::string		sName;			// �Փˑ���I�u�W�F�N�g��

		T_OBB()
		{
			vCenter = Vector3<float>(0.0f, 0.0f, 0.0f);
			vAxis[AXIS_X] = Vector3<float>(1.0f, 0.0f, 0.0f);
			vAxis[AXIS_Y] = Vector3<float>(0.0f, 1.0f, 0.0f);
			vAxis[AXIS_Z] = Vector3<float>(0.0f, 0.0f, 1.0f);
			fExtent[AXIS_X] = 0.0f;
			fExtent[AXIS_Y] = 0.0f;
			fExtent[AXIS_Z] = 0.0f;
			sName = "None";
		}
	};

	// �ŏ��ړ��x�N�g���\����(�߂荞�ݖ߂��p)
	struct T_MTV
	{
		Vector3<float>	vAxis;		// �Փˎ�
		float			fOverlap;	// �d�Ȃ��
		bool			bIsCol;		// �Փ˂��Ă��邩
		bool			bIsTrigger;	// true:�g���K�[����p(���蔲������)
		std::string		sName;		// �Փˑ���I�u�W�F�N�g��

		T_MTV()
		{
			vAxis		= Vector3<float>(0.0f, 0.0f, 0.0f);
			fOverlap	= 10000.0f;
			bIsCol		= false;
			bIsTrigger	= false;
			sName		= "None";
		}
	};

public:
	ComponentCollisionOBB(ObjectBase* pOwner);
	void Init() override;
	void Update() override;

	// �Փ˔���
	bool CheckCollision(ComponentCollisionBase* otherCol) override;
	bool CheckCollisionOBB(ComponentCollisionBase* otherCol);		// OBB��OBB
	T_OBB CreateOBB(ComponentCollisionBase* Col);					// OBB�\���̍쐬

	// �Q�b�^�[
	std::vector<T_MTV> GetMtvs();

	// �Z�b�^�[
	void SetMtvs(std::vector<T_MTV> mtvs);

	DEFINE_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG

private:
	bool CheckLocalSeparateAxis(T_OBB tObb1, T_OBB tObb2, Vector3<float> vDis);	// OBB���m�̃��[�J������������
	bool CheckCrossSeparateAxis(T_OBB tObb1, T_OBB tObb2, Vector3<float> vDis);	// OBB���m�̌�������������

	float GetProjectionLength(Vector3<float> vAxis, T_OBB tObb);	// �ˉe���̒����擾


private:
	// �ŏ��ړ��x�N�g���\���̔z��
	// ���Փˎ��̂߂荞�݉����Ɏg�p
	// ���Փˊm�F����I�u�W�F�N�g����
	std::vector<T_MTV>	m_tMtvs;	
#ifdef _DEBUG
	int m_nSelectMTV;	// �I�𒆂̍ŏ��ړ��x�N�g��
#endif // _DEBUG
};

