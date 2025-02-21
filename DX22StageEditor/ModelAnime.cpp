/* ========================================
	DX22Base/
	------------------------------------
	アニメーションモデル用cpp
	------------------------------------
	ModelAnime.cpp
========================================== */


// =============== インクルード ===================
#include "ModelAnime.h"
#include "ShaderManager.h"
#include "ShadowManager.h"

#include "LightManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#if _MSC_VER >= 1930
	#ifdef _DEBUG
		#pragma comment(lib, "assimp-vc143-mtd.lib")
	#else
		#pragma comment(lib, "assimp-vc143-mt.lib")
	#endif
#elif _MSC_VER >= 1920
	#ifdef _DEBUG
		#pragma comment(lib, "assimp-vc142-mtd.lib")
	#else
		#pragma comment(lib, "assimp-vc142-mt.lib")
	#endif
#elif _MSC_VER >= 1910
	#ifdef _DEBUG
		#pragma comment(lib, "assimp-vc141-mtd.lib")
	#else
		#pragma comment(lib, "assimp-vc141-mt.lib")
	#endif
#endif







/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
ModelAnime::ModelAnime()
	: m_fLoadScale(1.0f)
	, m_LoadFlip(None)
	, m_nActiveNo(ANIME_NONE)
	, m_nBlendNo(ANIME_NONE)
	, m_nParametricAnimeNos{ ANIME_NONE, ANIME_NONE }
	, m_fBlendTime(0.0f)
	, m_fBlendTimeTotal(0.0f)
	, m_fParametricBlendRatio(0.0f)
	, m_sModelName("")
#ifdef _DEBUG
	, m_pBoneLine(nullptr)
	, m_nBoneCnt(0)
#endif // _DEBUG
{

	m_pVS = GET_VS_DATA(VS_KEY::VS_MODEL_ANIME);
	m_pPS = GET_PS_DATA(PS_KEY::PS_MODEL_ANIME);
}


/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：解放
=========================================== */
ModelAnime::~ModelAnime()
{
	Reset();
}

/* ========================================
	コピーコンストラクタ
	-------------------------------------
	内容：コピーコンストラクタ
		　※アニメモデルのディープコピー時用
=========================================== */
ModelAnime::ModelAnime(const ModelAnime& other)
	: m_fLoadScale(0.0f)
	, m_LoadFlip(E_Flip::None)
	, m_NodeList()
	, m_AnimeList()
	, m_pVS(nullptr)
	, m_pPS(nullptr)
	, m_nActiveNo(0)
	, m_nBlendNo(0)
	, m_nParametricAnimeNos{0,0}
	, m_fBlendTime(0.0f)
	, m_fBlendTimeTotal(0.0f)
	, m_fParametricBlendRatio(0.0f)
	, m_sModelName("")
#ifdef _DEBUG
	, m_pBoneLine(nullptr)
	, m_nBoneCnt(0)
#endif // _DEBUG
{
	if (this == &other)	return;

	this->m_fLoadScale				= other.m_fLoadScale;
	this->m_LoadFlip				= other.m_LoadFlip;
	this->m_NodeList				= other.m_NodeList;
	this->m_AnimeList				= other.m_AnimeList;
	this->m_pVS						= other.m_pVS;
	this->m_pPS						= other.m_pPS;
	this->m_nActiveNo				= other.m_nActiveNo;
	this->m_nBlendNo				= other.m_nBlendNo;
	this->m_nParametricAnimeNos[0]	= other.m_nParametricAnimeNos[0];
	this->m_nParametricAnimeNos[1]	= other.m_nParametricAnimeNos[1];
	this->m_fBlendTime				= other.m_fBlendTime;
	this->m_fBlendTimeTotal			= other.m_fBlendTimeTotal;
	this->m_fParametricBlendRatio	= other.m_fParametricBlendRatio;
	this->m_sModelName				= other.m_sModelName;
#ifdef _DEBUG
	this->m_pBoneLine = std::make_unique<ShapeLine>(*other.m_pBoneLine);
#endif // _DEBUG

	// 各変数が持つポインタをディープコピーする
	
	// メッシュ配列
	this->m_MeshList = other.m_MeshList;
	for (auto& mesh : this->m_MeshList)
	{
		mesh.pMesh = new MeshBuffer(*mesh.pMesh);
	}
	// マテリアル配列
	this->m_MaterialList = other.m_MaterialList;
	for (auto& mat : this->m_MaterialList)
	{
		mat.pTexture = new Texture(*mat.pTexture);
	}
	// アニメーション別変形情報

	for (int i = 0; i < MAX_TRANSFORM; ++i)
	{
		m_NodeTransform[i].resize(other.m_NodeTransform[i].size());
		for (int j = 0; j < other.m_NodeTransform[i].size(); ++j)
		{
			m_NodeTransform[i][j] = other.m_NodeTransform[i][j];
		}
	}
	

}


