/* ========================================
	DX22Base/
	------------------------------------
	UI画像用cpp
	------------------------------------
	UIComponentSprite.cpp
========================================== */

// =============== インクルード ===================
#include "UIComponentSprite.h"
#include "UIComponentTransform.h"
#include "UIObjectBase.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "MeshBuffer.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所持オブジェクト
=========================================== */
UIComponentSprite::UIComponentSprite(UIObjectBase* owner)
	: UIComponentBase(owner, UIOrderSprite)
	, m_pCompTran(nullptr)
	, m_pMesh(nullptr)
	, m_pTexture(nullptr)
	, m_pVS(nullptr)
	, m_pPS(nullptr)
	, m_vOffset(0.0f, 0.0f)
	, m_vUvPos(0.0f, 0.0f)
	, m_vUvScale(1.0f, 1.0f)
	, m_vColor(1.0f, 1.0f, 1.0f)
	, m_fAlpha(1.0f)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
=========================================== */
void UIComponentSprite::Init()
{
	m_pCompTran = m_pOwnerObj->GetComponent<UIComponentTransform>();

	InitMesh();

	m_pVS		= GET_VS_DATA(VS_KEY::VS_SPRITE);
	m_pPS		= GET_PS_DATA(PS_KEY::PS_SPRITE);
	m_pTexture	= GET_TEXTURE_DATA(TEX_KEY::TEST);	// デフォルトテクスチャ
}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
=========================================== */
void UIComponentSprite::Draw()
{
	if (!m_bIsVisible) return;

	DirectX::XMFLOAT4X4 matWVP[3];
	// ワールド行列
	matWVP[0] = CreateWorldMatrix();	
	// ビュー行列
	DirectX::XMStoreFloat4x4(&matWVP[1], DirectX::XMMatrixIdentity());	// UIはビュー行列がないので単位行列
	// プロジェクション行列
	matWVP[2] = CAMERA_MNG_INST.GetActiveCamera()->GetProjectionMatrixUI();

	Vector2<float> vScale = m_pCompTran->GetWorldScale();
	// 頂点シェーダに渡すパラメータ
	DirectX::XMFLOAT4 param[3];
	param[0] = { m_vOffset.x, m_vOffset.y, vScale.x, vScale.y };		// オフセット、スケール
	param[1] = { m_vUvPos.x, m_vUvPos.y, m_vUvScale.x, m_vUvScale.y };	// UV座標、UVスケール
	param[2] = { m_vColor.x, m_vColor.y, m_vColor.z, m_fAlpha };		// 色、透明度

	//頂点シェーダ
	m_pVS->WriteBuffer(0, matWVP);
	m_pVS->WriteBuffer(1, param);
	m_pVS->Bind();

	// ピクセルシェーダ
	m_pPS->SetTexture(0, m_pTexture);
	m_pPS->Bind();

	// メッシュ描画
	m_pMesh->Draw();
}


/* ========================================
	メッシュ初期化関数
	-------------------------------------
	内容：メッシュの初期化
=========================================== */
void UIComponentSprite::InitMesh()
{
	struct T_Vertex
	{
		float pos[3];
		float uv[2];
	};
	T_Vertex vtx[] = {
		{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}},	// 左上
		{{ 0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}},	// 右上
		{{-0.5f,-0.5f, 0.0f}, {0.0f, 1.0f}},	// 左下
		{{ 0.5f,-0.5f, 0.0f}, {1.0f, 1.0f}},	// 右下
	};

	// メッシュ
	MeshBuffer::T_MeshData desc = {};
	desc.pVtx		= vtx;
	desc.vtxSize	= sizeof(T_Vertex);
	desc.vtxCount	= _countof(vtx);
	desc.topology	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	m_pMesh			= std::make_shared<MeshBuffer>(desc);
}

/* ========================================
	ワールド行列作成関数
	-------------------------------------
	内容：ワールド行列を作成する
	-------------------------------------
	戻り値：ワールド行列
=========================================== */
DirectX::XMFLOAT4X4 UIComponentSprite::CreateWorldMatrix()
{
	DirectX::XMFLOAT4X4 mat;

	Vector2<float>	vPos	= m_pCompTran->GetWorldPosition();
	float			fRot	= m_pCompTran->GetWorldRotation();

	DirectX::XMStoreFloat4x4(&mat,								// Float4x4に変換して格納
		DirectX::XMMatrixTranspose(								// 転地
			DirectX::XMMatrixRotationZ(fRot) *					// 回転
			DirectX::XMMatrixTranslation(vPos.x, vPos.y, 0.0f)	// 移動
		));

	return mat;
}

/* ========================================
	ゲッター(テクスチャ)関数
	-------------------------------------
	戻値：テクスチャ
=========================================== */
Texture* UIComponentSprite::GetTexture()
{
	return m_pTexture;
}

/* ========================================
	ゲッター(表示フラグ)関数
	-------------------------------------
	戻値：表示フラグ
=========================================== */
bool UIComponentSprite::GetIsVisible()
{
	return m_bIsVisible;
}

/* ========================================
	ゲッター(オフセット)関数
	-------------------------------------
	戻値：オフセット
=========================================== */
Vector2<float> UIComponentSprite::GetOffset()
{
	return m_vOffset;
}

