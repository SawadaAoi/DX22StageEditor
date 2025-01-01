#pragma once
#include "ComponentCollisionBase.h"
class ComponentCollisionPlane :
    public ComponentCollisionBase
{
public:
	ComponentCollisionPlane(ObjectBase* pOwner);

	void Init() override;
	void Update() override;

	// �Փ˔���
	bool CheckCollision(ComponentCollisionBase* otherCol) override;
	bool CheckCollisionAABB(ComponentCollisionAABB* otherCol);		// AABB��AABB

	// �Q�b�^�[
	Vector3<float> GetMin();
	Vector3<float> GetMax();

	// �Z�b�^�[
	void SetMin(Vector3<float> vMin);
	void SetMax(Vector3<float> vMax);

private:
	Vector3<float> m_vMin;		// �ŏ����W(���A���A��O)
	Vector3<float> m_vMax;		// �ő���W(�E�A��A��)
};

