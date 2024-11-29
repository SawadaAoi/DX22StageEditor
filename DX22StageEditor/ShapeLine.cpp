/* ========================================
	DX22Base/
	------------------------------------
	線描画用cpp
	------------------------------------
	ShapeLine.cpp
========================================== */

// =============== インクルード ===================
#include "ShapeLine.h"
#include "ShaderManager.h"	// 事前に読み込まれたシェーダーを使用する為
#include "DirectXManager.h"	// レンダーターゲットの設定を行う為
#include "CameraManager.h"	// カメラの情報を取得する為
#include "Quaternion.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
ShapeLine::ShapeLine()
	: m_vPos(0.0f, 0.0f, 0.0f)
	, m_pMeshBuffer(nullptr)
	, m_pPS(nullptr)
	, m_pVS(nullptr)
	, m_nLineCnt(0)
	, m_nMaxLineCnt(0)
	, m_pVertex(nullptr)
	, m_WVP{}
{
}

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化(複数の線を描画する時に使用)
	-------------------------------------
	引数1：DrawNum	描画する線の数
=========================================== */
ShapeLine::ShapeLine(int DrawNum)
	: m_vPos(0.0f, 0.0f, 0.0f)
	, m_vScale(1.0f, 1.0f, 1.0f)
	, m_qRotation()
	, m_pMeshBuffer(nullptr)
	, m_pPS(nullptr)
	, m_pVS(nullptr)
	, m_nLineCnt(0)
	, m_nMaxLineCnt(DrawNum)
	, m_pVertex(nullptr)
	, m_WVP{}

{
	m_pVS = GET_VS_DATA(VS_KEY::VS_LINE);	// 頂点シェーダー読み込み
	m_pPS = GET_PS_DATA(PS_KEY::PS_LINE);	// ピクセルシェーダー読み込み


	MakeLine();
}

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化(1本の線を描画する時に使用)
	-------------------------------------
	引数1：start	始点
	引数2：end		終点
	引数3：color	色
=========================================== */
ShapeLine::ShapeLine(Vector3<float> start, Vector3<float> end, Vector3<float> color)
	: ShapeLine(1)	// 1本の線を描画するので、最大数は1
{
	AddLine(start, end, color);	// 線を追加
}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：解放処理
=========================================== */
ShapeLine::~ShapeLine()
{
	SAFE_DELETE(m_pMeshBuffer);
	SAFE_DELETE(m_pVertex);
}

/* ========================================
	コピーコンストラクタ
	-------------------------------------
	内容：コピーコンストラクタ
=========================================== */
ShapeLine::ShapeLine(const ShapeLine& other)
	: m_vPos{ 0.0f, 0.0f, 0.0f }
	, m_vScale{ 1.0f, 1.0f, 1.0f }
	, m_qRotation()
	, m_pMeshBuffer(nullptr)
	, m_pPS(nullptr)
	, m_pVS(nullptr)
	, m_nLineCnt(0)
	, m_nMaxLineCnt(0)
	, m_pVertex(nullptr)
	, m_WVP{}
{
	if (this == &other) return;

	int nVertexNum = other.m_nMaxLineCnt * 2;	// 1本2頂点

	m_pMeshBuffer	= new MeshBuffer(*other.m_pMeshBuffer);
	m_pVertex		= new T_LineVertex[nVertexNum];
	memcpy_s(m_pVertex, sizeof(T_LineVertex) * nVertexNum, other.m_pVertex, sizeof(T_LineVertex) * nVertexNum);

	m_pPS			= other.m_pPS;
	m_pVS			= other.m_pVS;
	m_nLineCnt		= other.m_nLineCnt;
	m_nMaxLineCnt	= other.m_nMaxLineCnt;
	m_vPos			= other.m_vPos;
	m_vScale		= other.m_vScale;
	m_qRotation		= other.m_qRotation;
}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
=========================================== */
void ShapeLine::Draw()
{
	DirectXManager::OnOffDepthStencilView(true);	// 深度バッファの有効化

	SetWVP();	// WVP行列の設定
	// 定数バッファ(ワールド、ビュー、プロジェクション行列)の書き込み
	m_pVS->WriteBuffer(0, m_WVP);

	// シェーダーのバインド(シェーダーの設定を GPU に送る)
	m_pVS->Bind();
	m_pPS->Bind();
	
	// 描画
	m_pMeshBuffer->Draw();

}

