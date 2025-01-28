/* ========================================
	DX22Base/
	------------------------------------
	コンポーネント(アニメーションモデル)用cpp
	------------------------------------
	ComponentModelAnime.cpp
========================================== */

// =============== インクルード ===================
#include "ComponentModelAnime.h"
#include "ModelAnime.h"
#include "ComponentTransform.h"
#include "ObjectBase.h"
#include "CameraManager.h"
#include "ModelAnimeManager.h"
#include "LightManager.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有者オブジェクト
=========================================== */
ComponentModelAnime::ComponentModelAnime(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderModelAnim)
	, m_pModel(nullptr)
	, m_pCompTran(nullptr)
	, m_bIsVisible(true)
	, m_vPosOffset(0.0f, 0.0f, 0.0f)
#ifdef _DEBUG
	, m_bDispBone(false)
	, m_bAnimeLoop(false)
	, m_fAnimeSpeed(1.0f)
	, m_nModelKey(ModelAnimeManager::E_ANIME_BASE_KEY::AB_MAX)
	, m_nAnimeKey(0)
	, m_pDelModel(nullptr)
#endif // _DEBUG
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化
=========================================== */
void ComponentModelAnime::Init()
{
	m_pCompTran = m_pOwnerObj->GetComponent<ComponentTransform>();
}

