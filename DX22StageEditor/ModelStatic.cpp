/* ========================================
	DX22Base/
	------------------------------------
	静的モデル用cpp
	------------------------------------
	ModelStatic.cpp
========================================== */

// =============== インクルード ===================
#include "ModelStatic.h"
#include "DirectXTex/TextureLoad.h"
#include "LightManager.h"
#include "ShadowManager.h"
#include "ComponentTransform.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ShaderManager.h"
#include "CameraManager.h"

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
ModelStatic::ModelStatic()
	: m_pVS(nullptr)
	, m_pPS(nullptr)
{
	m_pVS = GET_VS_DATA(VS_KEY::VS_MODEL_STATIC);
	m_pPS = GET_PS_DATA(PS_KEY::PS_MODEL_STATIC);
}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：解放
=========================================== */
ModelStatic::~ModelStatic()
{
	Reset();	// メッシュ配列とマテリアル配列を解放
}

/* ========================================
	描画処理関数
	-------------------------------------
	内容：描画
=========================================== */
void ModelStatic::Draw()
{
	if (m_pPS == nullptr || m_pVS == nullptr) { return; }

	for (unsigned int i = 0; i < m_MeshList.size(); ++i)
	{
		m_pPS->SetTexture(0,m_MaterialList[m_MeshList[i].materialID].pTexture);
		m_pPS->Bind();
		m_pVS->Bind();

		m_MeshList[i].pMesh->Draw();
	}
}

/* ========================================
	リセット関数
	-------------------------------------
	内容：メッシュ配列とマテリアル配列を解放
=========================================== */
void ModelStatic::Reset()
{
	auto meshIt = m_MeshList.begin();
	while (meshIt != m_MeshList.end())
	{
		if(meshIt->pMesh) delete meshIt->pMesh;
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
	メッシュ読込関数
	-------------------------------------
	内容：メッシュデータを読み込む
	-------------------------------------
	引数1: const char*	モデルデータパス
	引数2: float		モデルのサイズ
	引数3: Flip			反転設定
	-------------------------------------
	戻値：bool	true:成功	false:失敗
=========================================== */
bool ModelStatic::Load(const char* file, float scale, E_Flip flip)
{
#ifdef _DEBUG
	m_sErrorMessage = "";
#endif
	Reset();	// メッシュ配列とマテリアル配列を解放

	// assimpの設定
	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;				// 三角形化
	flag |= aiProcess_JoinIdenticalVertices;	// 同一頂点の結合
	flag |= aiProcess_Triangulate;				// 三角形化
	flag |= aiProcess_FlipUVs;					// UV反転
	flag |= aiProcess_PreTransformVertices;		// 頂点の変換

	// 反転設定
	if (flip == E_Flip::XFlip)  flag |= aiProcess_MakeLeftHanded;	// 左手座標系

	// assimpで読み込み
	const aiScene* pScene = importer.ReadFile(file, flag);
	if (!pScene) {
#ifdef _DEBUG
		m_sErrorMessage = importer.GetErrorString();
#endif
		return false;
	}

	// ディレクトリの読み取り
	std::string directory = file;
	auto strIt = directory.begin();
	while (strIt != directory.end()) {
		if (*strIt == '/')
			*strIt = '\\';
		++strIt;
	}
	directory = directory.substr(0, directory.find_last_of('\\') + 1);

	// メッシュ作成
	MakeMesh(pScene, scale, flip);
	// マテリアルの作成
	MakeMaterial(pScene, directory);

	return true;
}


/* ========================================
	ゲッター(メッシュ)関数
	-------------------------------------
	引数：メッシュ番号
	-------------------------------------
	戻値：Mesh*　メッシュ
=========================================== */
const ModelStatic::T_Mesh* ModelStatic::GetMesh(unsigned int index)
{
	// 範囲外チェック
	if (index < GetMeshSize())
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
uint32_t ModelStatic::GetMeshSize()
{
	return static_cast<uint32_t>(m_MeshList.size());
}


/* ========================================
	セッター(頂点シェーダー)関数
	-------------------------------------
	引数：VertexShader*　頂点シェーダー
=========================================== */
void ModelStatic::SetVertexShader(VertexShader* vs)
{
	m_pVS = vs;
}

/* ========================================
	セッター(ピクセルシェーダー)関数
	-------------------------------------
	引数：PixelShader*　ピクセルシェーダー
=========================================== */
void ModelStatic::SetPixelShader(PixelShader* ps)
{
	m_pPS = ps;
}

/* ========================================
	セッター(ワールドビュープロジェクション行列)関数
	-------------------------------------
	引数：XMFLOAT4X4*　行列ポインタ(配列)
=========================================== */
void ModelStatic::SetWVPMatrix(const DirectX::XMFLOAT4X4* matWVP)
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
void ModelStatic::SetLightMaterial(float fDiffuse, float fSpecular, float fAmbient, bool bUseLight)
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
void ModelStatic::SetCameraPos(Vector3<float> fCameraPos)
{
	m_pPS->WriteBuffer(1, &fCameraPos);
}

/* ========================================
	セッター(ライト情報)関数
	-------------------------------------
	引数：std::vector<ObjectLight*>	シーン上のライトリスト
=========================================== */
void ModelStatic::SetLights(std::vector<ObjectLight*> lights)
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
		float			lightType = static_cast<float>(lights[i]->GetLightType());
		Vector3<float>	lightPos = lights[i]->GetPosition();
		Vector3<float>	lightColor = lights[i]->GetColor();
		Vector3<float>	lightDir = lights[i]->GetDirection();

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


#ifdef _DEBUG
/* ========================================
	ゲッター(エラーメッセージ)関数
	-------------------------------------
	戻値：string　エラーメッセージ
=========================================== */
std::string ModelStatic::GetError()
{
	return m_sErrorMessage;
}
#endif