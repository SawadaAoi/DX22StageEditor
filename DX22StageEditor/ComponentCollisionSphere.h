/* ========================================
	DX22Base/
	------------------------------------
	�Փ˔���R���|�[�l���g(����)�p�w�b�_
	------------------------------------
	�����F���̂̏Փ˔�����s���R���|�[�l���g
	------------------------------------
	ComponentCollisionSphere.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentCollisionBase.h"

// =============== �O���錾 =======================
class ComponentCollisionAABB;

// =============== �N���X��` =====================
class ComponentCollisionSphere :
    public ComponentCollisionBase
{
public:
	ComponentCollisionSphere(ObjectBase* pOwner);

	void Init() override;
	void Update() override;

	// �Փ˔���
	bool CheckCollision(ComponentCollisionBase* otherCol) override;

	// �Q�b�^�[
	float GetRadius();

	// �Z�b�^�[
	void SetRadius(float fRadius);

	DEFINE_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	bool CheckCollisionSphere(ComponentCollisionSphere* otherCol);	// �����m�̏Փ˔���
private:
	float m_fRadius;			// ���a
};

