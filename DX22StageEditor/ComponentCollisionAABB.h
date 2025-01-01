/* ========================================
	DX22Base/
	------------------------------------
	�Փ˔���R���|�[�l���g(AABB)�p�w�b�_
	------------------------------------
	�����F���[���h�̎��ɕ��s�ȏՓ˔���`��
		�@���I�u�W�F�N�g�̌������ς���Ă�
			�Փ˔��藧���̂̌����͕ς��Ȃ�
	------------------------------------
	ComponentCollisionAABB.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentCollisionBase.h"

// =============== �O���錾 =======================
class ComponentCollisionSphere;

// =============== �N���X��` =====================
class ComponentCollisionAABB :
    public ComponentCollisionBase
{
public:
	ComponentCollisionAABB(ObjectBase* pOwner);

	void Init() override;
	void Update() override;

	// �Փ˔���
	bool CheckCollision(ComponentCollisionBase* otherCol) override;
	bool CheckCollisionAABB(ComponentCollisionAABB* otherCol);		// AABB��AABB
	bool CheckCollisionOBB(ComponentCollisionOBB* otherCol);		// AABB��OBB
	bool CheckCollisionSphere(ComponentCollisionSphere* otherCol);	// AABB�Ƌ�

	// �Q�b�^�[
	Vector3<float> GetMin();
	Vector3<float> GetMax();

	// �Z�b�^�[
	void SetMin(Vector3<float> vMin);
	void SetMax(Vector3<float> vMax);


	DEFINE_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�

#ifdef _DEBUG
		void Debug(DebugUI::Window& window) override;
#endif // _DEBUG

private:
	Vector3<float> m_vMin;		// �ŏ����W(���A���A��O)
	Vector3<float> m_vMax;		// �ő���W(�E�A��A��)
};

