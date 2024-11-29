/* ========================================
	DX22Base/
	------------------------------------
	地面接触判定コンポーネント用ヘッダ
	------------------------------------
	説明：下方向にレイを飛ばし、地面との接触判定を行う
	------------------------------------
	ComponentGroundRaycast.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ComponentBase.h"
#include "Vector3.h"
#include <memory>
#include <vector>
#include "ShapeLine.h"


// =============== 前方宣言 =======================
class ComponentTransform;
class ComponentGround;

// =============== クラス定義 =====================
class ComponentGroundRaycast :
    public ComponentBase
{
public:
	ComponentGroundRaycast(ObjectBase* pOwner);
	~ComponentGroundRaycast() {};

	void Init() override;
	void Update() override;
	void Draw() override;

	// ゲッター
	Vector3<float> GetHitPos();				// レイ地面接触座標
	bool GetHitFlg();						// レイ地面接触判定フラグ
	// セッター
	void SetRayLength(float fLength);						// レイの長さを設定
	void SetRayDirection(const Vector3<float>& vDir);		// レイの方向を設定
	void SetStartPosOffset(const Vector3<float>& vOffset);	// レイの始点のオフセットを設定
	void SetDrawLine(bool bDraw);							// レイ描画フラグを設定

	DEFINE_COMPONENT_TYPE	// コンポーネントの種類ID取得関数

#ifdef _DEBUG
		void Debug(DebugUI::Window& window) override;
#endif // _DEBUG

private:
	void CheckGround();																		// 地面との接触判定
	bool CheckHit(const Vector3<float>& vPlanePos, const Vector3<float> vPlaneNormal);		// レイが地面に当たっているかどうかを判定
	bool CheckOnGround(ComponentGround* pPlaneGround);										// 接触座標が地面の上にあるかどうかを判定

private:
	ComponentTransform* m_pOwnerTransform;	// 座標、回転、スケール

	Vector3<float>		m_vStartPos;		// レイの始点
	Vector3<float>		m_vStartPosOffset;	// レイの始点のオフセット
	Vector3<float>		m_vDirection;		// レイの方向
	float				m_fRayLength;		// レイの長さ

	bool									m_bIsDispRayLine;			// レイ描画フラグ
	std::unique_ptr<ShapeLine>				m_pRayLine;					// レイ描画線
	bool									m_bIsDispTriPointToHitPos;	// 三角頂点→地面接触座標までの線表示フラグ
	std::vector<std::unique_ptr<ShapeLine>> m_pLines_TriPointToHitPos;	// 地面三角頂点から地面接触座標までの線

	Vector3<float>	m_vHitPos;		// レイ地面接触座標
	bool			m_bIsHit;		// レイ地面接触判定フラグ

#ifdef _DEBUG
	std::string		m_sDispIsHit;	// 地面接触表示
#endif // _DEBUG

};

