/* ========================================
	DX22Base/
	------------------------------------
	プレイヤー操作コンポーネント用cpp
	------------------------------------
	ComponentPlayerController.cpp
========================================== */

// =============== インクルード =====================
#include "ComponentPlayerController.h"
#include "ObjectBase.h"	// 所持オブジェクトの取得
#include "ComponentTransform.h"	// トランスフォームコンポーネント
#include "ComponentRigidbody.h"	// リジッドボディコンポーネント
#include "Input.h"
#include "unordered_map"

// =============== 定数定義 =======================
// ComponentRigidbody::E_ForceModeを省略
using E_ForceMode = ComponentRigidbody::E_ForceMode;


const float DEFAULT_MOVE_SPEED		= 5.0f;	// 移動速度
const float DEFAULT_ROTATE_SPEED	= 15.0f;	// 回転速度
const float DEFAULT_JUMP_POWER		= 5.0f;	// ジャンプ力


const std::unordered_map<std::string, BYTE> KEY_MAP
{
	{ "SPACE"	, VK_SPACE	 },
	{ "UP"		, VK_UP		 },
	{ "DOWN"	, VK_DOWN	 },
	{ "LEFT"	, VK_LEFT	 },
	{ "RIGHT"	, VK_RIGHT	 },
	{ "CONTROL" , VK_CONTROL },
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
	, m_pCompTran(nullptr)
	, m_pCompRigidbody(nullptr)
	, m_fMoveSpeed(DEFAULT_MOVE_SPEED)
	, m_fRotateSpeed(DEFAULT_ROTATE_SPEED)
	, m_fJumpPower(DEFAULT_JUMP_POWER)
	, m_MoveKey{ 'W', 'S', 'A', 'D', VK_SPACE, VK_LEFT, VK_RIGHT }
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
	// シフトキーを押している間は移動なし
	if (Input::IsKeyPress(VK_SHIFT)) return;

	Vector3<float> moveDir = Vector3<float>::Zero();	// 移動方向

	// 移動(ワールド方向)
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::Forward]))
	{
		moveDir += Vector3<float>::Forward();
	}
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::Back]))
	{
		moveDir += Vector3<float>::Back();
	}
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::Left]))
		{
		moveDir += Vector3<float>::Left();
	}
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::Right]))
	{
		moveDir += Vector3<float>::Right();
	}

	m_pCompRigidbody->AddForce(moveDir * m_fMoveSpeed);

	// ジャンプ
	if (Input::IsKeyTrigger(m_MoveKey[E_MoveKey::Jump]))
	{
		m_pCompRigidbody->AddForce(Vector3<float>::Up() * m_fJumpPower, E_ForceMode::IMPULSE);
	}

	// Y軸回転
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::RotateLeft]))
	{
		m_pCompTran->RotateY(-m_fRotateSpeed * DELTA_TIME);
	}
	if (Input::IsKeyPress(m_MoveKey[E_MoveKey::RotateRight]))
	{
		m_pCompTran->RotateY(m_fRotateSpeed * DELTA_TIME);
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
void ComponentPlayerController::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupPlayerCtr = Item::CreateGroup("PlayerController");

	pGroupPlayerCtr->AddGroupItem(Item::CreateBind("MoveSpeed",		Item::Kind::Float, &m_fMoveSpeed));		// 移動速度
	pGroupPlayerCtr->AddGroupItem(Item::CreateBind("RotateSpeed",	Item::Kind::Float, &m_fRotateSpeed));	// 回転速度
	pGroupPlayerCtr->AddGroupItem(Item::CreateBind("JumpPower",		Item::Kind::Float, &m_fJumpPower));		// ジャンプ力

	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Forward",		&m_MoveKey[E_MoveKey::Forward]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Back",			&m_MoveKey[E_MoveKey::Back]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Right",		&m_MoveKey[E_MoveKey::Right]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Left",			&m_MoveKey[E_MoveKey::Left]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_Jump",			&m_MoveKey[E_MoveKey::Jump]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_RotateLeft",	&m_MoveKey[E_MoveKey::RotateLeft]));
	pGroupPlayerCtr->AddGroupItem(CreateKeyList("MoveKey_RotateRight",	&m_MoveKey[E_MoveKey::RotateRight]));

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