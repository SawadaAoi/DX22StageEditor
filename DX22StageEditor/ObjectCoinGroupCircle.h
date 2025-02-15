/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(コイングループ(円)用ヘッダ
	------------------------------------
	説明：コインを円状に生成するクラス
		　子オブジェクトとしてコインを生成する
	------------------------------------
	ObjectCoinGroupCircle.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBase.h"

// =============== 前方宣言 =======================
class ObjectCoin;

// =============== クラスの定義 ===================
class ObjectCoinGroupCircle :
	public ObjectBase
{
public:
	// セーブデータ
	struct S_SaveData
	{
		int		nCoinNum;		// コイン数
		float	fCoinRadius;	// コイン円半径
		bool	bUseGravity;	// 重力使用フラグ
	};
public:
	ObjectCoinGroupCircle(SceneBase* pScene);
	void UpdateLocal() override;

	// ゲッター
	bool GetUseGravity();

	// セッター
	void SetUseGravity(bool bUseGravity);

	// データ保存
	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectCoinGroupCircle)
#ifdef _DEBUG
		void DebugLocal(DebugUI::Window& window);
#endif // DEBUG
private:
	void CreateCoin();
	void ResetCoin();
private:
	std::vector<ObjectCoin*> m_pCoinList;	// コインリスト
	int		m_nCoinNum;						// コイン数
	float	m_fCircleRadius;				// コイン円半径

	bool m_bIsInitCreate;	// 生成フラグ
	bool m_bUseGravity;		// 重力使用フラグ
};

