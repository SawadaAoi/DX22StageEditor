/* ========================================
	CatRobotGame/
	------------------------------------
	プレイヤー開始地点用ヘッダ
	------------------------------------
	説明：ステージのプレイヤー開始地点
		　※エディターでプレイヤーの初期位置を設定しやすくするため
	------------------------------------
	ObjectPlayerStart.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBase.h"

// =============== クラス定義 ===================
class ObjectPlayerStart :
    public ObjectBase
{
public:
	ObjectPlayerStart(SceneBase* pScene);

	void InitLocal() override;
	void UpdateLocal() override;

	// ゲッター
	bool GetIsPlayerStart() const;

	// セッター
	void SetIsPlayerStart(bool bIsPlayerStart);

	DEFINE_OBJECT_TYPE(ObjectPlayerStart)

#ifdef _DEBUG
		void DebugLocal(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	bool m_bIsPlayerStart; // true = スタート / false = 未スタート
};

