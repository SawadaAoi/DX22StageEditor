/* ========================================
	DX22Base/
	------------------------------------
	アニメーションモデル用ヘッダ
	------------------------------------
	説明：アニメーションモデルを扱うためのクラス
		　※ブレンド再生は、アニメA→アニメBへ滑かに遷移する再生方法
		　※合成再生は、アニメAとアニメBを合成して再生する再生方法
	------------------------------------
	ModelAnime.h
========================================== */
#pragma once

// =============== インクルード ===================
#include <DirectXMath.h>
#include <vector>
#include "VertexShader.h"
#include "PixelShader.h"
#include "MeshBuffer.h"
#include <functional>
#include "ShapeLine.h"
#include <memory>
#include "ColorVec3.h"
#include "ObjectLight.h"

#include <assimp/Importer.hpp>


// =============== クラス定義 =====================
class ModelAnime
{
public:
	// モデル反転設定
	enum E_Flip
	{
		None,			// DirectX準拠(反転する
		XFlip,			// Maya準拠
		ZFlip,			// DirectX準拠(Mayaから180回転した状態
		ZFlipUseAnime,	// DirecX準拠(アニメーションさせる場合
	};

private:
	// アニメーション計算領域
	enum E_AnimeTransform
	{
		MAIN,			// 通常再生
		BLEND,			// ブレンド再生
		PARAMETRIC0,	// 合成A
		PARAMETRIC1,	// 合成B
		MAX_TRANSFORM
	};

public:
	// 型定義
	using NodeIndex = int;	// ボーン(階層)番号
	using AnimeNo = int;	// アニメーション番号

	// 定数定義
	static const NodeIndex	INDEX_NONE = -1;		// 該当ノードなし
	static const AnimeNo	ANIME_NONE = -1;		// 該当アニメーションなし
	static const AnimeNo	PARAMETRIC_ANIME = -2;	// 合成アニメーション

private:
	// 内部型定義
	using Children = std::vector<NodeIndex>;	// ノード階層情報

	// 内部定数定義
	static const UINT		MAX_BONE = 200;	// １メッシュの最大ボーン数(ここを変更する場合.hlsl側の定義も変更する

	// アニメーションの変換情報
	struct T_Transform
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT4	quaternion;
		DirectX::XMFLOAT3	scale;
	};
	using Key = std::pair<float, T_Transform>;		// 時間 / 変換情報
	using Timeline = std::map<float, T_Transform>;	// 時間 / 変換情報
	using Transforms = std::vector<T_Transform>;

	// アニメーションとボーンの関連付け情報
	struct T_Channel
	{
		NodeIndex	index;		// ボーン番号
		Timeline	timeline;	// 変換情報
	};
	using Channels = std::vector<T_Channel>;

	// ボーン情報
	struct T_Node
	{
		std::string			name;		// ボーン名
		NodeIndex			parent;		// 親ボーン
		Children			children;	// 子ボーン
		DirectX::XMMATRIX	mat;		// 変換行列
	};
	using Nodes = std::vector<T_Node>;


public:
	// 頂点情報
	struct T_Vertex
	{
		DirectX::XMFLOAT3	pos;
		DirectX::XMFLOAT3	normal;
		DirectX::XMFLOAT2	uv;
		DirectX::XMFLOAT4	color;
		float				weight[4];
		unsigned int		index[4];
	};
	using Vertices = std::vector<T_Vertex>;
	using Indices = std::vector<unsigned long>;

	// 頂点の骨変形情報
	struct T_Bone
	{
		NodeIndex index;
		DirectX::XMMATRIX invOffset;
	};
	using Bones = std::vector<T_Bone>;

	// メッシュ
	struct T_Mesh
	{
		Vertices		vertices;
		Indices			indices;
		unsigned int	materialID;
		Bones			bones;
		MeshBuffer*		pMesh;
	};
	using Meshes = std::vector<T_Mesh>;

	// マテリアル情報
	struct T_Material
	{
		DirectX::XMFLOAT4	diffuse;	// 拡散光(メインカラー
		DirectX::XMFLOAT4	ambient;	// 環境光(陰の部分のカラー
		DirectX::XMFLOAT4	specular;	// 鏡面反射光(強く光る部分のカラー
		Texture*			pTexture;	// テクスチャ
	};
	using Materials = std::vector<T_Material>;

	// アニメーション情報
	struct T_Animation
	{
		float		nowTime;	// 現在の再生時間
		float		totalTime;	// 最大再生時間
		float		speed;		// 再生速度
		bool		isLoop;		// ループ指定
		Channels	channels;	// 変換情報
		std::string sName;		// アニメーション名
	};
	using Animations = std::vector<T_Animation>;

