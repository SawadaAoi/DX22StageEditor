/* ========================================
	CatRobotGame/
	------------------------------------
	敵キャラ移動コンポーネント(追跡)用ヘッダ
	------------------------------------
	説明：敵キャラの追跡移動処理クラス
		　指定範囲内に近づいたらプレイヤーを追跡する
	------------------------------------
	ComponentEnemyMoveChase.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ComponentEnemyMoveBase.h"
#include <memory>
#include "Quaternion.h"

// =============== 前方宣言 =======================
class ShapeLine;

// =============== クラス定義 =====================
class ComponentEnemyMoveChase :
    public ComponentEnemyMoveBase
{
public:
	ComponentEnemyMoveChase(ObjectBase* pOwner);
	void Init() override;
	void Update() override;
	void Draw() override;

	// ゲッター
	float GetChaseStartDist() const;		// 追跡開始距離を取得する
	bool GetDispDistLine() const;			// 距離ライン表示フラグを取得する
	float GetLimitDistSq() const;			// 追跡限界距離を取得する

	// セッター
	void SetChaseStartDist(float fDist);	// 追跡開始距離を設定する
	void SetDispDistLine(bool bDisp);		// 距離ライン表示フラグを設定する
	void SetLimitDistSq(float fDist);		// 追跡限界距離を設定する

	DEFINE_COMPONENT_TYPE
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	void Move();
	void ChaseTarget();	// プレイヤーを追跡する
	void BackToStartPos();	// 移動開始座標に戻る

	// プレイヤーとの距離ラインを描画する
	void DrawPlayerDistLine();
private:
	ObjectBase* m_pTargetObj;	// 追跡対象オブジェクト

	float m_fChaseStartDist;	// 追跡開始距離
	float m_fLimitDistSq;		// 追跡限界距離

	bool m_bDispDistLine;					// 距離ライン表示フラグ
	std::unique_ptr<ShapeLine> m_pDistLine;	// 距離ライン描画用

	Vector3<float>	m_vStartPos;	// 移動開始座標
	Quaternion		m_qStartRot;	// 移動開始角度
};