/* ========================================
	ゲッター(UV座標)関数
	-------------------------------------
	戻値：UV座標
=========================================== */
Vector2<float> UIComponentSprite::GetUvPos()
{
	return m_vUvPos;
}

/* ========================================
	ゲッター(UVスケール)関数
	-------------------------------------
	戻値：UVスケール
=========================================== */
Vector2<float> UIComponentSprite::GetUvScale()
{
	return m_vUvScale;
}

/* ========================================
	ゲッター(色)関数
	-------------------------------------
	戻値：色
=========================================== */
Vector3<float> UIComponentSprite::GetColor()
{
	return m_vColor;
}

/* ========================================
	ゲッター(透過度)関数
	-------------------------------------
	戻値：透過度
=========================================== */
float UIComponentSprite::GetAlpha()
{
	return m_fAlpha;
}

/* ========================================
	セッター(テクスチャ)関数
	-------------------------------------
	引数1：テクスチャ
=========================================== */
void UIComponentSprite::SetTexture(Texture* pTexture)
{
	m_pTexture = pTexture;
}

/* ========================================
	セッター(表示フラグ)関数
	-------------------------------------
	引数1：表示フラグ
=========================================== */
void UIComponentSprite::SetIsVisible(bool visible)
{
	m_bIsVisible = visible;
}

/* ========================================
	セッター(オフセット)関数
	-------------------------------------
	引数1：オフセット
=========================================== */
void UIComponentSprite::SetOffset(Vector2<float> offset)
{
	m_vOffset = offset;
}

/* ========================================
	セッター(UV座標)関数
	-------------------------------------
	引数1：UV座標
=========================================== */
void UIComponentSprite::SetUvPos(Vector2<float> uvPos)
{
	m_vUvPos = uvPos;
}

/* ========================================
	セッター(UVスケール)関数
	-------------------------------------
	引数1：UVスケール
=========================================== */
void UIComponentSprite::SetUvScale(Vector2<float> uvScale)
{
	m_vUvScale = uvScale;
}

/* ========================================
	セッター(色)関数
	-------------------------------------
	引数1：色
=========================================== */
void UIComponentSprite::SetColor(Vector3<float> color)
{
	m_vColor = color;
}

/* ========================================
	セッター(透過度)関数
	-------------------------------------
	引数1：透過度
=========================================== */
void UIComponentSprite::SetAlpha(float alpha)
{
	m_fAlpha = alpha;
}


#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void UIComponentSprite::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupUISprite = Item::CreateGroup("UI_Sprite");

	pGroupUISprite->AddGroupItem(Item::CreateBind("Visible", Item::Kind::Bool, &m_bIsVisible));	// 表示フラグ
	pGroupUISprite->AddGroupItem(Item::CreateBind("Offset",	Item::Kind::Vector2,&m_vOffset));	// オフセット
	pGroupUISprite->AddGroupItem(Item::CreateBind("UVPos",	Item::Kind::Vector2,&m_vUvPos));	// UV座標
	pGroupUISprite->AddGroupItem(Item::CreateBind("UVScale",Item::Kind::Vector2,&m_vUvScale));	// UVスケール
	pGroupUISprite->AddGroupItem(Item::CreateBind("Color",	Item::Kind::Vector,	&m_vColor));	// 色
	pGroupUISprite->AddGroupItem(Item::CreateBind("Alpha",	Item::Kind::Float,	&m_fAlpha));	// 透過度
	
	pGroupUISprite->AddGroupItem(InitTextureList());	// テクスチャリスト

	window.AddItem(pGroupUISprite);

}


/* ========================================
	テクスチャリスト初期化関数
	-------------------------------------
	内容：テクスチャリストの初期化
	-------------------------------------
	戻り値：DebugUI::Item*	リスト項目
=========================================== */
DebugUI::Item* UIComponentSprite::InitTextureList()
{
	using namespace DebugUI;

	DebugUI::Item* pTexList = Item::CreateList("Texture", [this](const void* arg)
	{
		// テクスチャ変更
		std::string sTexName = reinterpret_cast<const char*>(arg);
		TextureManager::E_TEX_KEY eKey;

		std::unordered_map < TextureManager::E_TEX_KEY, std::string > texPathMap = TEXTURE_MNG_INST.GetTexturePaths();
		for (auto& texPath : texPathMap)
		{
			if (texPath.second == sTexName)
			{
				eKey = texPath.first;
				break;
			}
		}

		std::unordered_map<TextureManager::E_TEX_KEY, std::shared_ptr<Texture>> texMap = TEXTURE_MNG_INST.GetTextureDatas();

		m_pTexture = texMap.find(eKey)->second.get();


	}, false, true);

	// 登録されているテクスチャをリストに追加
	std::unordered_map<TextureManager::E_TEX_KEY, std::string> texPathMap = TEXTURE_MNG_INST.GetTexturePaths();
	for (auto& texPath : texPathMap)
	{
		pTexList->AddListItem(texPath.second.c_str());
	}

	// 現在のテクスチャを選択
	std::unordered_map<TextureManager::E_TEX_KEY, std::shared_ptr<Texture>> texMap = TEXTURE_MNG_INST.GetTextureDatas();
	for (auto& tex : texMap)
	{
		if (tex.second.get() == m_pTexture)
		{
			pTexList->SetListNo(tex.first);
			break;
		}
	}

	return pTexList;
}
#endif // _DEBUG