/* ========================================
	更新関数
	-------------------------------------
	内容：毎フレーム呼び出す更新処理
	-------------------------------------
	引数：前フレームからの経過時間
=========================================== */
void ModelAnime::Update(float tick)
{
	// アニメーションの再生確認
	if (m_nActiveNo == ANIME_NONE) { return; }

	//--- アニメーション行列の更新
	// パラメトリック
	if (m_nActiveNo == PARAMETRIC_ANIME || m_nBlendNo == PARAMETRIC_ANIME)
	{
		CalcAnime(PARAMETRIC0, m_nParametricAnimeNos[0]);
		CalcAnime(PARAMETRIC1, m_nParametricAnimeNos[1]);
	}
	// メインアニメ
	if (m_nActiveNo != ANIME_NONE && m_nActiveNo != PARAMETRIC_ANIME)
	{
		CalcAnime(MAIN, m_nActiveNo);
	}
	// ブレンドアニメ
	if (m_nBlendNo != ANIME_NONE && m_nBlendNo != PARAMETRIC_ANIME)
	{
		CalcAnime(BLEND, m_nBlendNo);
	}

	// アニメーション行列に基づいて骨行列を更新
	CalcBones(0, DirectX::XMMatrixScaling(m_fLoadScale, m_fLoadScale, m_fLoadScale));

	//--- アニメーションの時間更新
	// メインアニメ
	UpdateAnime(m_nActiveNo, tick);
	// ブレンドアニメ
	if (m_nBlendNo != ANIME_NONE)
	{
		UpdateAnime(m_nBlendNo, tick);
		m_fBlendTime += tick;
		if (m_fBlendTime >= m_fBlendTimeTotal)	// ブレンドアニメの終了判定
		{
			// ブレンドアニメの自動終了
			m_fBlendTime = 0.0f;
			m_fBlendTimeTotal = 0.0f;
			m_nActiveNo = m_nBlendNo;
			m_nBlendNo = ANIME_NONE;
		}
	}
	// パラメトリック
	if (m_nActiveNo == PARAMETRIC_ANIME || m_nBlendNo == PARAMETRIC_ANIME)
	{
		UpdateAnime(m_nParametricAnimeNos[0], tick);
		UpdateAnime(m_nParametricAnimeNos[1], tick);
	}
}

