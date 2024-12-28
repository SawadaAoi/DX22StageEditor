/* ========================================
	CatRobotGame/
	------------------------------------
	敵キャラ移動コンポーネント(ランダム)用ヘッダ
	------------------------------------
	説明：敵キャラのランダム移動処理クラス
		　一定間隔でランダムな座標を生成し、移動する
	------------------------------------
	ComponentEnemyMoveRandom.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ComponentEnemyMoveBase.h"
#include <vector>
#include <memory>

// =============== 前方宣言 =======================
class ShapeLine;
class ShapeSphere;

// =============== クラス定義 =====================
class ComponentEnemyMoveRandom :
    public ComponentEnemyMoveBase
{
public:
	ComponentEnemyMoveRandom(ObjectBase* pOwner);
	void Init() override;
	void Update() override;
	void Draw() override;

	// ゲッター
	Vector3<float> GetTargetPos() const;
	float GetTargetSwitchTime() const;
	std::vector<Vector3<float>>& GetMoveLimitRect();

	// セッター
	void SetTargetSwitchTime(float fTime);
	void SetMoveLimitRect(const std::vector<Vector3<float>>& vtRect);	// 移動範囲を設定する
	void SetMoveLimitRectSquare(float fSize);				// 移動範囲を正方形に設定する場合の一辺の長さ
	void SetMoveLimitRectXZ(const Vector2<float>& vSize);	// 移動範囲をXZ平面で設定する場合のサイズ
	void SetMoveLimitRectPosY(float fPosY);					// 移動範囲座標Yを設定する
	void SetDispMoveLimitRect(bool bDisp);					// 移動範囲表示フラグを設定する

	DEFINE_COMPONENT_TYPE

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	void Move();
	void SwitchTargetPos();

	Vector3<float> GetRandomPoint(
		const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3, const Vector3<float>& v4);
	Vector3<float> GetRandomPointInTriangle(
		const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3);

	float GetTriangleArea(const Vector3<float>& P1, const Vector3<float>& P2, const Vector3<float>& P3);
private:
	Vector3<float>	m_vTargetPos;			// 目的座標
	float			m_fTargetSwitchTime;	// 目的座標切り替え時間
	float			m_fTargetSwitchCnt;		// 目的座標切り替えカウント

	std::vector<Vector3<float>> m_vtMoveLimitRect;	// 移動範囲座標(四角形)
	float m_fMoveLimitRectPosY;						// 移動範囲座標Y

	bool m_bDispMoveLimitRect;						// 移動範囲表示フラグ
	std::unique_ptr<ShapeLine>	m_pRectLine;		// 移動範囲描画用
	std::unique_ptr<ShapeSphere> m_pTargetSphere;	// 目的座標描画用
};

