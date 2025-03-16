/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(ジャンプバネ)用ヘッダ
	------------------------------------
	説明：Rigidbodyを持つオブジェクトが上に乗ると
		  ジャンプ力を与える
		  ObjectJumpPad
		  L m_pSpringModelObj
		   L m_pJumpSwitchObj
	------------------------------------
	ObjectJumpPad.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBase.h"

// =============== 前方宣言 =======================
class ComponentModelStatic;
class ComponentCollisionAABB;
class ObjectJumpSwitch;

// =============== クラス定義 ===================
class ObjectJumpPad :
    public ObjectBase
{
public:
	enum E_SpringState
	{
		Stay,		// そのまま
		Stretch,	// 伸びる
		Shrink,		// 縮む
	};
public:
	ObjectJumpPad(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	// ゲッター
	float GetStretchRate();

	// セッター
	void SetSpringState(E_SpringState eState);

	DEFINE_OBJECT_TYPE(ObjectJumpPad)
private:
	void InitSpringModel();
	void InitJumpSwitch();
private:
	ObjectBase*			m_pSpringModelObj;	// バネオブジェクト
	ObjectJumpSwitch*	m_pJumpSwitchObj;	// ジャンプスイッチ

	E_SpringState	m_eSpringState;			// バネの状態
	float			m_fStretchTime;			// 伸びる時間

	Vector3<float> m_vBasePos;				// ベースポジション

};

