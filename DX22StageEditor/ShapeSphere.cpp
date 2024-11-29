/* ========================================
	DX22Base/
	------------------------------------
	図形描画(球)用cpp
	------------------------------------
	ShapeSphere.cpp
========================================== */

// =============== インクルード ===================
#include "ShapeSphere.h"
#include "vector"
#include <DirectXMath.h>

// =============== 定数定義 =======================
// 通常
const int LONGITUDE_COUNT_NORMAL	= 20;	// 経度の数(縦線)
const int LATITUDE_COUNT_NORMAL		= 20;	// 緯度の数(横線)
// ワイヤーフレーム
const int LONGITUDE_COUNT_WIRE		= 15;	// 経度の数(縦線)
const int LATITUDE_COUNT_WIRE		= 15;	// 緯度の数(横線)

const float RADIUS					= 0.5f;	// 半径

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
ShapeSphere::ShapeSphere()
{
	MakeMesh();	// メッシュの作成
}

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：fPos		座標
	引数2：fSize	サイズ
	引数3：fColor	色
	引数4：eMode	描画モード
=========================================== */
ShapeSphere::ShapeSphere(Vector3<float> fPos, Vector3<float> fSize, Vector3<float> fColor, E_DrawMode eMode)
{
	SetPosition(fPos);
	SetScale(fSize);
	SetColor(fColor);
	SetDrawMode(eMode);

	//MakeMesh();	// メッシュの作成
}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：解放処理
=========================================== */
ShapeSphere::~ShapeSphere()
{
}

/* ========================================
	メッシュ作成関数
	-------------------------------------
	内容：メッシュを作成する
=========================================== */
void ShapeSphere::MakeMesh()
{
	MakeMeshNormal();
	MakeMeshWire();
}

/* ========================================
	メッシュ作成(通常)関数
	-------------------------------------
	内容：メッシュを作成する(通常)
=========================================== */
void ShapeSphere::MakeMeshNormal()
{
	// 球の頂点データとインデックスデータをvectorで作成
	std::vector<T_Vertex> vertices = CreateSphereVertices(LONGITUDE_COUNT_NORMAL, LONGITUDE_COUNT_NORMAL);
	std::vector<int> indices = CreateSphereIndices(LONGITUDE_COUNT_NORMAL, LONGITUDE_COUNT_NORMAL);

	// バッファの作成
	MeshBuffer::T_MeshData desc = {};
	desc.pVtx					= vertices.data();							// 頂点データ
    desc.vtxCount				= static_cast<UINT>(vertices.size());		// 頂点の数(std::sizeは配列の数を数える)
	desc.vtxSize				= sizeof(T_Vertex);							// 頂点一つあたりのデータサイズ
	desc.pIdx					= indices.data();										// インデックスデータ(頂点の順番)
	desc.idxCount				= static_cast<UINT>(indices.size());		// インデックスの数
	desc.idxSize				= sizeof(int);								// インデックス一つあたりのデータサイズ
	desc.topology				= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	// トポロジーの設定(三角形リスト)
	m_pMeshBuffer[DRAW_NORMAL]	= new MeshBuffer(desc);						// メッシュバッファの作成

}

/* ========================================
	メッシュ作成(ワイヤフレーム)関数
	-------------------------------------
	内容：メッシュを作成する(ワイヤフレーム)
=========================================== */
void ShapeSphere::MakeMeshWire()
{
	// 球の頂点データとインデックスデータをvectorで作成
	std::vector<T_Vertex> vertices	= CreateSphereVertices(LATITUDE_COUNT_WIRE, LONGITUDE_COUNT_WIRE);
	std::vector<int> indices		= CreateSphereIndices(LATITUDE_COUNT_WIRE, LONGITUDE_COUNT_WIRE);

	// バッファの作成
	MeshBuffer::T_MeshData desc = {};
	desc.pVtx					= vertices.data();						// 頂点データ
	desc.vtxCount				= static_cast<UINT>(vertices.size());	// 頂点の数(std::sizeは配列の数を数える)
	desc.vtxSize				= sizeof(T_Vertex);						// 頂点一つあたりのデータサイズ
	desc.pIdx					= indices.data();									// インデックスデータ(頂点の順番)
	desc.idxCount				= static_cast<UINT>(indices.size());	// インデックスの数
	desc.idxSize				= sizeof(int);							// インデックス一つあたりのデータサイズ
	desc.topology				= D3D11_PRIMITIVE_TOPOLOGY_LINELIST;	// トポロジーの設定(頂点のペアごとに独立した線)
	m_pMeshBuffer[DRAW_WIRE_FRAME]	= new MeshBuffer(desc);					// メッシュバッファの作成

}