/* ========================================
	描画処理関数
	-------------------------------------
	内容：描画
	-------------------------------------
	引数：const std::vector<UINT>*	描画順番
=========================================== */
void ModelAnime::Draw(const std::vector<UINT>* order)
{
	if (m_pPS == nullptr || m_pVS == nullptr) { return; }

	// 描画数設定
	size_t drawNum = m_MeshList.size();

	// 描画順番指定がある場合
	if (order)
	{
		drawNum = order->size();
	}

	// 描画
	for (UINT i = 0; i < drawNum; ++i)
	{
		// メッシュ番号
		UINT meshNo;

		// 描画順番指定がある場合
		if (order)
		{
			meshNo = (*order)[i];	// 配列からメッシュ番号取得
		}
		else
		{
			meshNo = i;
		}

		
		// 描画コールバック
		const T_Mesh*		pMesh	= this->GetMesh(meshNo);
		const T_Material& pMaterial	= *this->GetMaterial(pMesh->materialID);

		m_pPS->SetTexture(0, pMaterial.pTexture);	// テクスチャセット
		m_pPS->Bind();


		DirectX::XMFLOAT4X4 bones[200];
		for (int i = 0; i < pMesh->bones.size() && i < 200; ++i)
		{
			// この計算はゲームつくろー「スキンメッシュの仕組み」が参考になる
			DirectX::XMStoreFloat4x4(&bones[i], DirectX::XMMatrixTranspose(
				pMesh->bones[i].invOffset *
				this->GetBone(pMesh->bones[i].index)
			));
		}
		m_pVS->WriteBuffer(1, bones);
		m_pVS->Bind();

		// 描画
		m_MeshList[meshNo].pMesh->Draw();
	}

}

/* ========================================
	リセット関数
	-------------------------------------
	内容：メッシュ配列とマテリアル配列を解放
=========================================== */
void ModelAnime::Reset()
{
	auto meshIt = m_MeshList.begin();
	while (meshIt != m_MeshList.end())
	{
		if (meshIt->pMesh)	delete meshIt->pMesh;
		++meshIt;
	}

	auto matIt = m_MaterialList.begin();
	while (matIt != m_MaterialList.end())
	{
		if (matIt->pTexture) delete matIt->pTexture;
		++matIt;
	}
}

/* ========================================
	アニメモデルデータ読込関数
	-------------------------------------
	内容：アニメモデルデータを読み込む
	-------------------------------------
	引数1: const char*	モデルデータパス
	引数2: float		モデルのサイズ
	引数3: Flip			反転設定
	-------------------------------------
	戻値：bool	true:成功	false:失敗
=========================================== */
bool ModelAnime::Load(const char* file, float scale, E_Flip flip)
{
#ifdef _DEBUG
	m_sErrorMessage = "";
#endif
	Reset();

	// assimpの設定
	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_FlipUVs;
	if (flip == E_Flip::XFlip)  flag |= aiProcess_MakeLeftHanded;

	// assimpで読み込み
	const aiScene* pScene = importer.ReadFile(file, flag);
	if (!pScene) {
#ifdef _DEBUG
		m_sErrorMessage = importer.GetErrorString();
#endif
		return false;
	}

	// 読み込み時の設定を保存
	m_fLoadScale = scale;
	m_LoadFlip = flip;

	// ディレクトリの読み取り
	std::string directory = file;
	auto strIt = directory.begin();
	while (strIt != directory.end()) {
		if (*strIt == '/')
			*strIt = '\\';
		++strIt;
	}
	directory = directory.substr(0, directory.find_last_of('\\') + 1);

	// ノードの作成
	MakeBoneNodes(pScene);
	// メッシュ作成
	MakeMesh(pScene, scale, flip);
	// マテリアルの作成
	MakeMaterial(pScene, directory);


#ifdef _DEBUG
	m_pBoneLine = std::make_unique<ShapeLine>(250);

	// 再帰処理
	std::function<void(int, DirectX::XMFLOAT3)> FuncDrawBone =
		[&FuncDrawBone, this](int idx, DirectX::XMFLOAT3 parent)
	{
		// 親ノードから現在位置まで描画
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, DirectX::XMVector3TransformCoord(DirectX::XMVectorZero(), m_NodeList[idx].mat));

		Vector3<float> start, end;
		start = parent;
		end = pos;

		m_pBoneLine->AddLine(start, end, ColorVec3::RED);
		// 子ノードの描画
		auto it = m_NodeList[idx].children.begin();
		while (it != m_NodeList[idx].children.end())
		{
			FuncDrawBone(*it, pos);
			++it;
		}
	};

	// 描画実行
	FuncDrawBone(0, DirectX::XMFLOAT3());
