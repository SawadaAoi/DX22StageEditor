/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(ファイヤーバー)用ヘッダ
	------------------------------------
	説明：プレイヤーにダメージを与えるオブジェクト
	------------------------------------
	ObjectFireBar.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBase.h"


// =============== 定数定義 =======================
class ObjectBlockGroundAABB;

// =============== クラス定義 ===================
class ObjectFireBar :
    public ObjectBase
{
public:
	struct S_SaveData
	{
		int		nFireBallNum;	// 火の玉の数
		float	fRotateSpeed;	// 回転速度
	};
public:
	ObjectFireBar(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	void CopyLocal(ObjectBase* pObject) override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	// セッター
	void SetFireBallNum(int nFireBallNum);
	void SetRotateSpeed(float fRotateSpeed);
	

	DEFINE_OBJECT_TYPE(ObjectFireBar)
private:
	void InitCenterBlock();
	void InitFireBall();
private:
	ObjectBlockGroundAABB*		m_pCenterBlock;		// 中心ブロック
	ObjectBase*					m_pFireBallParent;	// 火の玉親(回転用)
	std::vector<ObjectBase*>	m_pFireBall;		// 火の玉

	int m_nFireBallNum;	// 火の玉の数
	float m_fRotateSpeed;	// 回転速度
};

