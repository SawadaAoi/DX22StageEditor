/* ========================================
	CatRobotGame/
	------------------------------------
	プレイヤー操作コンポーネント用cpp
	------------------------------------
	ComponentPlayerController.cpp
========================================== */

// =============== インクルード =====================
#include "ComponentPlayerController.h"
#include "ObjectBase.h"		// 所持オブジェクトの取得
#include "ObjectBulletPlayer.h"
#include "ObjectCameraPlayer.h"	// 移動方向計算用

#include "ComponentRigidbody.h"	// リジッドボディコンポーネント
#include "ComponentTransform.h"	// トランスフォームコンポーネント
#include "ComponentModelAnime.h"	// アニメーションモデルコンポーネント

#include "Input.h"
#include "SceneManager.h"
#include "unordered_map"
#include "ModelAnimeManager.h"

#include <cmath>

// =============== 定数定義 =======================
// ComponentRigidbody::E_ForceModeを省略
using E_ForceMode = ComponentRigidbody::E_ForceMode;

const float DEFAULT_MOVE_SPEED		= 5.0f;		// 移動速度
const float DEFAULT_ROTATE_SPEED	= 15.0f;	// 回転速度
const float DEFAULT_JUMP_POWER		= 5.0f;		// ジャンプ力


// アニメーション
const float ANIME_SPEED_IDLE = 1.0f;	// 待機アニメーション速度
const float ANIME_SPEED_WALK = 1.0f;	// 移動アニメーション速度
const float ANIME_SPEED_SHOT = 3.0f;	// ショットアニメーション速度

const float ANIME_BLEND_TIME = 0.2f;	// アニメーションブレンド時間
const float ANIME_SHOT_OK_RATIO = 0.5f;		// ショット可能なアニメ再生率

