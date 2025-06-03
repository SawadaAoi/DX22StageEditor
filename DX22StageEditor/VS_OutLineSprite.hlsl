/* ========================================
	CatRobotGame/
	------------------------------------
	2D画像枠線描画用頂点シェーダー
	------------------------------------
	説明：2D画像(ビルボード等)に枠線を描画するための頂点シェーダー
         　画像選択時に表示する
	------------------------------------
	VS_OutLineSprite.hlsl
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
cbuffer bufMat : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

// 定数バッファ(パラメータ)
cbuffer bufParam : register(b1)
{
    float2 offset; // 中心座標調整
    float2 size; // サイズ調整
    float2 uvPos; // UV座標調整
    float2 uvScale; // UVスケール
    float4 color; // 色（aを使ってもいい）
};

cbuffer bufCamera : register(b2)
{
    float4 cameraPos;
};


// アウトライン描画用メイン関数
VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    // アウトラインの太さ（固定 or color.a から取得してもよい）
    float outlineScale = 1.1f; // ← 1.05～1.2くらいで調整

    // サイズ拡大（アウトライン専用）
    float2 scaledSize = size * outlineScale;
    
    // 元の頂点位置をワールド空間に変換
    float4 worldPosOriginal = mul(float4(vin.pos, 1.0f), world);
    // カメラとの距離を計算
    float cameraDistance = length(worldPosOriginal.xyz - cameraPos.xyz);
    // スケーリング係数をカメラ距離に基づいて調整
    float scaleFactor = cameraDistance * 0.01f; // 適宜調整
    
    scaledSize *= (1.0f + scaleFactor); // サイズ調整

    // 座標処理
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos.xy *= scaledSize; // サイズ調整とスケーリング
    vout.pos.xy += offset;

    // 行列変換
    vout.pos = mul(vout.pos, world);
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);

    // UVはそのまま
    vout.uv = vin.uv;
    vout.uv *= uvScale;
    vout.uv += uvPos;
    // 色はそのまま
    vout.color = color;

    return vout;
}