public:
	ModelAnime();
	~ModelAnime();
	ModelAnime(const ModelAnime& other);
	void Update(float tick);					// アニメーション更新
	void Draw(const std::vector<UINT>* order = nullptr);

	void Reset();
	bool Load(const char* file, float scale = 1.0f, E_Flip flip = E_Flip::None);

	// ゲッター
	const T_Mesh* GetMesh(unsigned int index);
	uint32_t GetMeshNum();
	const T_Material* GetMaterial(unsigned int index);
	uint32_t GetMaterialNum();
	DirectX::XMMATRIX GetBone(NodeIndex index);
	const T_Animation* GetAnimation(AnimeNo no);

	// セッター
	void SetVertexShader(VertexShader* vs);
	void SetPixelShader(PixelShader* ps);
	void SetWVPMatrix(const DirectX::XMFLOAT4X4* matWVP);										// ワールド行列の設定(頂点シェーダー用)
	void SetLightMaterial(float fDiffuse, float fSpecular, float fAmbient, bool bUseLight);		// ライトパラメータ設定(ピクセルシェーダー用)
	void SetCameraPos(Vector3<float> fCameraPos);												// カメラ位置設定(ピクセルシェーダー用)
	void SetLights(std::vector<ObjectLight*> lights);											// ライト設定(ピクセルシェーダー用)

public:
	//--- アニメーション
	AnimeNo AddAnimation(const char* file);		// アニメーションの追加

	void Play(AnimeNo no, bool loop, float speed = 1.0f);						// 再生
	void PlayBlend(AnimeNo no, float blendTime, bool loop, float speed = 1.0f);	// ブレンド再生
	void SetParametric(AnimeNo no1, AnimeNo no2);								// 合成設定
	void SetParametricBlend(float blendRate);									// 合成割合設定
	void SetAnimationTime(AnimeNo no, float time);								// 現在再生時間を変更
	void SetAnimationTimeRatio(AnimeNo no, float rate);						// 再生割合を変更
	float GetAnimeNowTime(AnimeNo no);											// 現在再生時間を取得
	float GetAnimeTotalTime(AnimeNo no);										// アニメーションの総再生時間を取得
	float GetAnimePlaybackRatio(AnimeNo no);									// アニメーション再生割合を取得
	void SetAnimationSpeed(AnimeNo no, float speed);							// 再生速度を変更

	bool IsPlay(AnimeNo no);	// 再生中かどうか
	AnimeNo GetPlayNo();		// 再生中のアニメ番号
	AnimeNo GetBlendNo();		// ブレンド中のアニメ番号
	std::string GetModelName();	// モデル名取得

#ifdef _DEBUG
	static std::string GetError();
	void DrawBone(Vector3<float> vPos = { 0.0f,0.0f,0.0f }, Quaternion qRot = {}, Vector3<float> vScale = {1.0f,1.0f,1.0f});
#endif


private:
	// 各種生成
	void MakeMesh(const void* ptr, float scale, E_Flip flip);
	void MakeMaterial(const void* ptr, std::string directory);
	void MakeAnimation(const void* ptr);
	void MakeBoneNodes(const void* ptr);
	void MakeWeight(const void* ptr, int meshIdx);

	// Assimp行列変換
	static DirectX::XMMATRIX GetMatrixFromAssimpMatrix(aiMatrix4x4 M);


	// 内部計算
	bool AnimeNoCheck(AnimeNo no);
	void InitAnime(AnimeNo no);
	void CalcAnime(E_AnimeTransform kind, AnimeNo no);
	void UpdateAnime(AnimeNo no, float tick);
	void CalcBones(NodeIndex node, const DirectX::XMMATRIX parent);
	void LerpTransform(T_Transform* pOut, const T_Transform& a, const T_Transform& b, float rate);

	std::string GetFBXDataName(const char* file);
private:
#ifdef _DEBUG
	static inline std::string m_sErrorMessage;
	std::unique_ptr<ShapeLine> m_pBoneLine;
	int m_nBoneCnt;
#endif

private:
	float			m_fLoadScale;	// 読込時設定スケール
	E_Flip			m_LoadFlip;		// 読込時設定反転

	Meshes			m_MeshList;		// メッシュ配列
	Materials		m_MaterialList;	// マテリアル配列
	Nodes			m_NodeList;		// 階層情報
	Animations		m_AnimeList;		// アニメ配列
	VertexShader*	m_pVS;			// 設定中の頂点シェーダ
	PixelShader*	m_pPS;			// 設定中のピクセルシェーダ

	AnimeNo			m_nActiveNo;			// 現在再生中のアニメ番号
	AnimeNo			m_nBlendNo;				// ブレンド再生を行うアニメ番号
	AnimeNo			m_nParametricAnimeNos[2];	// 合成再生を行うアニメ番号
	float			m_fBlendTime;			// 現在の遷移経過時間
	float			m_fBlendTimeTotal;		// アニメ遷移にかかる合計時間
	float			m_fParametricBlendRatio;			// パラメトリックの再生割合

	Transforms		m_NodeTransform[MAX_TRANSFORM];	// アニメーション別変形情報

	std::string		m_sModelName;	// ディレクトリパス
};
