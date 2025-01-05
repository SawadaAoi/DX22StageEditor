/* ========================================
	DX22Base/
	------------------------------------
	メイン用cpp
	------------------------------------
	main.cpp
========================================== */

// =============== インクルード ===================
#include "ComponentCameraDebug.h"
#include "ObjectBase.h"				// 所持オブジェクト
#include "ComponentCameraBase.h"	// カメラの基本機能(移動、回転)
#include "ObjectCamera.h"			// 所持オブジェクトをカメラにキャスト
#include "WindowAPI.h"

// =============== 定数定義 =======================
const float LIMIT_PITCH = 90.0f;	// 縦回転制限

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有者オブジェクト
=========================================== */
ComponentCameraDebug::ComponentCameraDebug(ObjectBase* pObject)
	: ComponentBase(pObject, OrderCameraDebug)
	, m_fMoveSpeed(5.0f)
	, m_fRotSpeed(50.0f)
	, m_pCameraBase(nullptr)
	, m_vMouseMove(0.0f, 0.0f)
	, m_CursorPosOld{ 0, 0 }
	, m_bRotateMouse(true)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
=========================================== */
void ComponentCameraDebug::Init()
{
	m_pCameraBase = m_pOwnerObj->GetComponent<ComponentCameraBase>();	// カメラの基本機能を変数化
}


/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void ComponentCameraDebug::Update()
{
	// カメラがアクティブでない場合は処理しない
	if (static_cast<ObjectCamera*>(m_pOwnerObj)->GetActive() == false) return;


	Move();


	if (m_bRotateMouse)
	{
		RotateMouse();
	}
	else
	{
		RotateKey();
	}


}


/* ========================================
	移動関数
	-------------------------------------
	内容：移動処理
=========================================== */
void ComponentCameraDebug::Move()
{
	// 移動はShiftキーを押しながらのみ有効
	if (!Input::IsKeyPress(VK_SHIFT)) return;

	Vector3<float> vFowrard = m_pCameraBase->GetForward();	// カメラの前方向
	Vector3<float> vRight = m_pCameraBase->GetRight();		// カメラの右方向
	Vector3<float> vUp = m_pCameraBase->GetUp();			// カメラの上方向

	Vector3<float> fMove;	// 移動量

	if (Input::IsKeyPress('W')) fMove = vFowrard * m_fMoveSpeed;
	if (Input::IsKeyPress('S')) fMove = vFowrard * -m_fMoveSpeed;
	if (Input::IsKeyPress('A')) fMove = vRight * -m_fMoveSpeed;
	if (Input::IsKeyPress('D')) fMove = vRight * m_fMoveSpeed;
	if (Input::IsKeyPress('Q')) fMove = vUp * -m_fMoveSpeed;
	if (Input::IsKeyPress('E')) fMove = vUp * m_fMoveSpeed;

	m_pCameraBase->Move(fMove * DELTA_TIME);	// 移動処理
}

/* ========================================
	回転(キー入力)関数
	-------------------------------------
	内容：カメラの回転処理
=========================================== */
void ComponentCameraDebug::RotateKey()
{
	// キー回転はShiftキーを押しながらのみ有効
	if (!Input::IsKeyPress(VK_SHIFT)) return;

	float pitch = 0.0f;	// 縦回転
	float yaw = 0.0f;	// 横回転
	if (Input::IsKeyPress(VK_UP))		pitch = -m_fRotSpeed;
	if (Input::IsKeyPress(VK_DOWN))		pitch = m_fRotSpeed;
	if (Input::IsKeyPress(VK_LEFT))		yaw = -m_fRotSpeed;
	if (Input::IsKeyPress(VK_RIGHT))	yaw = m_fRotSpeed;

	// Z軸回転はしない
	m_pCameraBase->RotateLimit(pitch * DELTA_TIME, yaw * DELTA_TIME, LIMIT_PITCH);	// 回転処理(制限付き
}

/* ========================================
	回転(マウス)関数
	-------------------------------------
	内容：カメラの回転処理
=========================================== */
void ComponentCameraDebug::RotateMouse()
{
	if (Input::IsKeyPress(VK_RBUTTON))	// 右クリック押下時
	{
		// マウス移動量
		POINT cursorPos;
		GetCursorPos(&cursorPos);

		m_vMouseMove = Vector2<float>(
			static_cast<float>(cursorPos.x - m_CursorPosOld.x),
			static_cast<float>(cursorPos.y - m_CursorPosOld.y));

		m_CursorPosOld = cursorPos;

		// マウスの移動量 / 画面サイズ の比率から、画面全体でどれだけ回転するか指定する。
		float angleX = 360.0f * m_vMouseMove.x / WINDOW_INST.GetScreenSize().nWidth;
		float angleY = 180.0f * m_vMouseMove.y / WINDOW_INST.GetScreenSize().nHeight;

		m_pCameraBase->RotateLimit(angleY, 0, LIMIT_PITCH);
		m_pCameraBase->RotateLimit(0, angleX, LIMIT_PITCH);
	}
	else
	{
		GetCursorPos(&m_CursorPosOld);
		m_vMouseMove = { 0.0f, 0.0f };
	}
}


#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentCameraDebug::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupCameraDebug = Item::CreateGroup("CameraDebug");

	pGroupCameraDebug->AddGroupItem(Item::CreateBind("MoveSpeed", Item::Kind::Float, &m_fMoveSpeed));
	pGroupCameraDebug->AddGroupItem(Item::CreateBind("RotSpeed", Item::Kind::Float, &m_fRotSpeed));

	pGroupCameraDebug->AddGroupItem(Item::CreateBind("RotateMouse", Item::Kind::Bool, &m_bRotateMouse));

	window.AddItem(pGroupCameraDebug);
}

#endif // _DEBUG
