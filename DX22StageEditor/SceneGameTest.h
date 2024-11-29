#pragma once
#include "SceneBase.h"
class SceneGameTest :
	public SceneBase
{
public:
	void InitLocal() override;
	void UpdateLocal() override;

private:
	ObjectBase* m_pPlayer;
	ObjectBase* m_StaticPlane;

	class ObjectCamera* m_pCamera1;
};