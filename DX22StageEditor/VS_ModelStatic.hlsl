/* ========================================
	DX22Base/
	------------------------------------
	静的モデル用頂点シェーダー
	------------------------------------
	説明：静的モデルに使用する頂点シェーダー
	------------------------------------
	VS_ModelStatic.hlsl
========================================== */

// 頂点シェーダー入力構造体
struct VS_IN
{
    float3 pos      : POSITION;
    float3 normal   : NORMAL0;
    float2 uv       : TEXCOORD0;
    float4 color    : COLOR0;
};

// 頂点シェーダー出力構造体
struct VS_OUT
{
    float4 pos      : SV_POSITION;
    float3 normal   : NORMAL0;
    float2 uv       : TEXCOORD0;
    float4 color    : COLOR0;
    float4 pos_w    : POSITION0;
};

// 定数バッファ(ワールド、ビュー、プロジェクション行列)
cbuffer WVP : register(b0)
{ 
    float4x4 world; 
    float4x4 view; 
    float4x4 proj; 
};

// メイン関数
VS_OUT main(VS_IN vin)
{    
    VS_OUT vout;
    vout.pos    = float4(vin.pos, 1.0f);
    vout.pos    = mul(vout.pos, world);    // ローカル座標からワールド座標へ変換
    vout.pos_w  = vout.pos;               // ワールド座標を出力
    vout.pos    = mul(vout.pos, view);     // ワールド座標からビュー座標へ変換
    vout.pos    = mul(vout.pos, proj);     // ビュー座標からプロジェクション座標へ変換
	
    vout.normal = mul(vin.normal, (float3x3) world); // 法線ベクトルをワールド座標に変換
    vout.uv     = vin.uv; // UV座標をそのまま出力
    vout.color  = vin.color;

    return vout;
}