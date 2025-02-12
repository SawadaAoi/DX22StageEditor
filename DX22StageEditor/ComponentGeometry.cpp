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
#include "LightManager.h"
#include "CameraManager.h"


// =============== 定数定義 =======================
const int MAX_TEXTURE_NUM	= 6;	// テクスチャ最大数
const int MAX_UV_NUM		= 3;	// uv最大数(Z,X,Y)

// デバッグ用のUVの配列番号と軸名
static std::unordered_map<int, std::string> mapUvName =
{
	{ 0, "Z" },
	{ 1, "X" },
	{ 2, "Y" },
};

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
	, m_pTextures()
	, m_bIsTex(false)
	, m_bIsCulling(true)
	, m_fUvScale()
	, m_fUvOffset()
{
	m_pTransform = m_pOwnerObj->GetComponent<ComponentTransform>();

	m_pTextures.resize(MAX_TEXTURE_NUM);
	for (auto& tex : m_pTextures)
		tex = GET_TEXTURE_DATA(TextureManager::E_TEX_KEY::TEST);

	m_fUvOffset.resize(MAX_UV_NUM);
	for (auto& uv : m_fUvOffset)
		uv = { 0.0f, 0.0f };

	m_fUvScale.resize(MAX_UV_NUM);
	for (auto& scale : m_fUvScale)
		scale = { 1.0f, 1.0f };
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
		m_pShape->SetPosition(m_pTransform->GetPosition());
		m_pShape->SetRotation(m_pTransform->GetRotation());
		m_pShape->SetScale(m_pTransform->GetScale());
	}

	// テクスチャ設定
	// 立方体の場合は6面分のテクスチャを設定
	if (m_eType == TYPE_BOX)
	{
		for (int i = 0; i < MAX_TEXTURE_NUM; i++)
		{
			m_pShape->SetTexture(m_pTextures[i], i);
		}
		for (int i = 0; i < MAX_UV_NUM; i++)
		{
			m_pShape->SetUvScale(m_fUvScale[i], i);
			m_pShape->SetUvOffset(m_fUvOffset[i], i);
		}
	}
	// 球、平面の場合は1つのテクスチャを設定
	else
	{
		m_pShape->SetTexture(m_pTextures[0]);
		m_pShape->SetUvScale(m_fUvScale[0]);
		m_pShape->SetUvOffset(m_fUvOffset[0]);
	}
	m_pShape->SetUseTexture(m_bIsTex);
	m_pShape->SetIsCulling(m_bIsCulling);
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
		ObjectBase::T_LightParam lightParam = m_pOwnerObj->GetLightMaterial();
		m_pShape->SetLightMaterial(lightParam.fDiffuse, lightParam.fSpecular, lightParam.fAmbient, lightParam.bLightUse);					// ライトパラメー
		m_pShape->SetLights(LIGHT_MNG_INST.GetLightList());	// ライト設定
		m_pShape->SetCameraPos(CAMERA_MNG_INST.GetActiveCamera()->GetComponent<ComponentTransform>()->GetPosition());	// カメラ位置設定

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
	ゲッター(図形種類)関数
	-------------------------------------
	戻り値：図形の種類
=========================================== */
ComponentGeometry::E_ShapeType ComponentGeometry::GetShapeType() const
{
	return m_eType;
}

/* ========================================
	ゲッター(テクスチャ)関数
	-------------------------------------
	引数：int 配列番号
	-------------------------------------
	戻り値：テクスチャ
=========================================== */
Texture* ComponentGeometry::GetTexture(int nIndex) const
{
	// 立方体の場合は6面分のテクスチャを返す
	if (m_eType == TYPE_BOX)
	{
		// インデックスが範囲内の場合はテクスチャを返す
		if (nIndex >= 0 && nIndex < MAX_TEXTURE_NUM)
			return m_pTextures[nIndex];
		else
			return m_pTextures[0];
	}
	else
	{
		return m_pTextures[0];
	}
}

/* ========================================
	ゲッター(テクスチャ使用フラグ)関数
	-------------------------------------
	戻り値：テクスチャ使用フラグ
=========================================== */
bool ComponentGeometry::GetIsTex() const
{
	return m_bIsTex;
}

/* ========================================
	ゲッター(カリング設定)関数
	-------------------------------------
	戻り値：カリング設定
=========================================== */
bool ComponentGeometry::GetCulling() const
{
	return m_bIsCulling;
}

