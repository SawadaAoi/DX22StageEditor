/* ========================================
	DX22Base/
	------------------------------------
	図形表示コンポーネント用cpp
	------------------------------------
	ComponentGeometry.cpp
========================================== */

// =============== インクルード ===================
#include "ComponentGeometry.h"
#include "ComponentTransform.h"
#include "ObjectBase.h"
#include "ShapeBox.h"		// 立方体
#include "ShapeSphere.h"	// 球
#include "ShapePlane.h"		// 平面
#include "TextureManager.h"

// =============== 定数定義 =======================



/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：コンポーネントを持つオブジェクト
=========================================== */
ComponentGeometry::ComponentGeometry(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderGeometry)
	, m_eType(TYPE_NONE)
	, m_pShape(nullptr)
	, m_pTransform(nullptr)
	, m_pTexture(nullptr)
	, m_bIsTex(false)
{
	m_pTransform = m_pOwnerObj->GetComponent<ComponentTransform>();
	m_pTexture = GET_TEXTURE_DATA(TextureManager::E_TEX_KEY::TEST);	// デフォルトテクスチャ
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
=========================================== */
void ComponentGeometry::Init()
{
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void ComponentGeometry::Update()
{
	// 図形オブジェクトが生成されていない場合は処理を行わない
	if (m_pShape == nullptr) return;

	// 座標コンポーネントが取得できた場合は座標を設定
	if (m_pTransform != nullptr)
	{
		// 座標コンポーネントが取得できた場合は座標を設定
		m_pShape->SetPosition(m_pTransform->GetWorldPosition());
		m_pShape->SetRotation(m_pTransform->GetWorldRotation());
		m_pShape->SetScale(m_pTransform->GetWorldScale());
	}

	// テクスチャ設定
	m_pShape->SetTexture(m_pTexture);
	m_pShape->SetUseTexture(m_bIsTex);
}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
=========================================== */
void ComponentGeometry::Draw()
{
	// 図形オブジェクトが生成されている場合は描画
	if (m_pShape)
	{
		m_pShape->Draw();
	}
}


/* ========================================
	図形オブジェクト生成関数
	-------------------------------------
	内容：図形オブジェクトを生成
=========================================== */
void ComponentGeometry::CreateShape()
{
	// 既に生成されている場合は解放
	if (m_pShape)
	{
		m_pShape.reset();
	}

	// 図形の種類によって生成するオブジェクトを変更
	switch (m_eType)
	{
		// なし
	case TYPE_NONE:
		m_pShape = nullptr;
		break;
		// 立方体
	case TYPE_BOX:
		m_pShape = std::make_shared<ShapeBox>();
		break;
		// 球
	case TYPE_SPHERE:
		m_pShape = std::make_shared<ShapeSphere>();
		break;
	case TYPE_PLANE:
		m_pShape = std::make_shared<ShapePlane>();
		break;
	}
}

/* ========================================
	セッター(図形種類)関数
	-------------------------------------
	引数：図形の種類
=========================================== */
void ComponentGeometry::SetShapeType(E_ShapeType eType)
{
	m_eType = eType;
	CreateShape();	// 設定した値で図形オブジェクトを生成

}
/* ========================================
	セッター(テクスチャ)関数
	-------------------------------------
	引数：テクスチャ
=========================================== */
void ComponentGeometry::SetTexture(Texture* pTexture)
{
	m_pTexture = pTexture;
}

/* ========================================
	セッター(テクスチャ使用フラグ)関数
	-------------------------------------
	引数：テクスチャ使用フラグ
=========================================== */
void ComponentGeometry::SetIsTex(bool bIsTex)
{
	m_bIsTex = bIsTex;
}


#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentGeometry::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupGeometry = Item::CreateGroup("Geometry");
	// 図形種類リスト
	pGroupGeometry->AddGroupItem(InitShapeTypeList());	
	// テクスチャ使用フラグ
	pGroupGeometry->AddGroupItem(Item::CreateCallBack("UseTexture", Item::Kind::Bool, [this](bool isWrite, void* arg)
	{
		bool* pIsTex = reinterpret_cast<bool*>(arg);
		if (isWrite)
			m_bIsTex = *pIsTex;
		else
			*pIsTex = m_bIsTex;

		// テクスチャ使用フラグがONの場合のみテクスチャリストを表示
		if (!m_bIsTex)
		{
			WIN_OBJ_INFO["Geometry"]["Texture"].RemoveListItemAll();	// リストをクリア
		}
		else
		{	// テクスチャリストに登録されている画像をリストに追加
			std::unordered_map < TextureManager::E_TEX_KEY, std::string > texPathMap = TEXTURE_MNG_INST.GetTexturePaths();
			for (auto& texPath : texPathMap)
			{
				WIN_OBJ_INFO["Geometry"]["Texture"].AddListItem(texPath.second.c_str());
			}
		}
	}));	// テクスチャ使用フラグ
	// テクスチャリスト
	pGroupGeometry->AddGroupItem(InitTextureList());

	window.AddItem(pGroupGeometry);

	
}

/* ========================================
	図形種類リスト初期化関数
	-------------------------------------
	内容：図形種類リストの初期化
	-------------------------------------
	戻り値：DebugUI::Item*	リスト項目
=========================================== */
DebugUI::Item* ComponentGeometry::InitShapeTypeList()
{
	using namespace DebugUI;


	// 図形の種類
	std::unordered_map<std::string, E_ShapeType> mapShapeType =
	{
		{ "None",	TYPE_NONE },
		{ "Box",	TYPE_BOX },
		{ "Sphere", TYPE_SPHERE },
		{ "Plane",	TYPE_PLANE },
	};

	// 図形一覧リスト
	Item* pShapeList = Item::CreateList("ShapeType", [this, mapShapeType](const void* arg)
	{	// 項目クリックで図形変更
		std::string sType = reinterpret_cast<const char*>(arg);
		SetShapeType(mapShapeType.at(sType));
	}, false, true);

	// リスト項目追加
	for (auto& shape : mapShapeType)
	{
		pShapeList->AddListItem(shape.first.c_str());
	}
	pShapeList->SetListNo(static_cast<int>(m_eType));	// 選択中の図形を設定

	return pShapeList;
}


/* ========================================
	テクスチャリスト初期化関数
	-------------------------------------
	内容：テクスチャリストの初期化
	-------------------------------------
	戻り値：DebugUI::Item*	リスト項目
=========================================== */
DebugUI::Item* ComponentGeometry::InitTextureList()
{
	using namespace DebugUI;

	// テクスチャリスト
	Item* pTextureList = Item::CreateList("Texture", [this](const void* arg)
	{
		// リストの表示項目(画像パス)から画像キーを取得
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
		// 画像キーからテクスチャを取得
		std::unordered_map<TextureManager::E_TEX_KEY, std::shared_ptr<Texture>> texMap = TEXTURE_MNG_INST.GetTextureDatas();
		m_pTexture = texMap.find(eKey)->second.get();

	}, false, true);	// プルダウン

	// 登録されているテクスチャをリストに追加
	std::unordered_map < TextureManager::E_TEX_KEY, std::string > texPathMap = TEXTURE_MNG_INST.GetTexturePaths();
	for (auto& texPath : texPathMap)
	{
		pTextureList->AddListItem(texPath.second.c_str());
	}

	// 現在設定されている画像からリストの表示項目を設定
	std::unordered_map<TextureManager::E_TEX_KEY, std::shared_ptr<Texture>> texMap = TEXTURE_MNG_INST.GetTextureDatas();
	for (auto& tex : texMap)
	{
		if (tex.second.get() == m_pTexture)
		{
			pTextureList->SetListNo(tex.first);
			break;
		}
	}
	
	return pTextureList;
}

#endif // _DEBUG