/* ========================================
	DX22Base/
	------------------------------------
	線描画用頂点シェーダー
	------------------------------------
	説明：線(グリッド線や、レイ等)を描画するためのピクセルシェーダー
	------------------------------------
	VS_Line.hlsl
========================================== */

// 頂点シェーダー入力構造体
struct VS_IN
{
    float3 pos : POSITION0;
    float4 color : COLOR0;
};
// 頂点シェーダー出力構造体
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

// 定数バッファ(ワールド、ビュー、プロジェクション行列)
cbuffer bufMat : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

// メイン関数
VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, world);
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    vout.color = vin.color;
    return vout;
}