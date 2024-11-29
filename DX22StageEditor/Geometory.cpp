/* ========================================
	DX22Base/
	------------------------------------
	図形表示用cpp
	------------------------------------
	Geometory.cpp
========================================== */

// =============== インクルード ===================
#include "Geometory.h"


/* ========================================
	初期化関数
	-------------------------------------
	内容：各種図形の初期化
=========================================== */
void Geometory::Init()
{
	for (int i = 0; i < 3; ++i)
		DirectX::XMStoreFloat4x4(&m_WVP[i], DirectX::XMMatrixIdentity());
	MakeBox();
	MakeCylinder();
	MakeSphere();
	MakeVS();
	MakePS();
	MakeLineShader();
	MakeLine();
}

/* ========================================
	終了関数
	-------------------------------------
	内容：各種図形の解放処理
=========================================== */
void Geometory::Uninit()
{
	SAFE_DELETE_ARRAY(m_pLineVtx);
	SAFE_DELETE(m_pLineShader[1]);
	SAFE_DELETE(m_pLineShader[0]);
	SAFE_DELETE(m_pPS);
	SAFE_DELETE(m_pVS);
	SAFE_DELETE(m_pLines);
	SAFE_DELETE(m_pSphere);
	SAFE_DELETE(m_pCylinder);
	SAFE_DELETE(m_pBox);
}

/* ========================================
	ワールド行列の設定
	-------------------------------------
	引数：world	ワールド行列
=========================================== */
void Geometory::SetWorld(DirectX::XMFLOAT4X4 world)
{
	m_WVP[0] = world;
}

/* ========================================
	ビュー行列の設定
	-------------------------------------
	引数：view	ビュー行列
=========================================== */
void Geometory::SetView(DirectX::XMFLOAT4X4 view)
{
	m_WVP[1] = view;
}

/* ========================================
	プロジェクション行列の設定
	-------------------------------------
	引数：proj	プロジェクション行列
=========================================== */
void Geometory::SetProjection(DirectX::XMFLOAT4X4 proj)
{
	m_WVP[2] = proj;
}

/* ========================================
	直線作成関数
	-------------------------------------
	内容：直線を描画する
	-------------------------------------
	引数1：start	始点
	引数2：end		終点
	引数3：color	色
=========================================== */
void Geometory::AddLine(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, DirectX::XMFLOAT4 color)
{
	// 最大数を超えたら描画しない
	if (m_lineCnt < MAX_LINE_NUM)
	{
		LineVertex* pVtx = reinterpret_cast<LineVertex*>(m_pLineVtx);
		// 頂点データの設定
		// 始点(m_lineCnt * 2 + 0)と終点(m_lineCnt * 2 + 1)を設定　頂点2つで1本の線なのでm_lineCnt * 2
		pVtx[m_lineCnt * 2 + 0] = { start.x, start.y, start.z, color.x, color.y, color.z, color.w };	
		pVtx[m_lineCnt * 2 + 1] = { end.x, end.y, end.z, color.x, color.y, color.z, color.w };
		++m_lineCnt;
	}

	m_pLines->WriteVertexData(m_pLineVtx);

}


/* ========================================
	直線描画関数
	-------------------------------------
	内容：直線を描画する
=========================================== */
void Geometory::DrawLines()
{
	m_pLineShader[0]->WriteBuffer(0, m_WVP);
	m_pLineShader[0]->Bind();
	m_pLineShader[1]->Bind();
	m_pLines->Draw(m_lineCnt * 2);
	m_lineCnt = 0;
}

/* ========================================
	立方体作成関数
	-------------------------------------
	内容：立方体を描画する
=========================================== */
void Geometory::DrawBox()
{
	if (m_pBox == nullptr)
		return;
	m_pVS->WriteBuffer(0, m_WVP);
	m_pVS->Bind();
	m_pPS->Bind();
	m_pBox->Draw();
}

/* ========================================
	円柱作成関数
	-------------------------------------
	内容：円柱を描画する
=========================================== */
void Geometory::DrawCylinder()
{
	if (m_pCylinder == nullptr)
		return;
	m_pVS->WriteBuffer(0, m_WVP);
	m_pVS->Bind();
	m_pPS->Bind();
	m_pCylinder->Draw();
}

/* ========================================
	球作成関数
	-------------------------------------
	内容：球を描画する
=========================================== */
void Geometory::DrawSphere()
{
	if (m_pSphere == nullptr)
		return;
	m_pVS->WriteBuffer(0, m_WVP);
	m_pVS->Bind();
	m_pPS->Bind();
	m_pSphere->Draw();
}


/* ========================================
	頂点シェーダー作成関数
	-------------------------------------
	内容：頂点シェーダーを作成する
=========================================== */
void Geometory::MakeVS()
{
const char* VSCode = R"EOT(
struct VS_IN {
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
};
struct VS_OUT {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};
cbuffer Matrix : register(b0) {
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
VS_OUT main(VS_IN vin) {
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.uv = vin.uv;
	return vout;
})EOT";

	m_pVS = new VertexShader();
	m_pVS->Compile(VSCode);
}

/* ========================================
	ピクセルシェーダー作成関数
	-------------------------------------
	内容：ピクセルシェーダーを作成する
=========================================== */
void Geometory::MakePS()
{
#ifdef _DEBUG
	const char* PSCode = R"EOT(
struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};
float4 main(PS_IN pin) : SV_TARGET0 {
	float4 color = float4(1,1,1,1);
	float2 halfGrid = floor(abs(pin.uv) * 2.0f);
	float2 quatGrid = floor(abs(pin.uv) * 8.0f);

	float half = fmod(halfGrid.x + halfGrid.y, 2.0f);
	float quat = fmod(quatGrid.x + quatGrid.y, 2.0f);

	color.rgb = ((half * 0.1f) * quat + 0.45f) + (1 - quat) * 0.05f;
	return color;
})EOT";
#else
	const char* PSCode = R"EOT(
struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};
float4 main(PS_IN pin) : SV_TARGET0 {
	return float4(1.0f, 0.0f, 1.0f, 1.0f);
})EOT";
#endif

	m_pPS = new PixelShader();
	m_pPS->Compile(PSCode);
}

/* ========================================
	直線用シェーダー作成関数
	-------------------------------------
	内容：直線描画用のシェーダーを作成する
=========================================== */
void Geometory::MakeLineShader()
{
	const char* VSCode = R"EOT(
struct VS_IN {
	float3 pos : POSITION0;
	float4 color : COLOR0;
};
struct VS_OUT {
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
};
cbuffer Matrix : register(b0) {
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
VS_OUT main(VS_IN vin) {
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.color = vin.color;
	return vout;
})EOT";

	const char* PSCode = R"EOT(
struct PS_IN {
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
};
float4 main(PS_IN pin) : SV_TARGET0 {
	return pin.color;
})EOT";

	m_pLineShader[0] = new VertexShader();
	m_pLineShader[0]->Compile(VSCode);
	m_pLineShader[1] = new PixelShader();
	m_pLineShader[1]->Compile(PSCode);
}


void Geometory::MakeLine()
{
	m_pLineVtx = new LineVertex[MAX_LINE_NUM * 2];	// 1本2頂点
	m_lineCnt = 0;	// 描画数初期化
	MeshBuffer::T_MeshData desc = {};
	desc.pVtx = m_pLineVtx;
	desc.vtxCount = MAX_LINE_NUM * 2;
	desc.vtxSize = sizeof(LineVertex);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	desc.bWrite = true;
	m_pLines = new MeshBuffer(desc);
}