/* ========================================
	終了関数
	-------------------------------------
	内容：終了処理
=========================================== */
void ComponentModelAnime::Uninit()
{
	if (m_pModel)
	{
		SAFE_DELETE(m_pModel);
	}
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void ComponentModelAnime::Update()
{
	if (!m_pModel) return;

	m_pModel->Update(DELTA_TIME);	// アニメーション更新

#ifdef _DEBUG

	if (m_pDelModel)
	{
		SAFE_DELETE(m_pDelModel);
		m_pDelModel = nullptr;
	}
#endif // _DEBUG
}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
=========================================== */
void ComponentModelAnime::Draw()
{
	if (!m_pModel) return;
	if (!m_bIsVisible) return;

	// ワールド行列作成
	DirectX::XMFLOAT4X4 matWVP[3];
	matWVP[0] = m_pCompTran->GetWorldMatrixOffset(m_vPosOffset);
	matWVP[1] = CAMERA_MNG_INST.GetActiveCamera()->GetViewMatrix();
	matWVP[2] = CAMERA_MNG_INST.GetActiveCamera()->GetProjectionMatrix();

	m_pModel->SetWVPMatrix(matWVP);	// ワールド行列をセット

	ObjectBase::T_LightParam lightParam = m_pOwnerObj->GetLightMaterial();
	m_pModel->SetLightMaterial(lightParam.fDiffuse, lightParam.fSpecular, lightParam.fAmbient, lightParam.bLightUse);					// ライトパラメータ設定
	m_pModel->SetLights(LIGHT_MNG_INST.GetLightList());	// ライト設定
	m_pModel->SetCameraPos(CAMERA_MNG_INST.GetActiveCamera()->GetComponent<ComponentTransform>()->GetPosition());	// カメラ位置設定

#ifdef _DEBUG
	if (m_bDispBone)	// ボーン表示
		m_pModel->DrawBone(m_pCompTran->GetPosition(), m_pCompTran->GetRotation(), m_pCompTran->GetScale());
	else
		m_pModel->Draw();	// 描画
#else
	m_pModel->Draw();	// 描画

#endif // _DEBUG
}

/* ========================================
	アニメ再生関数
	-------------------------------------
	内容：アニメーション再生
	-------------------------------------
	引数1：アニメ番号
	引数2：ループ再生フラグ
	引数3：再生速度
=========================================== */
void ComponentModelAnime::PlayAnime(int animeNo, bool loop, float speed)
{
	if (!m_pModel) return;
	m_pModel->Play(animeNo, loop, speed);

}

/* ========================================
	アニメ再生(ブレンド)関数
	-------------------------------------
	内容：アニメーション再生(ブレンド)
	-------------------------------------
	引数1：アニメ番号
	引数2：ブレンド時間
	引数3：ループ再生フラグ
	引数4：再生速度
=========================================== */
void ComponentModelAnime::PlayAnimeBlend(int animeNo, float blendTime, bool loop, float speed)
{
	if (!m_pModel) return;
	m_pModel->PlayBlend(animeNo, blendTime, loop, speed);
}

/* ========================================
	アニメ再生(合成)関数
	-------------------------------------
	内容：アニメーション再生(合成)
	-------------------------------------
	引数1：アニメ番号1
	引数2：アニメ番号2
	引数3：合成率
=========================================== */
void ComponentModelAnime::PlayAnimeParametric(int animeNo1, int animeNo2, float blendRate)
{
	if (!m_pModel) return;
	m_pModel->SetParametric(animeNo1, animeNo2);
	m_pModel->SetParametricBlend(blendRate);
	m_pModel->Play(ModelAnime::PARAMETRIC_ANIME, true);
}

/* ========================================
	アニメ再生(合成・ブレンド)関数
	-------------------------------------
	内容：アニメーション再生(合成・ブレンド)
	-------------------------------------
	引数1：アニメ番号1
	引数2：アニメ番号2
	引数3：合成率
	引数4：ブレンド時間
	引数5：ループ再生フラグ
	引数6：再生速度
=========================================== */
void ComponentModelAnime::PlayAnimeParametricBlend(int animeNo1, int animeNo2, float blendRate, float blendTime, bool loop, float speed)
{
	if (!m_pModel) return;
	m_pModel->SetParametric(animeNo1, animeNo2);
	m_pModel->SetParametricBlend(blendRate);
	m_pModel->PlayBlend(ModelAnime::PARAMETRIC_ANIME, blendTime, loop, speed);
}

/* ========================================
	アニメ再生中判定関数
	-------------------------------------
	内容：アニメーション再生中か判定
	-------------------------------------
	引数1：アニメ番号
	-------------------------------------
	戻り値：bool	再生中かどうか
=========================================== */
bool ComponentModelAnime::GetIsPlayAnime(int animeNo)
{
	if (!m_pModel) return false;
	return m_pModel->IsPlay(animeNo);
}

/* ========================================
	アニメ再生時間取得関数
	-------------------------------------
	内容：アニメーション再生時間取得
	-------------------------------------
	引数1：アニメ番号
	-------------------------------------
	戻り値：float	再生時間
=========================================== */
float ComponentModelAnime::GetAnimeNowTime(int animeNo)
{
	if (!m_pModel) return 0.0f;
	return m_pModel->GetAnimeNowTime(animeNo);
}

/* ========================================
	アニメ総再生時間取得関数
	-------------------------------------
	内容：アニメーション総再生時間取得
	-------------------------------------
	引数1：アニメ番号
	-------------------------------------
	戻り値：float	総再生時間
=========================================== */
float ComponentModelAnime::GetAnimeTotalTime(int animeNo)
{
	if (!m_pModel) return 0.0f;
	return m_pModel->GetAnimeTotalTime(animeNo);
}

/* ========================================
	アニメ再生割合取得関数
	-------------------------------------
	内容：アニメーション再生割合取得
	-------------------------------------
	引数1：アニメ番号
	-------------------------------------
	戻り値：float	再生割合
=========================================== */
float ComponentModelAnime::GetAnimePlaybackRatio(int animeNo)
{
	if (!m_pModel) return 0.0f;
	return m_pModel->GetAnimePlaybackRatio(animeNo);
}

/* ========================================
	ゲッター(モデル)関数
	-------------------------------------
	戻値：ModelAnime*	モデルポインタ
=========================================== */
ModelAnime* ComponentModelAnime::GetModel()
{
	return m_pModel;
}

/* ========================================
	ゲッター(表示フラグ)関数
	-------------------------------------
	戻値：bool	表示フラグ
=========================================== */
bool ComponentModelAnime::GetIsVisible()
{
	return m_bIsVisible;
}

/* ========================================
	ゲッター(位置オフセット)関数
	-------------------------------------
	戻値：Vector3<float>	位置オフセット
=========================================== */
Vector3<float> ComponentModelAnime::GetPosOffset()
{
	return m_vPosOffset;
}

/* ========================================
	セッター(アニメ再生割合)関数
	-------------------------------------
	引数1：アニメ番号
	引数2：再生割合
=========================================== */
void ComponentModelAnime::SetAnimeTimeRatio(int animeNo, float rate)
{
	if (!m_pModel) return;
	m_pModel->SetAnimationTimeRatio(animeNo, rate);
}

/* ========================================
	セッター(モデル)関数
	-------------------------------------
	引数：ModelAnime*	pModel	モデルポインタ
=========================================== */
void ComponentModelAnime::SetModel(ModelAnime* pModel)
{
	m_pModel = pModel;
}

/* ========================================
	セッター(表示フラグ)関数
	-------------------------------------
	引数：bool	bIsVisible	表示フラグ
=========================================== */
void ComponentModelAnime::SetIsVisible(bool bIsVisible)
{
	m_bIsVisible = bIsVisible;
}

/* ========================================
	セッター(位置オフセット)関数
	-------------------------------------
	引数：Vector3<float>	offset	位置オフセット
=========================================== */
void ComponentModelAnime::SetPosOffset(Vector3<float> offset)
{
	m_vPosOffset = offset;
}

#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentModelAnime::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupModel = Item::CreateGroup("ModelAnime");

	pGroupModel->AddGroupItem(Item::CreateBind("IsVisible ", Item::Kind::Bool, &m_bIsVisible));
	pGroupModel->AddGroupItem(Item::CreateBind("PosOffset  ", Item::Kind::Vector, &m_vPosOffset));
	pGroupModel->AddGroupItem(Item::CreateBind("DispBone ", Item::Kind::Bool, &m_bDispBone));

	pGroupModel->AddGroupItem(InitModelList());	// モデルリスト


	pGroupModel->AddGroupItem(Item::CreateBind("Loop", Item::Kind::Bool, &m_bAnimeLoop));
	pGroupModel->AddGroupItem(Item::CreateBind("Speed", Item::Kind::Float, &m_fAnimeSpeed));
	pGroupModel->AddGroupItem(Item::CreateList("Animation", [this](const void* arg)
	{
		ClickAnimeList(reinterpret_cast<const char*>(arg));
	}, false, true));	// アニメリスト

	window.AddItem(pGroupModel);

	InitAnimeList();

}