// 入力キーマップ
const std::unordered_map<std::string, BYTE> KEY_MAP
{
	{ "SPACE"	, VK_SPACE	 },
	{ "UP"		, VK_UP		 },
	{ "DOWN"	, VK_DOWN	 },
	{ "LEFT"	, VK_LEFT	 },
	{ "RIGHT"	, VK_RIGHT	 },
	{ "CONTROL" , VK_CONTROL },
	{ "ENTER"	, VK_RETURN  },
	{ "A", 'A' },
	{ "B", 'B' },
	{ "C", 'C' },
	{ "D", 'D' },
	{ "E", 'E' },
	{ "F", 'F' },
	{ "G", 'G' },
	{ "H", 'H' },
	{ "I", 'I' },
	{ "J", 'J' },
	{ "K", 'K' },
	{ "L", 'L' },
	{ "M", 'M' },
	{ "N", 'N' },
	{ "O", 'O' },
	{ "P", 'P' },
	{ "Q", 'Q' },
	{ "R", 'R' },
	{ "S", 'S' },
	{ "T", 'T' },
	{ "U", 'U' },
	{ "V", 'V' },
	{ "W", 'W' },
	{ "X", 'X' },
	{ "Y", 'Y' },
	{ "Z", 'Z' },

};

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有者オブジェクト
=========================================== */
ComponentPlayerController::ComponentPlayerController(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderPlayerControl)
	, m_pObjCamera(nullptr)
	, m_pCompTran(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_pCompModelAnime(nullptr)
	, m_fMoveSpeed(DEFAULT_MOVE_SPEED)
	, m_fRotateSpeed(DEFAULT_ROTATE_SPEED)
	, m_fJumpPower(DEFAULT_JUMP_POWER)
	, m_MoveKey{ 'W', 'S', 'A', 'D', VK_SPACE , VK_RETURN }
	, m_bIsInputEnable(true)
	, m_bUseJump(true)
	, m_bShot(false)

{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================== */
void ComponentPlayerController::Init()
{
	m_pCompTran = m_pOwnerObj->GetComponent<ComponentTransform>();	// トランスフォームを取得
	m_pCompRigidbody = m_pOwnerObj->GetComponent<ComponentRigidbody>();	// リジッドボディを取得
	m_pCompModelAnime = m_pOwnerObj->GetComponent<ComponentModelAnime>();	// アニメーションモデルを取得

	m_pObjCamera = SceneManager::GetScene()->GetSceneObject<ObjectCameraPlayer>();	// カメラオブジェクトを取得

}

/* ========================================
	終了関数
	-------------------------------------
	内容：終了処理
========================================== */
void ComponentPlayerController::Uninit()
{
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================== */
void ComponentPlayerController::Update()
{
	// カメラオブジェクトが取得できていない場合は取得
	if (!m_pObjCamera)
	{
		m_pObjCamera = SceneManager::GetScene()->GetSceneObject<ObjectCameraPlayer>();
		return;
	}

	if (!m_pCompModelAnime)
	{
		m_pCompModelAnime = m_pOwnerObj->GetComponent<ComponentModelAnime>();
		return;
	}

	// 入力無効
	if (!m_bIsInputEnable) return;	
	// シフトキーを押している間は移動なし
	if (Input::IsKeyPress(VK_SHIFT)) return;

	Move();		// 移動
	Jump();		// ジャンプ
	Shot();		// ショット

}



/* ========================================
	移動関数
	-------------------------------------
	内容：移動処理
========================================== */
void ComponentPlayerController::Move()
{
	Vector3<float> moveDir = Vector3<float>::Zero();	// 移動方向

	// キーボード入力
	moveDir = MoveKeybord();
	
	m_pCompRigidbody->AddForce(moveDir * m_fMoveSpeed);

	RotateToMoveDir(moveDir);	// 移動方向に回転

	// 移動アニメーション
	MoveAnime(moveDir);
}

/* ========================================
	ジャンプ関数
	-------------------------------------
	内容：ジャンプ処理
========================================== */
void ComponentPlayerController::Jump()
{
	if (!m_bUseJump) return;	// ジャンプ有効フラグが無効なら処理しない

	// ジャンプ
	if (Input::IsKeyTrigger(m_MoveKey[E_MoveKey::JUMP]))
	{
		m_pCompRigidbody->AddForce(Vector3<float>::Up() * m_fJumpPower, E_ForceMode::IMPULSE);
		m_pCompTran->TranslateY(1.1f);	// 少し浮かせる(地面判定の位置修正を考慮して)
	}
}

/* ========================================
	弾発射関数
	-------------------------------------
	内容：進行方向に弾を発射する
		　キー入力で、アニメ再生
		  アニメの適切なタイミングで弾を発射
========================================== */
void ComponentPlayerController::Shot()
{
	// ショットキーが押されたら　かつ　ショットアニメが再生されていない場合
	if (Input::IsKeyTrigger(m_MoveKey[E_MoveKey::SHOT]) &&
		!m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_PLAYER::PLYR_SHOT))
	{
		m_pCompModelAnime->PlayAnimeBlend(ANIME_KEY_PLAYER::PLYR_SHOT, ANIME_BLEND_TIME, false, ANIME_SPEED_SHOT);
		m_bShot = true;		// 連射防止
	}

	// ショットアニメが再生されている場合 かつ ショットアニメが手を上げるところまで再生されたら
	if (m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_PLAYER::PLYR_SHOT)
		&& m_pCompModelAnime->GetAnimePlaybackRatio(ANIME_KEY_PLAYER::PLYR_SHOT) >= ANIME_SHOT_OK_RATIO
		&& m_bShot)	// 既に弾を発射していないか
	{

		ObjectBullet* pBullet = m_pOwnerObj->GetOwnerScene()->AddSceneObject<ObjectBulletPlayer>("PlayerBullet");
		pBullet->GetTransform()->SetPosition(m_pCompTran->GetPosition());
		pBullet->GetTransform()->SetRotation(m_pCompTran->GetRotation());
		m_bShot = false;	// 連射防止
	}
}

/* ========================================
	キーボード入力関数
	-------------------------------------
	内容：キーボード入力処理
	-------------------------------------
	戻値：Vector3<float>		移動方向
=========================================== */
Vector3<float> ComponentPlayerController::MoveKeybord()
{
	Vector3<float> moveDir = Vector3<float>::Zero();	// 移動方向

	Vector3<float> cameraForward = m_pObjCamera->GetTransform()->GetForwardVector();	// カメラの前方ベクトル
	Vector3<float> cameraRight = m_pObjCamera->GetTransform()->GetRightVector();	// カメラの右方向ベクトル

	// 上下方向の移動を無効化
	cameraForward.y = 0.0f;
	cameraRight.y = 0.0f;
	// 正規化
	cameraRight.Normalize();
	cameraForward.Normalize();

	float stickAngleRadians = 0.0f;	// スティックの角度

	// 移動(ワールド方向)
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::FORWARD]))	moveDir += cameraForward;
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::BACK]))		moveDir -= cameraForward;
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::LEFT]))		moveDir -= cameraRight;
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::RIGHT]))		moveDir += cameraRight;

	return moveDir;
}

