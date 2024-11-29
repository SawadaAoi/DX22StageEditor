/* ========================================
	DX22Base/
	------------------------------------
	アニメーションモデル用頂点シェーダー
	------------------------------------
	説明：アニメーションモデルに使用する頂点シェーダー
	------------------------------------
	VS_ModelAnime.hlsl
========================================== */

// 頂点シェーダー入力構造体
struct VS_IN
{
    float3 pos      : POSITION;
    float3 normal   : NORMAL0;
    float2 uv       : TEXCOORD0;
    float4 color    : COLOR0;
    float4 weight   : WEIGHT0;
    uint4  index    : INDEX0;
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

// 定数バッファ(ボーン行列)
cbuffer Bone : register(b1)
{
    float4x4 bone[200];
};

// メイン関数
VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    float4x4 anime;
    // ボーン行列の計算
    anime  = bone[vin.index.x] * vin.weight.x;
    anime += bone[vin.index.y] * vin.weight.y;
    anime += bone[vin.index.z] * vin.weight.z;
    anime += bone[vin.index.w] * vin.weight.w;
    
    vout.pos    = float4(vin.pos, 1.0f);
    vout.pos    = mul(vout.pos, anime);
    vout.pos    = mul(vout.pos, world);
    vout.pos_w  = vout.pos;             // ワールド座標を出力
    vout.pos    = mul(vout.pos, view);
    vout.pos    = mul(vout.pos, proj);
    
    vout.normal = vin.normal;
    vout.normal = mul(vout.normal, (float3x3) anime);
    vout.normal = mul(vout.normal, (float3x3) world);
    
    vout.uv     = vin.uv;
    vout.color  = vin.color;
    return vout;
};