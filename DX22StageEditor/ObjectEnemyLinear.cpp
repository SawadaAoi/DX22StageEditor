/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(敵キャラ線移動)用cpp
	------------------------------------
	ObjectEnemyLinear.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectEnemyLinear.h"
#include "ComponentEnemyMoveLinear.h"
#include "ComponentEnemyState.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
=========================================== */
ObjectEnemyLinear::ObjectEnemyLinear(SceneBase* pScene)
	:ObjectEnemy(pScene)
	, m_pCompEnemyMoveLinear(nullptr)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
=========================================== */
void ObjectEnemyLinear::InitLocal()
{
	ObjectEnemy::InitLocal();

	// コンポーネント追加
	m_pCompEnemyMoveLinear = AddComponent<ComponentEnemyMoveLinear>();

	m_pCompEnemyState->SetMoveComponent(m_pCompEnemyMoveLinear);
}

/* ========================================
	ローカルデータ出力関数
	-------------------------------------
	内容：オブジェクトのローカルデータをファイルに書き込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectEnemyLinear::OutPutLocalData(std::ofstream& file)
{
	ObjectEnemy::OutPutLocalData(file);

	S_SaveDataEnemyLinear data;

	// 移動速度
	data.fMoveSpeed = m_pCompEnemyMoveLinear->GetMoveSpeed();
	// 移動座標数
	data.nWayPointNum = m_pCompEnemyMoveLinear->GetWayPoints().size();
	// 逆順フラグ
	data.bIsReverse = m_pCompEnemyMoveLinear->GetIsReverse();

	// ファイルに書き込む
	file.write((char*)&data, sizeof(S_SaveDataEnemyLinear));

	// 移動座標
	for (int i = 0; i < m_pCompEnemyMoveLinear->GetWayPoints().size(); i++)
	{
		// ファイルに書き込む
		file.write((char*)&m_pCompEnemyMoveLinear->GetWayPoints()[i], sizeof(Vector3<float>));
	}
}

/* ========================================
	ローカルデータ入力関数
	-------------------------------------
	内容：ファイルからオブジェクトのローカルデータを読み込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectEnemyLinear::InputLocalData(std::ifstream& file)
{
	ObjectEnemy::InputLocalData(file);

	S_SaveDataEnemyLinear data;

	// ファイルから読み込む
	file.read((char*)&data, sizeof(S_SaveDataEnemyLinear));

	// 移動速度
	m_pCompEnemyMoveLinear->SetMoveSpeed(data.fMoveSpeed);
	// 移動座標数
	int nWayPointNum = data.nWayPointNum;
	// 逆順フラグ
	m_pCompEnemyMoveLinear->SetIsReverse(data.bIsReverse);

	// 移動座標
	for (int i = 0; i < data.nWayPointNum; i++)
	{
		Vector3<float> wayPoint;

		// ファイルから読み込む
		file.read((char*)&wayPoint, sizeof(Vector3<float>));

		// 移動座標追加
		m_pCompEnemyMoveLinear->AddWayPoint(wayPoint);
	}

}