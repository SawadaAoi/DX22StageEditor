/* ========================================
	DX22Base/
	------------------------------------
	図形描画用ピクセル頂点シェーダー
	------------------------------------
	説明：テスト用の図形描画用頂点シェーダー
	------------------------------------
	VS_Shape.hlsl
========================================== */

// 頂点シェーダー入力構造体
struct VS_IN
{
    float3 pos      : POSITION0;
    float3 normal   : NORMAL0; // データの用途を定義する
    float2 uv       : TEXCOORD0;
};

// 頂点シェーダー出力構造体
struct VS_OUT
{
    float4 pos      : SV_POSITION;
    float3 normal   : NORMAL0; // データの用途を定義する
    float2 uv       : TEXCOORD0;
    float4 pos_w    : POSITION0;
};

// 定数バッファ
// 描画するモデルのワールド変換行列、ビュー変換行列、プロジェクション変換行列
cbuffer bufMat : register(b0)
{
    float4x4 world; // ワールド変換行列
    float4x4 view;  // ビュー変換行列
    float4x4 proj;  // プロジェクション変換行列
};

// メイン関数
VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    vout.pos    = float4(vin.pos, 1.0f);
    vout.pos    = mul(vout.pos, world);
    vout.pos_w  = vout.pos;                 // ワールド座標を出力
    vout.pos    = mul(vout.pos, view);
    vout.pos    = mul(vout.pos, proj);
    
    vout.normal = mul(vin.normal, (float3x3)world);
    vout.uv     = vin.uv;
    
    return vout;
}