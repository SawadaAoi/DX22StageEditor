/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(敵キャラランダム移動)用cpp
	------------------------------------
	ObjectEnemyRandom.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectEnemyRandom.h"
#include "ComponentEnemyMoveRandom.h"
#include "ComponentEnemyState.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
========================================== */
ObjectEnemyRandom::ObjectEnemyRandom(SceneBase* pScene)
	:ObjectEnemy(pScene)
	, m_pCompEnemyMoveRandom(nullptr)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================== */
void ObjectEnemyRandom::InitLocal()
{
	ObjectEnemy::InitLocal();

	// コンポーネント追加
	m_pCompEnemyMoveRandom = AddComponent<ComponentEnemyMoveRandom>();

	m_pCompEnemyState->SetMoveComponent(m_pCompEnemyMoveRandom);
}

/* ========================================
	ローカルデータ出力関数
	-------------------------------------
	内容：オブジェクトのローカルデータをファイルに書き込む
	-------------------------------------
	引数1：ファイル
========================================== */
void ObjectEnemyRandom::OutPutLocalData(std::ofstream& file)
{
	ObjectEnemy::OutPutLocalData(file);

	S_SaveDataEnemyRandom data;

	// 移動速度
	data.fMoveSpeed = m_pCompEnemyMoveRandom->GetMoveSpeed();

	// ターゲット切り替え時間
	data.fTargetSwitchTime = m_pCompEnemyMoveRandom->GetTargetSwitchTime();

	// 移動範囲中心座標
	data.vMoveLimitCenter = m_pCompEnemyMoveRandom->GetMoveLimitRectCenter();

	// 移動範囲
	std::vector<Vector3<float>> vMoveLimit = m_pCompEnemyMoveRandom->GetMoveLimitRect();
	data.vMoveLimitRF = vMoveLimit[0];
	data.vMoveLimitLF = vMoveLimit[1];
	data.vMoveLimitLB = vMoveLimit[2];
	data.vMoveLimitRB = vMoveLimit[3];

	// ファイルに書き込む
	file.write((char*)&data, sizeof(S_SaveDataEnemyRandom));
}

/* ========================================
	ローカルデータ入力関数
	-------------------------------------
	内容：オブジェクトのローカルデータをファイルから読み込む
	-------------------------------------
	引数1：ファイル
========================================== */
void ObjectEnemyRandom::InputLocalData(std::ifstream& file)
{
	ObjectEnemy::InputLocalData(file);

	S_SaveDataEnemyRandom data;

	// ファイルから読み込む
	file.read((char*)&data, sizeof(data));

	// 移動速度
	m_pCompEnemyMoveRandom->SetMoveSpeed(data.fMoveSpeed);

	// ターゲット切り替え時間
	m_pCompEnemyMoveRandom->SetTargetSwitchTime(data.fTargetSwitchTime);

	// 移動範囲中心座標
	m_pCompEnemyMoveRandom->SetMoveLimitRectCenter(data.vMoveLimitCenter);

	// 移動範囲
	std::vector<Vector3<float>> vMoveLimit;
	vMoveLimit.push_back(data.vMoveLimitRF);
	vMoveLimit.push_back(data.vMoveLimitLF);
	vMoveLimit.push_back(data.vMoveLimitLB);
	vMoveLimit.push_back(data.vMoveLimitRB);

	m_pCompEnemyMoveRandom->SetMoveLimitRect(vMoveLimit);

	m_pCompEnemyMoveRandom->InitTargetPos();

}
