/* ========================================
	CatRobotGame/
	------------------------------------
	リスポーン用ヘッダ
	------------------------------------
	説明：衝突したらリスポーンするオブジェクト
	------------------------------------
	ObjectRespawn.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBase.h"

// =============== 前方宣言 =======================
class ComponentCollisionAABB;

// =============== クラス =======================
class ObjectRespawn :
    public ObjectBase
{
public:
	// リスポーン種類
	enum E_RespawnType
	{
		RT_RELOAD_SCENE,	// シーンを再読み込み
		RT_POSITION,		// 決まった位置からリスポーン
	};


	// セーブデータ
	struct S_SaveData
	{
		E_RespawnType	eRespawnType;		// リスポーン種類
		Vector3<float>	vRespawnPosition;	// リスポーン位置
	};

public:
	ObjectRespawn(SceneBase* pScene);

	void InitLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;

	// データ保存
	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	// ゲッター
	E_RespawnType GetRespawnType() const;
	Vector3<float> GetRespawnPosition() const;

	// セッター
	void SetRespawnType(E_RespawnType type);
	void SetRespawnPosition(const Vector3<float>& pos);

	DEFINE_OBJECT_TYPE(ObjectRespawn)

#ifdef _DEBUG
	void DebugLocal(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	E_RespawnType	m_RespawnType;			// リスポーン種類
	Vector3<float>	m_RespawnPosition;		// リスポーン位置


	ComponentCollisionAABB* m_pCompColAABB;	// 当たり判定
};