/* ========================================
	コントローラー入力関数
	-------------------------------------
	内容：コントローラー入力処理
	-------------------------------------
	戻値：Vector3<float>		移動方向
=========================================== */
Vector3<float> ComponentPlayerController::MoveController()
{
	Vector3<float> moveDir = Vector3<float>::Zero();	// 移動方向

	Vector3<float> cameraForward = m_pObjCamera->GetTransform()->GetForwardVector();	// カメラの前方ベクトル
	Vector3<float> cameraRight = m_pObjCamera->GetTransform()->GetRightVector();	// カメラの右方向ベクトル

	// 上下方向の移動を無効化
	cameraForward.y = 0.0f;
	cameraRight.y = 0.0f;
	// 正規化
	cameraRight.Normalize();
	cameraForward.Normalize();

	float stickAngleRadians = 0.0f;	// スティックの角度


	// スティックの角度に基づいて移動方向を計算
	float moveX = cos(MathUtils::ToRadian(stickAngleRadians)); // スティック角度のX成分
	float moveZ = sin(MathUtils::ToRadian(stickAngleRadians)); // スティック角度のZ成分

	// カメラの向きに合わせた移動ベクトルを計算
	moveDir = cameraForward * moveZ + cameraRight * moveX;

	return moveDir;
}

/* ========================================
	移動方向回転関数
	-------------------------------------
	内容：自身を移動方向に回転する
	-------------------------------------
	引数1：Vector3<float>		移動方向
=========================================== */
void ComponentPlayerController::RotateToMoveDir(Vector3<float> moveDir)
{
	// 移動していない場合は処理しない
	if (moveDir.LengthSq() <= 0.01f) return;

	moveDir.Normalize();	// 正規化

	float targetRad			= atan2(moveDir.x, moveDir.z);								// 目標角度
	Quaternion qTargetRot	= Quaternion::FromEulerAngle({ 0.0f, targetRad, 0.0f });	// 目標回転
	Quaternion qSelfRot		= m_pCompTran->GetLocalRotation();							// 自身回転

	// 回転が遠回りにならないように調整
	// ※クォータニオン同士の内積が負の場合は最短距離で回転しないため、回転方向を逆にする
	if (qSelfRot.Dot(qTargetRot) < 0.0f)
	{
		qTargetRot = -qTargetRot;
	}

	Quaternion qRot = Quaternion::Slerp(qSelfRot, qTargetRot, 0.2f);	// スムーズに回転

	m_pCompTran->SetRotation(qRot);	// 回転を設定

}

/* ========================================
	移動アニメーション関数
	-------------------------------------
	内容：移動アニメーション処理
	-------------------------------------
	引数1：Vector3<float>		移動方向
=========================================== */
void ComponentPlayerController::MoveAnime(Vector3<float> vMoveDir)
{
	// ゲームクリア、ゲームオーバーアニメ再生中は処理しない
	if (m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_PLAYER::PLYR_GAMECLEAR) ||
		m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_PLAYER::PLYR_GAMEOVER)) return;

	// ショットアニメが再生されている場合、移動アニメは再生しない
	if (m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_PLAYER::PLYR_SHOT))		return;

	// 移動している場合
	if (std::abs(vMoveDir.x) > 0.0f || std::abs(vMoveDir.z) > 0.0f)
	{
		// 既に歩行アニメが再生されている場合は再生しない
		if (!m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_PLAYER::PLYR_WALK))
			m_pCompModelAnime->PlayAnimeBlend(ANIME_KEY_PLAYER::PLYR_WALK, ANIME_BLEND_TIME, true, ANIME_SPEED_WALK);
	}
	else
	{
		// 既に待機アニメが再生されている場合は再生しない
		if (!m_pCompModelAnime->GetIsPlayAnime(ANIME_KEY_PLAYER::PLYR_IDLE))
			m_pCompModelAnime->PlayAnimeBlend(ANIME_KEY_PLAYER::PLYR_IDLE, ANIME_BLEND_TIME, true, ANIME_SPEED_IDLE);
	}
}