#endif // _DEBUG


	m_sModelName = GetFBXDataName(file);

	return true;
}


/* ========================================
	FBXデータ名取得関数
	-------------------------------------
	内容：ファイルパスからFBXデータ名(モデルやアニメーション)を取得
	-------------------------------------
	引数：const char*	ファイルパス
	-------------------------------------
	戻値：std::string	FBXデータ名
=========================================== */
std::string ModelAnime::GetFBXDataName(const char* file)
{
	std::string path = file;

	// パスから末尾のファイル名を取得
	path = path.substr(path.find_last_of('/') + 1, path.size());
	// 拡張子を削除
	const std::string extension = ".fbx";
	if (path.size() >= extension.size() &&
		path.rfind(extension) == path.size() - extension.size()) {
		// ".fbx" を削除
		path.erase(path.size() - extension.size());
	}

	return path;
}



/* ========================================
	ゲッター(メッシュ)関数
	-------------------------------------
	引数：メッシュ番号
	-------------------------------------
	戻値：Mesh*　メッシュ
=========================================== */
const ModelAnime::T_Mesh* ModelAnime::GetMesh(unsigned int index)
{
	if (index < GetMeshNum())
	{
		return &m_MeshList[index];
	}
	return nullptr;
}


/* ========================================
	ゲッター(メッシュ配列サイズ)関数
	-------------------------------------
	戻値：uint32_t　メッシュ配列サイズ
=========================================== */
uint32_t ModelAnime::GetMeshNum()
{
	return static_cast<uint32_t>(m_MeshList.size());
}

/* ========================================
	ゲッター(マテリアル)関数
	-------------------------------------
	引数：マテリアル番号
	-------------------------------------
	戻値：Material*　マテリアル
=========================================== */
const ModelAnime::T_Material* ModelAnime::GetMaterial(unsigned int index)
{
	if (index < GetMaterialNum())
	{
		return &m_MaterialList[index];
	}
	return nullptr;
}

/* ========================================
	ゲッター(マテリアル配列サイズ)関数
	-------------------------------------
	戻値：uint32_t　マテリアル配列サイズ
=========================================== */
uint32_t ModelAnime::GetMaterialNum()
{
	return static_cast<uint32_t>(m_MaterialList.size());
}


/* ========================================
	ゲッター(アニメーション後変換行列)関数
	-------------------------------------
	引数：NodeIndex	ボーン番号
	-------------------------------------
	戻値：DirectX::XMMATRIX　該当ボーンの変換行列
=========================================== */
DirectX::XMMATRIX ModelAnime::GetBone(NodeIndex index)
{
	if (index < m_NodeList.size())
	{
		return m_NodeList[index].mat;
	}
	return DirectX::XMMatrixIdentity();
}

/* ========================================
	ゲッター(アニメーション情報配列)関数
	-------------------------------------
	引数：AnimeNo アニメ番号
	-------------------------------------
	戻値：ModelAnime::T_Animation*　該当アニメーション情報
=========================================== */
const ModelAnime::T_Animation* ModelAnime::GetAnimation(AnimeNo no)
{
	if (AnimeNoCheck(no))
	{
		return &m_AnimeList[no];
	}
	return nullptr;
}

/* ========================================
	セッター(頂点シェーダー)関数
	-------------------------------------
	引数：VertexShader*　頂点シェーダー
=========================================== */
void ModelAnime::SetVertexShader(VertexShader* vs)
{
	m_pVS = vs;
}

/* ========================================
	セッター(ピクセルシェーダー)関数
	-------------------------------------
	引数：PixelShader*　ピクセルシェーダー
=========================================== */
void ModelAnime::SetPixelShader(PixelShader* ps)
{
	m_pPS = ps;
}

