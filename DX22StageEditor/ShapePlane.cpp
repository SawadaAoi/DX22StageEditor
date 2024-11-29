/* ========================================
	DX22Base/
	------------------------------------
	図形描画(板ポリゴン)用cpp
	------------------------------------
	ShapePlane.cpp
========================================== */

// =============== インクルード ===================
#include "ShapePlane.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
ShapePlane::ShapePlane()
{
	MakeMesh();
}

/* ========================================
	メッシュ作成関数
	-------------------------------------
	内容：メッシュを作成する
=========================================== */
void ShapePlane::MakeMesh()
{
	MakeMeshNormal();
	MakeMeshWire();
}

/* ========================================
	メッシュ作成(通常)関数
	-------------------------------------
	内容：メッシュを作成する(通常)
=========================================== */
void ShapePlane::MakeMeshNormal()
{
	// 面の法線(真上に向かう)
	Vector3<float> vNormal = { 0.0f, 1.0f, 0.0f };	

	// 頂点データ
	T_Vertex vtx[] = {
		{{-0.5f, 0.0f, 0.5f}, vNormal, { 0.0f, 0.0f}},	// 左上
		{{ 0.5f, 0.0f, 0.5f}, vNormal, { 1.0f, 0.0f}},	// 右上
		{{-0.5f, 0.0f,-0.5f}, vNormal, { 0.0f, 1.0f}},	// 左下
		{{ 0.5f, 0.0f,-0.5f}, vNormal, { 1.0f, 1.0f}},	// 右下
	};
	
	// インデックスデータ
	int idx[] = 
	{
		0,1,2,1,3,2,
	};

	// バッファの作成
	MeshBuffer::T_MeshData desc = {};
	desc.pVtx					= vtx;										// 頂点データ
	desc.vtxCount				= _countof(vtx);							// 頂点の数(_countofは配列の数を数える)
	desc.vtxSize				= sizeof(T_Vertex);							// 頂点一つあたりのデータサイズ
	desc.pIdx					= idx;										// インデックスデータ(頂点の順番)
	desc.idxCount				= _countof(idx);							// インデックスの数
	desc.idxSize				= sizeof(int);								// インデックス一つあたりのデータサイズ
	desc.topology				= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	// トポロジーの設定(三角形リスト)
	m_pMeshBuffer[DRAW_NORMAL]	= new MeshBuffer(desc);						// メッシュバッファの作成

}

/* ========================================
	メッシュ作成(ワイヤフレーム)関数
	-------------------------------------
	内容：メッシュを作成する(ワイヤフレーム)
=========================================== */
void ShapePlane::MakeMeshWire()
{
	
	// 面の法線(真上に向かう)
	Vector3<float> vNormal = { 0.0f, 1.0f, 0.0f };

	// 頂点データ
	T_Vertex vtx[] = {
		{{-0.5f, 0.0f, 0.5f}, vNormal, { 0.0f, 0.0f}},
		{{ 0.5f, 0.0f, 0.5f}, vNormal, { 1.0f, 0.0f}},
		{{-0.5f, 0.0f,-0.5f}, vNormal, { 0.0f, 1.0f}},
		{{ 0.5f, 0.0f,-0.5f}, vNormal, { 1.0f, 1.0f}},
	};

	// インデックスデータ
	int idx[] = {
		0,1,1,3,3,2,2,0,1,2,
	};

	// バッファの作成
	MeshBuffer::T_MeshData desc		= {};
	desc.pVtx						= vtx;									// 頂点データ
	desc.vtxCount					= _countof(vtx);						// 頂点の数(_countofは配列の数を数える)
	desc.vtxSize					= sizeof(T_Vertex);						// 頂点一つあたりのデータサイズ
	desc.pIdx						= idx;									// インデックスデータ(頂点の順番)
	desc.idxCount					= _countof(idx);						// インデックスの数
	desc.idxSize					= sizeof(int);							// インデックス一つあたりのデータサイズ
	desc.topology					= D3D11_PRIMITIVE_TOPOLOGY_LINELIST;	// トポロジーの設定(三角形リスト)
	m_pMeshBuffer[DRAW_WIRE_FRAME]	= new MeshBuffer(desc);					// メッシュバッファの作成

}