/* ========================================
	ゲッター(移動速度)関数
	-------------------------------------
	戻値：float		移動速度
=========================================== */
float ComponentPlayerController::GetMoveSpeed()
{
	return m_fMoveSpeed;
}

/* ========================================
	ゲッター(回転速度)関数
	-------------------------------------
	戻値：float		回転速度
=========================================== */
float ComponentPlayerController::GetRotateSpeed()
{
	return m_fRotateSpeed;
}

/* ========================================
	ゲッター(ジャンプ有効フラグ)関数
	-------------------------------------
	戻値：bool		ジャンプ有効フラグ
=========================================== */
bool ComponentPlayerController::GetUseJump()
{
	return m_bUseJump;
}

/* ========================================
	セッター(移動速度)関数
	-------------------------------------
	戻値：float		移動速度
=========================================== */
void ComponentPlayerController::SetMoveSpeed(float fMoveSpeed)
{
	m_fMoveSpeed = fMoveSpeed;
}

/* ========================================
	セッター(回転速度)関数
	-------------------------------------
	戻値：float		回転速度
=========================================== */
void ComponentPlayerController::SetRotateSpeed(float fRotateSpeed)
{
	m_fRotateSpeed = fRotateSpeed;
}

/* ========================================
	セッター(ジャンプ有効フラグ)関数
	-------------------------------------
	戻値：bool		ジャンプ有効フラグ
=========================================== */
void ComponentPlayerController::SetUseJump(bool bUseJump)
{
	m_bUseJump = bUseJump;
}

/* ========================================
	セッター(入力有効フラグ)関数
	-------------------------------------
	引数1：bool		入力有効フラグ
=========================================== */
void ComponentPlayerController::SetInputEnable(bool bEnable)
{
	m_bIsInputEnable = bEnable;
}



#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentPlayerController::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupPlayerCtr = Item::CreateGroup("PlayerController");

	pGroupPlayerCtr->AddGroupItem(Item::CreateBind("IsInputEnable", Item::Kind::Bool, &m_bIsInputEnable));	// 入力有効フラグ

	pGroupPlayerCtr->AddGroupItem(Item::CreateBind("MoveSpeed",		Item::Kind::Float, &m_fMoveSpeed));		// 移動速度
	pGroupPlayerCtr->AddGroupItem(Item::CreateBind("RotateSpeed",	Item::Kind::Float, &m_fRotateSpeed));	// 回転速度
	pGroupPlayerCtr->AddGroupItem(Item::CreateBind("JumpPower",		Item::Kind::Float, &m_fJumpPower));		// ジャンプ力

	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Forward",		&m_MoveKey[E_MoveKey::FORWARD]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Back",			&m_MoveKey[E_MoveKey::BACK]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Right",		&m_MoveKey[E_MoveKey::RIGHT]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Left",			&m_MoveKey[E_MoveKey::LEFT]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Jump",			&m_MoveKey[E_MoveKey::JUMP]));

	window.AddItem(pGroupPlayerCtr);

}

/* ========================================
	キー選択リスト作成関数
	-------------------------------------
	内容：キー選択リストを作成
	-------------------------------------
	引数1：項目名
	引数2：キーを割り当てる変数
	-------------------------------------
	戻り値：Item*		アイテム
=========================================== */
DebugUI::Item* ComponentPlayerController::CreateKeyList(std::string sName, BYTE* moveKey)
{
	using namespace DebugUI;

	// リスト作成
	DebugUI::Item* reItem =  Item::CreateList(sName.c_str(), [this, moveKey](const void* arg)
	{
		std::string key = reinterpret_cast<const char*>(arg);
		*moveKey = KEY_MAP.at(key);	// 入力キーを設定(BYTE)

	}, false, true);

	// リスト初期表示を設定
	int i = 0; // リスト位置
	for (auto& key : KEY_MAP)
	{
		reItem->AddListItem(key.first.c_str());
		
		// 初期値と一致する文字列を探す
		if (key.second == *moveKey)
		{
			reItem->SetListNo(i);
		}
		
		i++;
	}

	

	return reItem;
}
#endif // _DEBUG