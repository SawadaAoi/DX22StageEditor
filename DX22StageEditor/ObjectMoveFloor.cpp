/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(移動床)用cpp
	------------------------------------
	ObjectMoveFloor.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectMoveFloor.h"
#include "ComponentGimmickMoveLinear.h"
#include "ComponentTransform.h"
#include "ComponentRigidbody.h"
#include "ComponentGroundBox.h"
#include "ComponentCollisionBase.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
========================================== */
ObjectMoveFloor::ObjectMoveFloor(SceneBase* pScene)
	: ObjectBlock(pScene)
	, m_pCompMoveLinear(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_pCompGroundBox(nullptr)
{
	SetTag(E_ObjectTag::Ground);	// タグの設定
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化
========================================== */
void ObjectMoveFloor::InitLocal()
{
	m_eColType = E_COL_TYPE::COL_OBB;	// 衝突判定形状
	ObjectBlock::InitLocal();			// 親クラスの初期化関数を呼ぶ

	m_pCompMoveLinear = AddComponent<ComponentGimmickMoveLinear>();	// 直線移動コンポーネント
	m_pCompGroundBox = AddComponent<ComponentGroundBox>();			// 地面判定コンポーネント

	m_pCompColBase->SetDynamic(true);	// 動的オブジェクトに設定
}

/* ========================================
	衝突判定(開始時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(開始時)
	-------------------------------------
	引数：衝突相手オブジェクト
=========================================== */
void ObjectMoveFloor::OnCollisionEnter(ObjectBase* pHit)
{
	// プレイヤーか敵キャラが乗ったら
	if (pHit->GetTag() == E_ObjectTag::Player ||
		pHit->GetTag() == E_ObjectTag::Enemy)
	{
		AddChildObject(pHit);	// 子オブジェクトに追加(一緒に移動)
	}
}

/* ========================================
	衝突判定(終了時)関数
	-------------------------------------
	内容：他オブジェクトとの衝突判定(終了時)
	-------------------------------------
	引数：衝突相手オブジェクト
=========================================== */
void ObjectMoveFloor::OnCollisionExit(ObjectBase* pHit)
{
	// プレイヤーか敵キャラが離れたら
	if (pHit->GetTag() == E_ObjectTag::Player ||
		pHit->GetTag() == E_ObjectTag::Enemy)
	{
		RemoveChildObject(pHit);	// 子オブジェクトから削除
	}
}

/* ========================================
	コピー関数(個別処理)
	-------------------------------------
	内容：オブジェクト個別のコピー処理
	-------------------------------------
	引数1：コピーされたオブジェクト
=========================================== */
void ObjectMoveFloor::CopyLocal(ObjectBase* pObject)
{
	ObjectBlock::CopyLocal(pObject);	// 親クラスのコピー関数を呼ぶ

	ComponentGimmickMoveLinear* pCopyObjCompMoveLinear = pObject->GetComponent<ComponentGimmickMoveLinear>();	// 移動コンポーネント

	// 移動速度
	pCopyObjCompMoveLinear->SetMoveSpeed(m_pCompMoveLinear->GetMoveSpeed());
	// 移動座標数
	int nWayPointNum = m_pCompMoveLinear->GetWayPoints().size();
	// 逆順フラグ
	pCopyObjCompMoveLinear->SetIsReverse(m_pCompMoveLinear->GetIsReverse());

	// 移動座標
	for (int i = 0; i < nWayPointNum; i++)
	{
		// 移動座標追加
		pCopyObjCompMoveLinear->AddWayPoint(m_pCompMoveLinear->GetWayPoints()[i]);
	}
}

/* ========================================
	ローカルデータ出力関数
	-------------------------------------
	内容：オブジェクトのローカルデータをファイルに書き込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectMoveFloor::OutPutLocalData(std::ofstream& file)
{
	ObjectBlock::OutPutLocalData(file);	// 親クラスのローカルデータ出力関数を呼ぶ

	S_SaveData data;

	// 移動速度
	data.fMoveSpeed = m_pCompMoveLinear->GetMoveSpeed();
	// 移動座標数
	data.nWayPointNum = m_pCompMoveLinear->GetWayPoints().size();
	// 逆順フラグ
	data.bIsReverse = m_pCompMoveLinear->GetIsReverse();

	// ファイルに書き込む
	file.write((char*)&data, sizeof(S_SaveData));

	// 移動座標
	for (int i = 0; i < m_pCompMoveLinear->GetWayPoints().size(); i++)
	{
		// ファイルに書き込む
		file.write((char*)&m_pCompMoveLinear->GetWayPoints()[i], sizeof(Vector3<float>));
	}
}

/* ========================================
	ローカルデータ入力関数
	-------------------------------------
	内容：ファイルからオブジェクトのローカルデータを読み込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectMoveFloor::InputLocalData(std::ifstream& file)
{
	ObjectBlock::InputLocalData(file);	// 親クラスのローカルデータ入力関数を呼ぶ

	S_SaveData data;

	// ファイルから読み込む
	file.read((char*)&data, sizeof(S_SaveData));

	// 移動速度
	m_pCompMoveLinear->SetMoveSpeed(data.fMoveSpeed);
	// 移動座標数
	int nWayPointNum = data.nWayPointNum;
	// 逆順フラグ
	m_pCompMoveLinear->SetIsReverse(data.bIsReverse);

	// 移動座標
	for (int i = 0; i < data.nWayPointNum; i++)
	{
		Vector3<float> wayPoint;

		// ファイルから読み込む
		file.read((char*)&wayPoint, sizeof(Vector3<float>));

		// 移動座標追加
		m_pCompMoveLinear->AddWayPoint(wayPoint);
	}
}

/* ========================================
	ゲッター(移動床コンポーネント)関数
	-------------------------------------
	戻値: コンポーネントポインタ
=========================================== */
ComponentGimmickMoveLinear* ObjectMoveFloor::GetCompMoveLinear()
{
	return m_pCompMoveLinear;
}
