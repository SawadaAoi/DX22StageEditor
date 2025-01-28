/* ========================================
	CatRobotGame/
	------------------------------------
	地面接触判定コンポーネント用cpp
	------------------------------------
	ComponentGroundRaycast.cpp
========================================== */

// =============== インクルード =====================
#include "ComponentGroundRaycast.h"

#include "ColorVec3.h"
#include "ComponentGround.h"	// 地面オブジェクトが持つ法線を取得するため
#include "ComponentGroundBox.h"
#include "ComponentTransform.h"
#include "ObjectGround.h"		// 接触対象の地面オブジェクト
#include "SceneBase.h"
#include <vector>

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有オブジェクト
=========================================== */
ComponentGroundRaycast::ComponentGroundRaycast(ObjectBase* pOwner)
	: ComponentBase(pOwner,OrderGroundRayCast)
	, m_vStartPos(0.0f, 0.0f, 0.0f)
	, m_vStartPosOffset(0.0f, 0.0f, 0.0f)
	, m_vDirection(0.0f, -1.0f, 0.0f)
	, m_fRayLength(1.0f)
	, m_pOwnerTransform(nullptr)
	, m_pRayLine(nullptr)
	, m_vHitPos(0.0f, 0.0f, 0.0f)
	, m_sHitObjName("")
#ifdef _DEBUG
	, m_bIsHit(false)
	, m_bIsDispRayLine(false)
	, m_bIsDispTriPointToHitPos(false)
#else
	, m_bIsHit(false)
	, m_bIsDispRayLine(false)
	, m_bIsDispTriPointToHitPos(false)