/* ========================================
	セッター(ワールドビュープロジェクション行列)関数
	-------------------------------------
	引数：XMFLOAT4X4*　行列ポインタ(配列)
=========================================== */
void ModelAnime::SetWVPMatrix(const DirectX::XMFLOAT4X4* matWVP)
{
	m_pVS->WriteBuffer(0, matWVP);
}


/* ========================================
	セッター(ライト設定(マテリアル))関数
	-------------------------------------
	引数1：float	拡散光
	引数2：float	鏡面反射光
	引数3：float	環境光
	引数4：bool		ライト使用フラグ
=========================================== */
void ModelAnime::SetLightMaterial(float fDiffuse, float fSpecular, float fAmbient, bool bUseLight)
{
	DirectX::XMFLOAT4 param[] =
	{
		{fDiffuse, fSpecular, fAmbient, bUseLight ? 1.0f : 0.0f}
	};

	m_pPS->WriteBuffer(0, param);
}

/* ========================================
	セッター(カメラ座標)関数
	-------------------------------------
	引数1：Vector3<float>	カメラ座標
=========================================== */
void ModelAnime::SetCameraPos(Vector3<float> fCameraPos)
{
	m_pPS->WriteBuffer(1, &fCameraPos);
}

/* ========================================
	セッター(ライト情報)関数
	-------------------------------------
	引数：std::vector<ObjectLight*>	シーン上のライトリスト
=========================================== */
void ModelAnime::SetLights(std::vector<ObjectLight*> lights)
{
	DirectX::XMFLOAT4 param[MAX_LIGHT_NUM][4];

	// 初期化
	for (int i = 0; i < MAX_LIGHT_NUM; i++)
	{
		param[i][0] = { 0.0f, 0.0f, 0.0f, 0.0f };
		param[i][1] = { 0.0f, 0.0f, 0.0f, 0.0f };
		param[i][2] = { 0.0f, 0.0f, 0.0f, 0.0f };
		param[i][3] = { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	// ライト情報をセット
	for (int i = 0; i < lights.size(); ++i)
	{
		float			lightType	= static_cast<float>(lights[i]->GetLightType());
		Vector3<float>	lightPos	= lights[i]->GetPosition();
		Vector3<float>	lightColor	= lights[i]->GetColor();
		Vector3<float>	lightDir	= lights[i]->GetDirection();

		param[i][0] = { lightType, lightPos.x, lightPos.y, lightPos.z };			// ライトタイプ、ライト位置
		param[i][1] = { lightColor.x, lightColor.y, lightColor.z, 1.0f };			// ライトカラー
		param[i][2] = { lightDir.x, lightDir.y, lightDir.z, lights[i]->GetRange() };// ライト方向、ライト有効範囲
		param[i][3] = { lights[i]->GetAngle(), 0.0f, 0.0f, 0.0f };					// スポットライト角度
	}
	// 丸影の情報をセット
	int nParamNum = lights.size();
	std::vector<ComponentShadow*> shadows = SHADOW_MNG_INST.GetShadowList();
	for (int i = 0; i < shadows.size(); i++)
	{
		if (nParamNum >= MAX_LIGHT_NUM) break;	// ライト、影合計数が最大数を超えたら終了

		Vector3<float>	vShadowPos = shadows.at(i)->GetPosition();

		param[nParamNum][0] = { 4.0f, vShadowPos.x, vShadowPos.y, vShadowPos.z };	// ライトタイプ(影は固定で4)、影の基準座標
		param[nParamNum][1] = { 0.0f, 0.0f, 0.0f, 0.0f };							// ライトカラー(未使用)
		param[nParamNum][2] = { 0.0f, 0.0f, 0.0f, shadows[i]->GetCircleSize() };	// ライト方向(未使用)、落影の円の大きさ
		param[nParamNum][3] = { 0.0f, 0.0f, 0.0f, 0.0f };							// スポットライト角度(未使用)

		nParamNum++;
	}

	m_pPS->WriteBuffer(2, param);
}




