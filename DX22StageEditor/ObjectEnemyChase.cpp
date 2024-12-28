/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(敵キャラ追跡移動)用cpp
	------------------------------------
	ObjectEnemyChase.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectEnemyChase.h"
#include "ComponentEnemyMoveChase.h"
#include "ComponentEnemyState.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
=========================================== */
ObjectEnemyChase::ObjectEnemyChase(SceneBase* pScene)
	:ObjectEnemy(pScene)
	, m_pCompEnemyMoveChase(nullptr)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
=========================================== */
void ObjectEnemyChase::InitLocal()
{
	ObjectEnemy::InitLocal();

	// コンポーネント追加
	m_pCompEnemyMoveChase = AddComponent<ComponentEnemyMoveChase>();

	m_pCompEnemyState->SetMoveComponent(m_pCompEnemyMoveChase);
}


/* ========================================
	ローカルデータ出力関数
	-------------------------------------
	内容：オブジェクトのローカルデータをファイルに書き込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectEnemyChase::OutPutLocalData(std::ofstream& file)
{
	ObjectEnemy::OutPutLocalData(file);

	S_SaveDataEnemyChase data;

	// 移動速度
	data.fMoveSpeed = m_pCompEnemyMoveChase->GetMoveSpeed();
	// 追跡開始距離
	data.fChaseStartDist = m_pCompEnemyMoveChase->GetChaseStartDist();
	// 追跡限界距離
	data.fLimitDistSq = m_pCompEnemyMoveChase->GetLimitDistSq();

	// ファイルに書き込む
	file.write((char*)&data, sizeof(S_SaveDataEnemyChase));
}

/* ========================================
	ローカルデータ入力関数
	-------------------------------------
	内容：ファイルからオブジェクトのローカルデータを読み込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectEnemyChase::InputLocalData(std::ifstream& file)
{
	ObjectEnemy::InputLocalData(file);

	S_SaveDataEnemyChase data;

	// ファイルから読み込む
	file.read((char*)&data, sizeof(S_SaveDataEnemyChase));
	// 移動速度
	m_pCompEnemyMoveChase->SetMoveSpeed(data.fMoveSpeed);
	// 追跡開始距離
	m_pCompEnemyMoveChase->SetChaseStartDist(data.fChaseStartDist);
	// 追跡限界距離
	m_pCompEnemyMoveChase->SetLimitDistSq(data.fLimitDistSq);

}