/* ========================================
	モデルリスト初期化関数
	-------------------------------------
	内容：モデルリストの初期化
	-------------------------------------
	戻り値：モデルリスト
=========================================== */
DebugUI::Item* ComponentModelAnime::InitModelList()
{
	using namespace DebugUI;

	// モデルリスト作成
	DebugUI::Item* pModelList = Item::CreateList("Model", [this](const void* arg)
	{
		// リストクリック時の処理
		ClickModelList(reinterpret_cast<const char*>(arg));

	}, false, true);

	// 登録されているモデルをリストに追加
	std::unordered_map<ModelAnimeManager::E_ANIME_BASE_KEY, std::string> modelPathMap = MODEL_ANIME_MNG_INST.GetModelPaths();
	for (auto& modelPath : modelPathMap)
	{
		pModelList->AddListItem(modelPath.second.c_str());
	}

	// 現在のモデルを選択
	std::unordered_map<ModelAnimeManager::E_ANIME_BASE_KEY, std::shared_ptr<ModelAnime>> modelMap = MODEL_ANIME_MNG_INST.GetModelDatas();
	for (auto& model : modelMap)
	{
		// モデル名が一致したら
		if (model.second->GetModelName() == m_pModel->GetModelName())
		{
			pModelList->SetListNo(model.first);
			m_nModelKey = model.first;
			break;
		}
	}


	return pModelList;
}

/* ========================================
	モデルリストクリック関数
	-------------------------------------
	内容：モデルリストクリック時の処理
	-------------------------------------
	引数：リストに表示されている文字列
=========================================== */
void ComponentModelAnime::ClickModelList(std::string sListName)
{
	ModelAnimeManager::E_ANIME_BASE_KEY eKey = ModelAnimeManager::E_ANIME_BASE_KEY::AB_MAX;

	// 前のモデルを削除用変数にセット(不具合回避のため後で削除)
	m_pDelModel = m_pModel;

	// モデルキー取得
	std::unordered_map<ModelAnimeManager::E_ANIME_BASE_KEY, std::string> modelPathMap = MODEL_ANIME_MNG_INST.GetModelPaths();
	for (auto& modelPath : modelPathMap)
	{
		if (modelPath.second == sListName)
		{
			eKey = modelPath.first;
			break;
		}
	}
	// モデルデータ取得
	m_pModel = GET_MODEL_ANIME(eKey);
	m_nModelKey = eKey;

	m_pModel->Play(0, m_bAnimeLoop, m_fAnimeSpeed);	// 先頭のアニメーション再生
	InitAnimeList();	// アニメリスト再読み込み
}

/* ========================================
	アニメリスト初期化関数
	-------------------------------------
	内容：アニメリストの初期化
=========================================== */
void ComponentModelAnime::InitAnimeList()
{
	using namespace DebugUI;

	DebugUI::Item* m_pAnimeList = &WIN_OBJ_INFO["ModelAnime"]["Animation"];		// アニメリスト取得
	reinterpret_cast<DebugUI::ItemList*>(m_pAnimeList)->RemoveListItemAll();	// リストクリア

	// 現在のアニメーション名取得
	std::string target = m_pModel->GetAnimation(m_pModel->GetPlayNo())->sName;	
	// リスト選択番号
	int nSelect = 0;		

	// 登録されているアニメーションをリストに追加
	std::unordered_map <int, std::string> modelPathMap = MODEL_ANIME_MNG_INST.GetAnimePaths(m_nModelKey);
	for (auto& modelPath : modelPathMap)
	{
		m_pAnimeList->AddListItem(modelPath.second.c_str());

		// 表示名称に含まれているか
		if (modelPath.second.find(target) != std::string::npos) 
			nSelect = modelPath.first;
	}

	// 現在のモデルを選択
	m_pAnimeList->SetListNo(nSelect);
	m_nAnimeKey = -1;

}

/* ========================================
	アニメリストクリック関数
	-------------------------------------
	内容：アニメリストクリック時の処理
	-------------------------------------
	引数：リストに表示されている文字列
=========================================== */
void ComponentModelAnime::ClickAnimeList(std::string sListName)
{
	// リスト名からアニメーションキー取得
	std::unordered_map <int, std::string> modelPathMap = MODEL_ANIME_MNG_INST.GetAnimePaths(m_nModelKey);
	for (auto& modelPath : modelPathMap)
	{
		if(sListName == modelPath.second)
		{
			m_pModel->Play(modelPath.first, m_bAnimeLoop, m_fAnimeSpeed);
			m_nAnimeKey = modelPath.first;
			break;
		}
	}
}
#endif // _DEBUG
