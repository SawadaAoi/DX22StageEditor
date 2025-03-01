/* ========================================
	DX22Base/
	------------------------------------
	オブジェクト(破壊可能ブロック)用ヘッダ
	------------------------------------
	説明：弾に当たると壊れるブロック
		　AABB、地面判定有り
	------------------------------------
	ObjectBlockBreakable.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBlock.h"

// =============== クラス定義 ===================
class ObjectBlockBreakable :
    public ObjectBlock
{
public:
	struct S_SaveData
	{
		int nHp;	// HP
	};
public:
	ObjectBlockBreakable(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;

	void CopyLocal(ObjectBase* pObject) override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	// セッター
	void SetHp(int nHp);

	DEFINE_OBJECT_TYPE(ObjectBlockBreakable)
#ifdef _DEBUG
	void DebugLocal(DebugUI::Window& window);
#endif // DEBUG
private:
	int m_nHp;		// HP
	int m_nHpOld;	// HP(前回)
};

