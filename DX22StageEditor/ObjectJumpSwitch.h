/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(ジャンプバネスイッチ)用ヘッダ
	------------------------------------
	説明：ジャンプバネのジャンプ力が変化する当たり判定用
		　※ジャンプバネの天面に配置
	------------------------------------
	ObjectJumpSwitch.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBase.h"

// =============== 前方宣言 =======================
class ComponentCollisionAABB;
class ObjectJumpPad;

// =============== クラス定義 ===================
class ObjectJumpSwitch :
    public ObjectBase
{
public:
	ObjectJumpSwitch(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;
	void OnCollisionStay(ObjectBase* pHit) override;
	void OnCollisionExit(ObjectBase* pHit) override;
private:
	ObjectJumpPad*				m_pJumpPad;			// ジャンプパッド(親)
	ComponentCollisionAABB*		m_pCompColAABB;		// 衝突判定
	float						m_fObjJumpPower;	// ジャンプ力

	
};