#endif // _DEBUG
{

}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
=========================================== */
void ComponentGroundRaycast::Init()
{
	m_pOwnerTransform	= m_pOwnerObj->GetComponent<ComponentTransform>();
	m_vStartPos			= m_pOwnerTransform->GetPosition() + m_vStartPosOffset;
	m_pRayLine			= std::make_unique<ShapeLine>(m_vStartPos, m_vStartPos + (m_vDirection * m_fRayLength));
	for (int i = 0; i < 3; i++)
	{
		m_pLines_TriPointToHitPos.push_back(std::make_unique<ShapeLine>(Vector3<float>(0.0f, 0.0f, 0.0f), Vector3<float>(0.0f, 0.0f, 0.0f)));
	}
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void ComponentGroundRaycast::Update()
{
	m_vStartPos = m_pOwnerTransform->GetPosition() + m_vStartPosOffset;
	m_pRayLine->UpdateLine(1, m_vStartPos, m_vStartPos + (m_vDirection * m_fRayLength), ColorVec3::RED);

	CheckGround();


#ifdef _DEBUG
	// 接触判定表示用
	if (m_bIsHit)
	{
		m_sDispIsHit = "True";
	}
	else
	{
		m_sDispIsHit = "False";
	}


#endif // _DEBUG
}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
=========================================== */
void ComponentGroundRaycast::Draw()
{
	if (m_bIsDispRayLine) 
	{
		m_pRayLine->Draw();	// レイ描画
	}

	if (m_bIsDispTriPointToHitPos)
	{
		for (auto& pLine : m_pLines_TriPointToHitPos)
		{
			pLine->Draw();	// 内外チェック用線描画
		}
	}

	
}


/* ========================================
	地面判定処理関数
	-------------------------------------
	内容：シーン上に存在する地面オブジェクト
		　と自身のレイとの当たり判定を行う
=========================================== */
void ComponentGroundRaycast::CheckGround()
{
	// シーン上に存在する地面オブジェクトを取得
	std::vector<ObjectBase*> vObjects;
	vObjects = m_pOwnerObj->GetOwnerScene()->GetSceneObjectsTag(E_ObjectTag::Ground);

	for (auto pObject : vObjects)
	{
		ComponentTransform* pPlaneTran	= pObject->GetComponent<ComponentTransform>();
		ComponentGround*	pGround		= pObject->GetComponent<ComponentGround>();

		// 地面コンポーネントがない場合は地面ボックスコンポーネントを取得
		pGround  = pGround ? pGround : pObject->GetComponent<ComponentGroundBox>();

		// コンポーネントが取得できない場合は次のオブジェクトへ
		if (pPlaneTran == nullptr) continue;
		if (pGround == nullptr) continue;

		
		// レイが地面に当たっているかどうかを判定
		if (!CheckHit(pGround))
		{
			m_bIsHit = false;
			continue;
		}

		// 接触座標が地面の上にあるかどうかを判定
		// 接触座標が地面の内側にある場合
 		if (CheckOnGround(pGround))
		{
			m_bIsHit		= true;					
			m_sHitObjName	= pObject->GetName();	// 接触オブジェクト名を取得(斜め床用)

			break;	// いずれかの地面に接触している場合はループを抜ける
		}
		// 接触座標が地面の外側にある場合
		else
		{
			m_bIsHit = false;
		}

	}
}

/* ========================================
	地面接触判定関数
	-------------------------------------
	内容：レイが地面に当たっているかどうかを判定
	-------------------------------------
	引数1：地面コンポーネント
	-------------------------------------
	戻り値：当たっている場合はtrue
=========================================== */
bool ComponentGroundRaycast::CheckHit(ComponentGround* pPlaneGround)
{
	// 地面の座標と法線を取得
	Vector3<float> vGroundPos		= GetGroundCenterPos(pPlaneGround);
	Vector3<float> vGroundNormal	= pPlaneGround->GetWorldNormalDirection().GetNormalize();

	// レイ
	Vector3<float> vRayStart	= m_vStartPos;									// 始点
	Vector3<float> vRayEnd		= m_vStartPos + (m_vDirection * m_fRayLength);	// 終点


	// 地面の座標からレイの始点までの距離
	float fDisStart = vGroundNormal.Dot(vRayStart - vGroundPos);	// 地面の法線にレイの始点までのベクトルを射影

	// 地面の座標からレイの終点までの距離
	float fDisEnd = vGroundNormal.Dot(vRayEnd - vGroundPos);		// 地面の法線にレイの終点までのベクトルを射影

	// レイが平面に接触しているか判定 ====================================

	// レイの始点と終点が平面の同じ側にある場合は接触していない
	// 始点(‐)終点(‐)または始点(+)終点(+)の場合は接触していない(必ず乗算結果はプラスになる)
	if ((fDisStart * fDisEnd) > 0.0f) 
	{
		return false;
	}
	// レイの始点と終点が平面の異なる側にある場合は接触している
	// レイの始点と終点で平面を挟んでいる
	else
	{
		float rate		= fDisStart / (fDisStart - fDisEnd);		// レイの始点から平面までの距離の割合を計算
		float fHitDis	= m_fRayLength * rate;						// レイ上の始点から接触点までの距離を計算
		m_vHitPos		= vRayStart + (m_vDirection * fHitDis);		// 接触点の座標を計算(始点+(方向×距離))

		return true;
	}
}


/* ========================================
	地面内側判定関数
	-------------------------------------
	内容：接触座標が地面の内側にあるかどうかを判定
	-------------------------------------
	引数1：地面コンポーネント
	-------------------------------------
	戻り値：地面の内側にある場合はtrue
=========================================== */
bool ComponentGroundRaycast::CheckOnGround(ComponentGround* pPlaneGround)
{
	// 地面の三角形情報を取得
	std::vector<ComponentGround::T_TriangleVertex> vTriangleVertex = pPlaneGround->GetTriangleVertex();	

	// 地面を構成する三角形の数だけループ
	for (auto& triangle : vTriangleVertex)
	{
		// 三角形の各辺を取得
		Vector3<float> m_vTriEdges[3] = {
			triangle.pos[1] - triangle.pos[0],	// 頂点1→頂点2
			triangle.pos[2] - triangle.pos[1],	// 頂点2→頂点3
			triangle.pos[0] - triangle.pos[2]	// 頂点3→頂点1
		};

		// 各頂点から接触点までのベクトルを取得
		Vector3<float> vHitVec[3] = {
			m_vHitPos - triangle.pos[0],	// 頂点1→接触点
			m_vHitPos - triangle.pos[1],	// 頂点2→接触点
			m_vHitPos - triangle.pos[2]		// 頂点3→接触点
		};


		Vector3<float> vCross[3];	// 外積を格納する変数

		// 各辺と接触点までのベクトルの外積を計算(接触点が三角形の内側にあるかどうかを判定)
		for (int i = 0; i < 3; i++)
		{
			vCross[i] = m_vTriEdges[i].Cross(vHitVec[i]);	// 外積を計算
		}

		// 接触点が三角形の辺と完全に重なっている場合の確認
		if (vCross[0].Length() == 0.0f || vCross[1].Length() == 0.0f || vCross[2].Length() == 0.0f)
		{
			return true;
		}
	
		// 各外積の互いの向きをチェック
		float vDot[3] = {
			vCross[0].Dot(vCross[1]),	// 外積1と外積2の内積
			vCross[1].Dot(vCross[2]),	// 外積2と外積3の内積
			vCross[2].Dot(vCross[0])	// 外積3と外積1の内積
		};

		// 内積が0以上の場合、各外積は同じ方向を向いている
		// つまり、接触点が三角形の内側にある
		if (vDot[0] > 0.0f && 
			vDot[1] > 0.0f && 
			vDot[2] > 0.0f)
		{
			// 確認用の線を描画
			for (int i = 0; i < 3; i++)
			{
				m_pLines_TriPointToHitPos[i]->UpdateLine(1, triangle.pos[i], m_vHitPos, ColorVec3::GREEN);
			}

			return true;	// 四角形を構成する三角形の内、どれか一つでも内側に接触点があればtrueを返す			
		}
	}

	// すべての三角形の内側にない場合はfalseを返す(接触点が地面の外側にある)
	return false;	
}

/* ========================================
	地面中心座標取得関数
	-------------------------------------
	内容：地面の中心座標を取得
	-------------------------------------
	引数1：地面コンポーネント
	-------------------------------------
	戻り値：地面の中心座標
=========================================== */
Vector3<float> ComponentGroundRaycast::GetGroundCenterPos(ComponentGround* pPlaneGround)
{

	Vector3<float> vCenter = Vector3<float>::Zero();

	std::vector<ComponentGround::T_TriangleVertex> vTriangleVertex = pPlaneGround->GetTriangleVertex();

	std::vector<Vector3<float>> vQuadVertexPos;	// 四角形の頂点座標を格納

	vQuadVertexPos.push_back(vTriangleVertex[0].pos[0]);	// 左上
	vQuadVertexPos.push_back(vTriangleVertex[0].pos[1]);	// 右上
	vQuadVertexPos.push_back(vTriangleVertex[1].pos[1]);	// 右下
	vQuadVertexPos.push_back(vTriangleVertex[1].pos[2]);	// 左下

	// 四角形の中心座標を計算（4点の平均）
	for (const auto& vertex : vQuadVertexPos)
	{
		vCenter.x += vertex.x;
		vCenter.y += vertex.y;
		vCenter.z += vertex.z;
	}
	vCenter.x /= 4.0f;
	vCenter.y /= 4.0f;
	vCenter.z /= 4.0f;

	return vCenter;
}


/* ========================================
	ゲッター(地面接触座標)関数
	-------------------------------------
	戻値：Vector3<float>	地面接触座標
=========================================== */
Vector3<float> ComponentGroundRaycast::GetHitPos()
{
	return m_vHitPos;
}

/* ========================================
	ゲッター(地面接触判定フラグ)関数
	-------------------------------------
	戻値：bool	地面接触判定フラグ
=========================================== */
bool ComponentGroundRaycast::GetHitFlg()
{
	return m_bIsHit;
}

/* ========================================
	ゲッター(接触オブジェクト名)関数
	-------------------------------------
	戻値：std::string	接触オブジェクト名
=========================================== */
std::string ComponentGroundRaycast::GetHitObjName()
{
	return m_sHitObjName;
}

/* ========================================
	セッター(レイの長さ)関数
	-------------------------------------
	引数1：float	レイの長さ
=========================================== */
void ComponentGroundRaycast::SetRayLength(float fLength)
{
	m_fRayLength = fLength;
}

/* ========================================
	セッター(レイの方向)関数
	-------------------------------------
	引数1：Vector3<float>	レイの方向
=========================================== */
void ComponentGroundRaycast::SetRayDirection(const Vector3<float>& vDir)
{
	m_vDirection = vDir;
}

/* ========================================
	セッター(レイ始点オフセット)関数
	-------------------------------------
	引数1：Vector3<float>	レイ始点オフセット
=========================================== */
void ComponentGroundRaycast::SetStartPosOffset(const Vector3<float>& vOffset)
{
	m_vStartPosOffset = vOffset;
}

/* ========================================
	セッター(レイ描画フラグ)関数
	-------------------------------------
	引数1：bool	レイ描画フラグ
=========================================== */
void ComponentGroundRaycast::SetDrawLine(bool bDraw)
{
	m_bIsDispRayLine = bDraw;
}


#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentGroundRaycast::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupGroundRaycast = Item::CreateGroup("GroundRaycast");

	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("StartPos",			Item::Vector, &m_vStartPos));		// レイの始点
	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("StartPosOffset",	Item::Vector, &m_vStartPosOffset));	// レイの始点オフセット
	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("Direction",			Item::Vector, &m_vDirection));		// レイの方向
	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("Length",			Item::Float, &m_fRayLength));		// レイの長さ

	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("HitPos",	Item::Vector, &m_vHitPos));		// レイ地面接触座標
	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("IsHit",		Item::Text, &m_sDispIsHit));	// レイ地面接触判定

	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("IsDispRayLine",				Item::Bool, &m_bIsDispRayLine));			// レイ描画フラグ
	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("IsDispTriPointToHitPos",	Item::Bool, &m_bIsDispTriPointToHitPos));	// 三角頂点→地面接触座標までの線表示フラグ

	pGroupGroundRaycast->AddGroupItem(Item::CreateBind("HitObjName", Item::Text, &m_sHitObjName));	// 接触オブジェクト名

	window.AddItem(pGroupGroundRaycast);

}
#endif // _DEBUG