/* ========================================
	DX22Base/
	------------------------------------
	UI画像用頂点シェーダー
	------------------------------------
	説明：UIの画像を描画するための頂点シェーダー
	------------------------------------
	VS_Sprite.hlsl
========================================== */

// 頂点入力構造体
struct VS_IN
{
    float3 pos : POSITION0;
    float2 uv : TEXCOORD0;
};
// 頂点出力構造体
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

// 定数バッファ(座標用行列)
cbuffer bufMat : register(b0) {
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

// 定数バッファ(パラメータ)
cbuffer bufParam : register(b1)
{
    float2 offset;  // 中心座標調整
    float2 size;    // サイズ調整
    float2 uvPos;   // UV座標調整
    float2 uvScale; // UVスケール
    float4 color;   // 色
};

// メイン関数
VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    vout.pos        = float4(vin.pos, 1.0f); 
    vout.pos.xy    *= size;
    vout.pos.xy    += offset;
    
    vout.pos        = mul(vout.pos, world);
    vout.pos        = mul(vout.pos, view);
    vout.pos        = mul(vout.pos, proj);
    
    vout.uv         = vin.uv;
    vout.uv        *= uvScale;
    vout.uv        += uvPos;
    
    vout.color      = color;
    return vout;
}