/* ========================================
	線追加関数
	-------------------------------------
	内容：描画する線を追加する
	-------------------------------------
	引数1：start	始点
	引数2：end		終点
	引数3：color	色
=========================================== */
void ShapeLine::AddLine(Vector3<float> start, Vector3<float> end, Vector3<float> color)
{
	// 最大数を超えたら描画しない
	if (m_nLineCnt < m_nMaxLineCnt)
	{
		T_LineVertex* pVtx = reinterpret_cast<T_LineVertex*>(m_pVertex);	// 現在の頂点データを取得
		int nLastIndex = m_nLineCnt * 2;	// 頂点データの一番後ろの位置(現在の線数 * 2(1本2頂点の為))
		
		// 頂点データを追加
		pVtx[nLastIndex + 0] = { start.x, start.y, start.z, color.x, color.y, color.z, 1.0f };	// 始点
		pVtx[nLastIndex + 1] = {   end.x,   end.y,   end.z, color.x, color.y, color.z, 1.0f };	// 終点

		m_nLineCnt++;	// 線の数を増やす

		// pVtxのポインタとm_pMeshBufferのポインタは同じアドレスを指しているので、
		// pVtxにデータを書き込むとm_pMeshBufferにもデータが書き込まれる
	}

	// 頂点データを更新
	m_pMeshBuffer->WriteVertexData(m_pVertex);	

}

/* ========================================
	線更新関数
	-------------------------------------
	内容：描画する線を更新する
	-------------------------------------
	引数1：nLineNum	更新したい線の番号(1本目は1)
	引数2：start	始点
	引数3：end		終点
	引数4：color	色
=========================================== */
void ShapeLine::UpdateLine(int nLineNum, Vector3<float> start, Vector3<float> end, Vector3<float> color)
{
	if (nLineNum <= 0) return;	// 線の番号が0以下の場合は更新しない

	if (nLineNum <= m_nMaxLineCnt)
	{
		T_LineVertex* pVtx = reinterpret_cast<T_LineVertex*>(m_pVertex);	// 現在の頂点データを取得

		// 更新したい線の頂点データの位置(更新したい線数 * 2(1本2頂点の為))
		// 頂点データは0から始まるので、-1する
		int nLastIndex = (nLineNum - 1) * 2;	

		// 頂点データを上書き
		pVtx[nLastIndex + 0] = { start.x, start.y, start.z, color.x, color.y, color.z, 1.0f };	// 始点
		pVtx[nLastIndex + 1] = { end.x, end.y, end.z, color.x, color.y, color.z, 1.0f };		// 終点

		// 頂点データを更新
		m_pMeshBuffer->WriteVertexData(m_pVertex);
	}
}



/* ========================================
	線作成関数
	-------------------------------------
	内容：線を作成する
=========================================== */
void ShapeLine::MakeLine()
{
	m_pVertex		= new T_LineVertex[m_nMaxLineCnt * 2];	// 1本2頂点
	m_nLineCnt		= 0;	// 描画数初期化

	MeshBuffer::T_MeshData desc = {};
	desc.pVtx					= m_pVertex;
	desc.vtxCount				= m_nMaxLineCnt * 2;					// あとで追加する可能性があるので最大数を設定(1本2頂点)
	desc.vtxSize				= sizeof(T_LineVertex);
	desc.topology				= D3D11_PRIMITIVE_TOPOLOGY_LINELIST;	// トポロジーの設定(ラインリスト)
	desc.bWrite					= true;									// 追加するので書き込み可能
	m_pMeshBuffer				= new MeshBuffer(desc);
}

/* ========================================
	WVP行列設定関数
	-------------------------------------
	内容：ワールド、ビュー、プロジェクション行列を設定する
=========================================== */
void ShapeLine::SetWVP()
{
	// ワールド行列の計算
	DirectX::XMStoreFloat4x4(&m_WVP[0],												// Float4x4に変換して格納
		DirectX::XMMatrixTranspose(													// 転地
			DirectX::XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z) *			// スケール
			m_qRotation.ToDirectXMatrix() *											// 回転
			DirectX::XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z)				// 移動
		));

	m_WVP[1] = CAMERA_MNG_INST.GetActiveCamera()->GetViewMatrix();
	m_WVP[2] = CAMERA_MNG_INST.GetActiveCamera()->GetProjectionMatrix();
}


/* ========================================
	セッター(座標)関数
	-------------------------------------
	引数：pos	座標
=========================================== */
void ShapeLine::SetPos(Vector3<float> pos)
{
	m_vPos = pos;
}

/* ========================================
	セッター(回転)関数
	-------------------------------------
	引数：rot	回転
=========================================== */
void ShapeLine::SetRotation(Quaternion rot)
{
	m_qRotation = rot;
}

/* ========================================
	セッター(スケール)関数
	-------------------------------------
	引数：scale	スケール
=========================================== */
void ShapeLine::SetScale(Vector3<float> scale)
{
	m_vScale = scale;
}
