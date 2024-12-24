/* ========================================
	DX22Base/
	------------------------------------
	コンポーネント更新順用ヘッダ
	------------------------------------
	説明：コンポーネントの更新順序を定義
	------------------------------------
	ComponentOrder.h
========================================== */
#pragma once

// ===列挙定義===========
enum E_UpdateOrder
{
	OrderTransform,			// トランスフォーム	
	OrderGroundRayCast,		// 地面(レイキャスト)	
	OrderRigidBody,			// 剛体
	OrderCollisionBase,		// 当たり判定基底
	OrderCollisionSphere,	// 当たり判定(球)
	OrderCollisionAABB,		// 当たり判定(AABB)
	OrderCollisionOBB,		// 当たり判定(OBB)
	OrderGround,			// 地面
	OrderModelStatic,		// モデル(静的)
	OrderModelAnim,			// モデル(アニメーション)
	OrderPlayerControl,		// プレイヤー操作
	OrderEnemyState,		// 敵状態
	OrderEnemyMove,			// 敵移動
	OrderBillboard,			// ビルボード
	OrderGeometry,			// ジオメトリ
	OrderCameraBase,		// カメラ基底
	OrderCameraDebug,		// カメラ(デバッグ)
	OrderLightBase,			// ライト基底
};