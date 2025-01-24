/* ========================================
	DX22Base/
	------------------------------------
	図形描画(立方体)用cpp
	------------------------------------
	ShapeBox.cpp
========================================== */

// =============== インクルード ===================
#include "ShapeBox.h"

// =============== 定数定義 =======================
const int MAX_TEXTURE_NUM = 6;	// テクスチャ最大数
const int MAX_UV_NUM = 3;		// uv最大数(Z,X,Y)

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
ShapeBox::ShapeBox()
	: ShapeBase()
{
	MakeMesh();	// メッシュの作成

	m_pTextures.resize(MAX_TEXTURE_NUM);	// テクスチャの数を設定
	m_fUvOffset.resize(MAX_UV_NUM);	// uvオフセットの数を設定
	m_fUvScale.resize(MAX_UV_NUM);	// uvスケールの数を設定
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
ShapeBox::ShapeBox(Vector3<float> fPos, Vector3<float> fSize, Vector3<float> fColor, E_DrawMode eMode)
	: ShapeBox()
{
	SetPosition(fPos);
	SetScale(fSize);
	SetColor(fColor);
	SetDrawMode(eMode);
}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画を行う
=========================================== */
void ShapeBox::Draw()
{
	DirectXManager::OnOffDepthStencilView(true);	// 深度バッファの有効化

	SetWVPMatrix();	// WVP行列の設定

	// 定数バッファ(ワールド、ビュー、プロジェクション行列)の書き込み
	m_pVS->WriteBuffer(0, m_WVP);



	// 定数バッファ(色、表示モード、テクスチャ使用フラグ)の書き込み
	float	fData[3] = { m_fColor.x, m_fColor.y, m_fColor.z };	// 色
	int		nData[2] = { m_eDrawMode, m_bIsTex };				// 表示モード、テクスチャ使用フラグ
	m_pPS->WriteBuffer(0, fData);
	m_pPS->WriteBuffer(1, nData);

	// シェーダーのバインド(シェーダーの設定を GPU に送る)
	m_pVS->Bind();
	m_pPS->Bind();

	if (m_bIsCulling)
		DirectXManager::SetCullingMode(DirectXManager::CullMode::CULL_BACK);	// カリング有効
	else
		DirectXManager::SetCullingMode(DirectXManager::CullMode::CULL_NONE);	// カリング無効

	// テクスチャ(インデックスを区切って描画する)
	for (int i = 0; i < m_pTextures.size(); ++i)
	{
		int nIdx = i / 2;	// 0,1,2,3,4,5 -> 0,0,1,1,2,2
		// Uvスケール、オフセットの設定
		DirectX::XMFLOAT4 fUvBuf = { m_fUvScale[nIdx].x, m_fUvScale[nIdx].y, m_fUvOffset[nIdx].x, m_fUvOffset[nIdx].y };
		m_pVS->WriteBuffer(1, &fUvBuf);

		m_pPS->SetTexture(0, m_pTextures[i]);		// テクスチャの設定
		m_pMeshBuffer[m_eDrawMode]->Draw(6, i * 6);	// 頂点バッファ、インデックスバッファをGPUに設定し、描画

	}

	// カリングを元に戻す
	DirectXManager::SetCullingMode(DirectXManager::CullMode::CULL_BACK);	// カリング有効
}

/* ========================================
	メッシュ作成関数
	-------------------------------------
	内容：メッシュを作成する
=========================================== */
void ShapeBox::MakeMesh()
{
	MakeMeshNormal();
	MakeMeshWire();
}


/* ========================================
	メッシュ作成(通常)関数
	-------------------------------------
	内容：メッシュを作成する(通常)
=========================================== */
void ShapeBox::MakeMeshNormal()
{
	// 各面の法線
	Vector3<float> fNormal[6] = {
		{ 0.0f, 0.0f,-1.0f},	// -Z面
		{ 0.0f, 0.0f, 1.0f},	// +Z面
		{-1.0f, 0.0f, 0.0f},	// -X面
		{ 1.0f, 0.0f, 0.0f},	// +X面
		{ 0.0f,-1.0f, 0.0f},	// -Y面
		{ 0.0f, 1.0f, 0.0f},	// +Y面
	};


	// 各頂点の座標・法線・uvデータの作成
	T_Vertex vtx[] = {

		// 頂点の順番はZ字になっている
		// 手前、奥 ----------------------------------
		// -Z面
		{{-0.5f, 0.5f,-0.5f}, {fNormal[0]}, { 0.0f, 0.0f}},	// 左上
		{{ 0.5f, 0.5f,-0.5f}, {fNormal[0]}, { 1.0f, 0.0f}},	// 右上
		{{-0.5f,-0.5f,-0.5f}, {fNormal[0]}, { 0.0f, 1.0f}},	// 左下	
		{{ 0.5f,-0.5f,-0.5f}, {fNormal[0]}, { 1.0f, 1.0f}},	// 右下

		// +Z面				 
		{{  0.5f, 0.5f,0.5f}, {fNormal[1]}, { 0.0f, 0.0f}},
		{{ -0.5f, 0.5f,0.5f}, {fNormal[1]}, { 1.0f, 0.0f}},
		{{  0.5f,-0.5f,0.5f}, {fNormal[1]}, { 0.0f, 1.0f}},
		{{ -0.5f,-0.5f,0.5f}, {fNormal[1]}, { 1.0f, 1.0f}},

		// 左右 ----------------------------------
		// -X面
		{{-0.5f, 0.5f, 0.5f}, {fNormal[2]}, { 0.0f, 0.0f}},
		{{-0.5f, 0.5f,-0.5f}, {fNormal[2]}, { 1.0f, 0.0f}},
		{{-0.5f,-0.5f, 0.5f}, {fNormal[2]}, { 0.0f, 1.0f}},
		{{-0.5f,-0.5f,-0.5f}, {fNormal[2]}, { 1.0f, 1.0f}},

		// +X面				 		  
		{{ 0.5f, 0.5f,-0.5f}, {fNormal[3]}, { 0.0f, 0.0f}},
		{{ 0.5f, 0.5f, 0.5f}, {fNormal[3]}, { 1.0f, 0.0f}},
		{{ 0.5f,-0.5f,-0.5f}, {fNormal[3]}, { 0.0f, 1.0f}},
		{{ 0.5f,-0.5f, 0.5f}, {fNormal[3]}, { 1.0f, 1.0f}},

		// 上下 ----------------------------------
		// -Y面
		{{-0.5f,-0.5f,-0.5f}, {fNormal[4]}, { 0.0f, 0.0f}},
		{{ 0.5f,-0.5f,-0.5f}, {fNormal[4]}, { 1.0f, 0.0f}},
		{{-0.5f,-0.5f, 0.5f}, {fNormal[4]}, { 0.0f, 1.0f}},
		{{ 0.5f,-0.5f, 0.5f}, {fNormal[4]}, { 1.0f, 1.0f}},

		// +Y面				 		  
		{{-0.5f, 0.5f, 0.5f}, {fNormal[5]}, { 0.0f, 0.0f}},
		{{ 0.5f, 0.5f, 0.5f}, {fNormal[5]}, { 1.0f, 0.0f}},
		{{-0.5f, 0.5f,-0.5f}, {fNormal[5]}, { 0.0f, 1.0f}},
		{{ 0.5f, 0.5f,-0.5f}, {fNormal[5]}, { 1.0f, 1.0f}},
	};
	int idx[] = {
		0,1,2,1,3,2,
		4,5,6,5,7,6,
		8,9,10,9,11,10,
		12,13,14,13,15,14,
		16,17,18,17,19,18,
		20,21,22,21,23,22,

	};

	// バッファの作成
	MeshBuffer::T_MeshData desc = {};
	desc.pVtx					= vtx;					// 頂点データ
	desc.vtxCount				= _countof(vtx);		// 頂点の数(_countofは配列の数を数える)
	desc.vtxSize				= sizeof(T_Vertex);		// 頂点一つあたりのデータサイズ
	desc.pIdx					= idx;					// インデックスデータ(頂点の順番)
	desc.idxCount				= _countof(idx);		// インデックスの数
	desc.idxSize				= sizeof(int);			// インデックス一つあたりのデータサイズ
	desc.topology				= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	// トポロジーの設定(三角形リスト)
	m_pMeshBuffer[DRAW_NORMAL]	= new MeshBuffer(desc);	// メッシュバッファの作成
}

/* ========================================
	メッシュ作成(ワイヤフレーム)関数
	-------------------------------------
	内容：メッシュを作成する(ワイヤフレーム)
=========================================== */
void ShapeBox::MakeMeshWire()
{
	// 各面の法線
	Vector3<float> fNormal = { 0.0f, 0.0f, 0.0f };	// 法線は使わないので0でいい

	// 各頂点の座標・uvデータの作成
	T_Vertex vtx[] = {

		// 頂点の順番はZ字になっている
		// 上下 ----------------------------------
		// +Y面				 		  
		{{-0.5f, 0.5f, 0.5f}, fNormal, { 0.0f, 0.0f}},	// 左上 0
		{{ 0.5f, 0.5f, 0.5f}, fNormal, { 1.0f, 0.0f}},	// 右上 1
		{{-0.5f, 0.5f,-0.5f}, fNormal, { 0.0f, 1.0f}},	// 左下 2
		{{ 0.5f, 0.5f,-0.5f}, fNormal, { 1.0f, 1.0f}},	// 右下 3

		// -Y面
		{{-0.5f,-0.5f,-0.5f}, fNormal, { 0.0f, 0.0f}},	// 左上 4
		{{ 0.5f,-0.5f,-0.5f}, fNormal, { 1.0f, 0.0f}},	// 右上 5
		{{-0.5f,-0.5f, 0.5f}, fNormal, { 0.0f, 1.0f}},	// 左下 6
		{{ 0.5f,-0.5f, 0.5f}, fNormal, { 1.0f, 1.0f}},	// 右下 7
	};


	int idx[] = {
		0,1, 1,3, 3,2, 2,0,		// 上面
		4,5, 5,7, 7,6, 6,4,		// 下面
		0,6, 1,7, 2,4, 3,5,		// 上面と下面を結ぶ線
	};

	// バッファの作成
	MeshBuffer::T_MeshData desc = {};
	desc.pVtx					= vtx;					// 頂点データ
	desc.vtxCount				= _countof(vtx);		// 頂点の数(_countofは配列の数を数える)
	desc.vtxSize				= sizeof(T_Vertex);		// 頂点一つあたりのデータサイズ
	desc.pIdx					= idx;					// インデックスデータ(頂点の順番)
	desc.idxCount				= _countof(idx);		// インデックスの数
	desc.idxSize				= sizeof(int);			// インデックス一つあたりのデータサイズ
	desc.topology				= D3D11_PRIMITIVE_TOPOLOGY_LINELIST;	// トポロジーの設定(三角形リスト)
	m_pMeshBuffer[DRAW_WIRE_FRAME]	= new MeshBuffer(desc);	// メッシュバッファの作成
}