/* ========================================
	頂点バッファ作成関数
	-------------------------------------
	内容：緯度、経度の数から球の頂点データを作成
	-------------------------------------
	引数1：latitudeCount	緯度の数
	引数2：longitudeCount	経度の数
	-------------------------------------
	戻値：vector<T_Vertex>	頂点データ
=========================================== */
std::vector<ShapeBase::T_Vertex> ShapeSphere::CreateSphereVertices(int latitudeCount, int longitudeCount)
{
	std::vector<T_Vertex> vertices; // 頂点の配列

	// 緯度方向のループ
	for (int lat = 0; lat <= latitudeCount; ++lat) {
		// 緯度に基づく角度を計算
		float theta = lat * DirectX::XM_PI / latitudeCount; // 緯度の角度
		float sinTheta = sin(theta);						// sin(θ)
		float cosTheta = cos(theta);						// cos(θ)

		// 経度方向のループ
		for (int lon = 0; lon <= longitudeCount; ++lon) {
			// 経度に基づく角度を計算
			float phi = lon * 2 * DirectX::XM_PI / longitudeCount; // 経度の角度
			float sinPhi = sin(phi); // sin(φ)
			float cosPhi = cos(phi); // cos(φ)

			// 球の頂点位置を計算
			Vector3<float> position;
			position.x = RADIUS * cosPhi * sinTheta; // x座標
			position.y = RADIUS * cosTheta;			 // y座標
			position.z = RADIUS * sinPhi * sinTheta; // z座標

			// テクスチャ座標を計算 (u, v)
			Vector2<float> texCoord;
			texCoord.x = static_cast<float>(lon) / longitudeCount; // u座標
			texCoord.y = static_cast<float>(lat) / latitudeCount; // v座標

			// 法線を計算 (頂点位置を正規化)
			Vector3<float> normal = position.GetNormalize();  // 頂点位置を正規化して法線を計算

			// 頂点を配列に追加
			vertices.push_back({ position, normal, texCoord });
		}
	}


	return vertices;
}


/* ========================================
	インデックスバッファ作成関数
	-------------------------------------
	内容：緯度、経度の数から球のインデックスデータを作成
	-------------------------------------
	引数1：latitudeCount	緯度の数
	引数2：longitudeCount	経度の数
	-------------------------------------
	戻値：vector<int>	インデックスデータ
=========================================== */
std::vector<int> ShapeSphere::CreateSphereIndices(int latitudeCount, int longitudeCount)
{
	std::vector<int> indices; // インデックスの配列

	// 緯度方向のループ
	for (int lat = 0; lat < latitudeCount; ++lat) 
	{
		// 経度方向のループ
		for (int lon = 0; lon < longitudeCount; ++lon) 
		{

			// 現在の頂点のインデックスを計算
			int first = (lat * (longitudeCount + 1)) + lon; // 現在の緯度の最初の頂点
			int second = first + longitudeCount + 1; // 次の緯度の最初の頂点

			// 三角形を形成するためのインデックスを追加（順番を逆に）
			indices.push_back(first);		// 現在の緯度の現在の経度の頂点
			indices.push_back(first + 1);	// 現在の緯度の次の経度の頂点
			indices.push_back(second);		// 次の緯度の現在の経度の頂点

			indices.push_back(second);		// 次の緯度の現在の経度の頂点
			indices.push_back(first + 1);	// 現在の緯度の次の経度の頂点
			indices.push_back(second + 1);	// 次の緯度の次の経度の頂点
		}
	}

	return indices;
}
