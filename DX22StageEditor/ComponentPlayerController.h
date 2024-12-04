/* ========================================
	DX22Base/
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

// =============== 前方宣言 =======================
class ComponentTransform;
class ComponentRigidbody;

// =============== クラス定義 =====================
class ComponentPlayerController :
    public ComponentBase
{
public:
	enum E_MoveKey
	{
		Forward = 0,	// 前進
		Back,			// 後退
		Left,			// 左
		Right,			// 右
		Jump,			// ジャンプ
		RotateLeft,		// 左回転
		RotateRight,	// 右回転

		Max
	};

public:
	ComponentPlayerController(ObjectBase* pOwner);

	void Init() override;
	void Uninit() override;
	void Update() override;

	DEFINE_COMPONENT_TYPE	// コンポーネントの種類ID取得関数

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	DebugUI::Item* CreateKeyList(std::string sName, BYTE* moveKey);
#endif // _DEBUG
private:
	 ComponentTransform* m_pCompTran;	 
	 ComponentRigidbody* m_pCompRigidbody;	

	 float m_fMoveSpeed;	// 移動速度
	 float m_fRotateSpeed;	// 回転速度
	 float m_fJumpPower;	// ジャンプ力

	 BYTE m_MoveKey[E_MoveKey::Max];		// 移動キー

	 // 入力有効フラグ
	 bool m_bIsInputEnable;
};

