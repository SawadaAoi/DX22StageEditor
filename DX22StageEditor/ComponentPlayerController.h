/* ========================================
	CatRobotGame/
	------------------------------------
	プレイヤー操作コンポーネント用ヘッダ
	------------------------------------
	説明：プレイヤーを操作する為のキー入力
		　の処理を管理する
	------------------------------------
	ComponentPlayerController.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ComponentBase.h"
#include "Windows.h"
#include <unordered_map>

// =============== 前方宣言 =======================
class ComponentTransform;
class ComponentRigidbody;
class ObjectCameraPlayer;

// =============== クラス定義 =====================
class ComponentPlayerController :
    public ComponentBase
{
public:
	enum E_MoveKey
	{
		FORWARD = 0,	// 前進
		BACK,			// 後退
		LEFT,			// 左
		RIGHT,			// 右
		JUMP,			// ジャンプ
		SHOT,			// ショット
		Max
	};

public:
	ComponentPlayerController(ObjectBase* pOwner);

	void Init() override;
	void Uninit() override;
	void Update() override;

	// ゲッター
	float GetMoveSpeed();
	float GetRotateSpeed();
	bool GetUseJump();

	// セッター
	void SetMoveSpeed(float fMoveSpeed);
	void SetRotateSpeed(float fRotateSpeed);
	void SetUseJump(bool bUseJump);

	DEFINE_COMPONENT_TYPE	// コンポーネントの種類ID取得関数

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	DebugUI::Item* CreateKeyList(std::string sName, BYTE* moveKey);
#endif // _DEBUG
private:
	void Move();	// 移動処理
	void Jump();	// ジャンプ処理
	void Shot();	// 弾発射処理

	Vector3<float> MoveKeybord();	// キーボード入力処理
	Vector3<float> MoveController();	// コントローラー入力処理

	// 移動方向に回転する
	void RotateToMoveDir(Vector3<float> moveDir);
private:
	 ComponentTransform* m_pCompTran;	 
	 ComponentRigidbody* m_pCompRigidbody;	
	 ObjectCameraPlayer* m_pObjCamera;

	 float m_fMoveSpeed;	// 移動速度
	 float m_fRotateSpeed;	// 回転速度
	 float m_fJumpPower;	// ジャンプ力

	 BYTE m_MoveKey[E_MoveKey::Max];		// 移動キー
	 bool m_bUseJump;	// ジャンプ中フラグ
	 
	 // 入力有効フラグ
	 bool m_bIsInputEnable;


};