/* ========================================
	ゲッター(テクスチャオフセット)関数
	-------------------------------------
	引数：int 配列番号
	-------------------------------------
	戻り値：テクスチャオフセット
=========================================== */
Vector2<float> ComponentGeometry::GetUvScale(int nIndex) const
{
	// 立方体の場合はZ,X,Yのテクスチャオフセット返す
	if (m_eType == TYPE_BOX)
	{
		// インデックスが範囲内の場合はテクスチャオフセット返す
		if (nIndex >= 0 && nIndex < MAX_UV_NUM)
			return m_fUvScale[nIndex];
		else
			return m_fUvScale[0];
	}
	else
	{
		return m_fUvScale[0];
	}
}

/* ========================================
	ゲッター(テクスチャスケール)関数
	-------------------------------------
	引数：int 配列番号
	-------------------------------------
	戻り値：テクスチャスケール
=========================================== */
Vector2<float> ComponentGeometry::GetUvOffset(int nIndex) const
{
	// 立方体の場合はZ,X,Yのテクスチャスケール返す
	if (m_eType == TYPE_BOX)
	{
		// インデックスが範囲内の場合はテクスチャスケール返す
		if (nIndex >= 0 && nIndex < MAX_UV_NUM)
			return m_fUvOffset[nIndex];
		else
			return m_fUvOffset[0];
	}
	else
	{
		return m_fUvOffset[0];
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
	引数1：テクスチャ
	引数2：int 配列番号
=========================================== */
void ComponentGeometry::SetTexture(Texture* pTexture, int nIndex)
{
	// 立方体の場合は6面分のテクスチャを設定
	if (m_eType == TYPE_BOX)
	{
		// インデックスが範囲内の場合はテクスチャを設定
		if (nIndex >= 0 && nIndex < MAX_TEXTURE_NUM)
			m_pTextures[nIndex] = pTexture;
		else
			m_pTextures[0] = pTexture;
	}
	else
	{
		m_pTextures[0] = pTexture;
	}
}

/* ========================================
	セッター(テクスチャ(全面))関数
	-------------------------------------
	引数：テクスチャ
=========================================== */
void ComponentGeometry::SetTextureAll(Texture* pTexture)
{
	if (m_eType == TYPE_BOX)
	{
		for (int i = 0; i < MAX_TEXTURE_NUM; i++)
		{
			m_pTextures[i] = pTexture;
		}
	}
	else
	{
		m_pTextures[0] = pTexture;
	}
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

/* ========================================
	セッター(カリング設定)関数
	-------------------------------------
	引数：カリング設定
=========================================== */
void ComponentGeometry::SetCulling(bool bIsCulling)
{
	m_bIsCulling = bIsCulling;
}

/* ========================================
	セッター(テクスチャオフセット)関数
	-------------------------------------
	引数1：テクスチャオフセット
	引数2：int 配列番号
=========================================== */
void ComponentGeometry::SetUvScale(const Vector2<float>& scale, int nIndex)
{
	// 立方体の場合はZ,X,Yのテクスチャオフセット設定
	if (m_eType == TYPE_BOX)
	{
		// インデックスが範囲内の場合はテクスチャオフセット設定
		if (nIndex >= 0 && nIndex < MAX_UV_NUM)
			m_fUvScale[nIndex] = scale;
		else
			m_fUvScale[0] = scale;
	}
	else
	{
		m_fUvScale[0] = scale;
	}
}

/* ========================================
	セッター(テクスチャスケール)関数
	-------------------------------------
	引数1：テクスチャスケール
	引数2：int 配列番号
=========================================== */
void ComponentGeometry::SetUvOffset(const Vector2<float>& offset, int nIndex)
{
	// 立方体の場合はZ,X,Yのテクスチャスケール設定
	if (m_eType == TYPE_BOX)
	{
		// インデックスが範囲内の場合はテクスチャスケール設定
		if (nIndex >= 0 && nIndex < MAX_UV_NUM)
			m_fUvOffset[nIndex] = offset;
		else
			m_fUvOffset[0] = offset;
	}
	else
	{
		m_fUvOffset[0] = offset;
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
void ComponentGeometry::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupGeometry = Item::CreateGroup("Geometry");
	// 図形種類リスト
	pGroupGeometry->AddGroupItem(InitShapeTypeList());	
	// テクスチャ使用フラグ
	pGroupGeometry->AddGroupItem(InitUseTexture());	// テクスチャ使用フラグ

	// 0番目のテクスチャを全コピー
	pGroupGeometry->AddGroupItem(InitTexAllCopy());	// テクスチャ全コピー

	// テクスチャリスト
	int nTexNum = (m_eType == TYPE_BOX) ? MAX_TEXTURE_NUM : 1;	// 立方体の場合は6面分のテクスチャリストを表示
	for (int i = 0; i < nTexNum; i++)
		pGroupGeometry->AddGroupItem(InitTextureList(i));

	// カリング設定
	pGroupGeometry->AddGroupItem(Item::CreateBind("Culling", Item::Kind::Bool, &m_bIsCulling));

	window.AddItem(pGroupGeometry);

	// Uvオフセット,スケール
	InitUv();

	
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

		// テクスチャ使用フラグがONの場合のみテクスチャリストを表示
		if (m_bIsTex)
		{
			// リストをクリア
			for (int i = 0; i < MAX_TEXTURE_NUM; i++)
			{
				std::string sTexName = "Texture_" + std::to_string(i);
				WIN_OBJ_INFO["Geometry"][sTexName.c_str()].RemoveListItemAll();
			}

			// 立方体の場合は6面分のテクスチャリストを表示
			int nTexNum = (m_eType == TYPE_BOX) ? MAX_TEXTURE_NUM : 1;
			// テクスチャリストに登録されている画像をリストに追加
			std::unordered_map < TextureManager::E_TEX_KEY, std::string > texPathMap = TEXTURE_MNG_INST.GetTexturePaths();
			for (int i = 0; i < nTexNum; i++)
			{
				std::string sTexName = "Texture_" + std::to_string(i);
				for (auto& texPath : texPathMap)
				{
					WIN_OBJ_INFO["Geometry"][sTexName.c_str()].AddListItem(texPath.second.c_str());
				}
			}

			RemoveUv();	// Uvオフセット,スケールを削除
			InitUv();	// Uvオフセット,スケールを初期化

		}


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
DebugUI::Item* ComponentGeometry::InitTextureList(int nIdx)
{
	using namespace DebugUI;

	// テクスチャリスト
	Item* pTextureList = Item::CreateList(("Texture_" + std::to_string(nIdx)).c_str(), [this, nIdx](const void* arg)
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
		m_pTextures[nIdx] = texMap.find(eKey)->second.get();

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
		if (tex.second.get() == m_pTextures[nIdx])
		{
			pTextureList->SetListNo(tex.first);
			break;
		}
	}
	
	return pTextureList;
}

/* ========================================
	テクスチャ使用フラグ初期化関数
	-------------------------------------
	内容：テクスチャ使用フラグの初期化
	-------------------------------------
	戻り値：DebugUI::Item*	リスト項目
=========================================== */
DebugUI::Item* ComponentGeometry::InitUseTexture()
{
	using namespace DebugUI;

	Item* pUseTexture = Item::CreateCallBack("UseTexture", Item::Kind::Bool, [this](bool isWrite, void* arg)
	{
		bool* pIsTex = reinterpret_cast<bool*>(arg);
		if (isWrite)
		{
			m_bIsTex = *pIsTex;
			// OFFの場合はテクスチャリスト、カリング設定、Uvオフセット,スケールを非表示
			if (!m_bIsTex)
			{
				// テクスチャリストをクリア
				int nTexNum = (m_eType == TYPE_BOX) ? MAX_TEXTURE_NUM : 1;
				for (int i = 0; i < nTexNum; i++)
				{
					std::string sTexName = "Texture_" + std::to_string(i);
					WIN_OBJ_INFO["Geometry"][sTexName.c_str()].RemoveListItemAll();
				}
				// カリング設定を削除
				WIN_OBJ_INFO["Geometry"].RemoveGroupItem("Culling");	
				// Uvオフセット,スケールを削除
				RemoveUv();	

			}
			// テクスチャ使用フラグがONの場合テクスチャリスト、カリング設定、Uvオフセット,スケールを表示
			else
			{
				// テクスチャリストの表示
				int nTexNum = (m_eType == TYPE_BOX) ? MAX_TEXTURE_NUM : 1;
				// テクスチャリストに登録されている画像をリストに追加
				std::unordered_map < TextureManager::E_TEX_KEY, std::string > texPathMap = TEXTURE_MNG_INST.GetTexturePaths();
				for (int i = 0; i < nTexNum; i++)
				{
					std::string sTexName = "Texture_" + std::to_string(i);
					for (auto& texPath : texPathMap)
					{
						WIN_OBJ_INFO["Geometry"][sTexName.c_str()].AddListItem(texPath.second.c_str());
					}
				}
				// カリング設定
				WIN_OBJ_INFO["Geometry"].AddGroupItem(Item::CreateBind("Culling", Item::Kind::Bool, &m_bIsCulling));
				// Uvオフセット,スケールを初期化
				InitUv();	
			}
		}
		else
		{
			*pIsTex = m_bIsTex;
		}
	});

	return pUseTexture;
}

/* ========================================
	テクスチャ全コピー初期化関数
	-------------------------------------
	内容：テクスチャ全コピーの初期化
	-------------------------------------
	戻り値：DebugUI::Item*	リスト項目
=========================================== */
DebugUI::Item* ComponentGeometry::InitTexAllCopy()
{
	using namespace DebugUI;

	Item* pInitTexAllCopy = Item::CreateCallBack("TexAllCopy_0", Item::Kind::Command, [this](bool isWrite, void* arg)
	{
		// 現在設定されている画像からリストの表示項目を設定
		std::unordered_map<TextureManager::E_TEX_KEY, std::shared_ptr<Texture>> texMap = TEXTURE_MNG_INST.GetTextureDatas();
		for (int i = 0; i < MAX_TEXTURE_NUM; i++)
		{
			m_pTextures[i] = m_pTextures[0];

			// 現在設定されている画像からリストの表示項目を設定
			std::string sTexName = "Texture_" + std::to_string(i);	// 項目名を取得
			for (auto& tex : texMap)
			{
				if (tex.second.get() == m_pTextures[i])
				{
					WIN_OBJ_INFO["Geometry"][sTexName.c_str()].SetListNo(tex.first);
					break;
				}
			}
		}
	}, false, true);

		
	return pInitTexAllCopy;
}

/* ========================================
	Uvオフセット,スケール初期化関数
	-------------------------------------
	内容：Uvオフセット,スケールの初期化
=========================================== */
void ComponentGeometry::InitUv()
{
	using namespace DebugUI;

	// 立方体の場合(6面分のテクスチャオフセット、スケールを設定)
	if (m_eType == TYPE_BOX)
	{
		for (int i = 0; i < MAX_UV_NUM; i++)
		{
			std::string sUvScaleName = "UvScale_" + mapUvName[i];
			std::string sUvOffsetName = "UvOffset_" + mapUvName[i];
			// テクスチャスケール
			WIN_OBJ_INFO["Geometry"].AddGroupItem(Item::CreateBind(sUvScaleName.c_str(), Item::Kind::Vector2, &m_fUvScale[i]));
			// テクスチャオフセット
			WIN_OBJ_INFO["Geometry"].AddGroupItem(Item::CreateBind(sUvOffsetName.c_str(), Item::Kind::Vector2, &m_fUvOffset[i]));
		}
	}
	// 球、平面の場合(1つのテクスチャオフセット、スケールを設定)
	else
	{
		// テクスチャスケール
		WIN_OBJ_INFO["Geometry"].AddGroupItem(Item::CreateBind("UvScale", Item::Kind::Vector2, &m_fUvScale[0]));
		// テクスチャオフセット
		WIN_OBJ_INFO["Geometry"].AddGroupItem(Item::CreateBind("UvOffset", Item::Kind::Vector2, &m_fUvOffset[0]));
	}
}

/* ========================================
	Uvオフセット,スケール削除関数
	-------------------------------------
	内容：Uvオフセット,スケールの削除
=========================================== */
void ComponentGeometry::RemoveUv()
{
	using namespace DebugUI;

	// 立方体の場合(6面分)と球、平面の場(1面)の両パターンの項目を削除
	for (int i = 0; i < MAX_UV_NUM; i++)
	{
		std::string sUvScaleName = "UvScale_" + mapUvName[i];
		std::string sUvOffsetName = "UvOffset_" + mapUvName[i];
		WIN_OBJ_INFO["Geometry"].RemoveGroupItem(sUvScaleName.c_str());
		WIN_OBJ_INFO["Geometry"].RemoveGroupItem(sUvOffsetName.c_str());
	}

	// テクスチャスケール
	WIN_OBJ_INFO["Geometry"].RemoveGroupItem("UvScale");
	// テクスチャオフセット
	WIN_OBJ_INFO["Geometry"].RemoveGroupItem("UvOffset");
}

#endif // _DEBUG