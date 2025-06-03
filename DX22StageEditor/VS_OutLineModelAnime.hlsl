/* ========================================
	CatRobotGame/
	------------------------------------
    3Dモデル(アニメ)枠線描画用頂点シェーダー
	------------------------------------
	説明：3Dモデルに枠線を描画するための頂点シェーダー
        　アニメーション対応
	------------------------------------
    VS_OutLineModelAnime.hlsl
========================================== */

// 頂点シェーダー入力構造体
struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float4 weight : WEIGHT0;
    uint4 index : INDEX0;
};

// 頂点シェーダー出力構造体
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

// 定数バッファ(ワールド、ビュー、プロジェクション行列)
cbuffer bufMat : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

// カメラ位置
cbuffer bufCamera : register(b1)
{
    float4 cameraPos;
};

// ボーン行列
cbuffer Bone : register(b2)
{
    float4x4 bone[200];
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    // --- スキニング（ボーンアニメーション）行列の合成 ---
    float4x4 skinMat =
        bone[vin.index.x] * vin.weight.x +
        bone[vin.index.y] * vin.weight.y +
        bone[vin.index.z] * vin.weight.z +
        bone[vin.index.w] * vin.weight.w;

    // --- スキン適用位置 ---
    float4 localPos = float4(vin.pos, 1.0f);
    float4 skinnedPos = mul(localPos, skinMat);

    // --- 法線もスキニング ---
    float3 skinnedNormal = mul(vin.normal, (float3x3) skinMat);

    // --- カメラ距離に応じてアウトラインの太さを調整 ---
    float4 worldPos = mul(skinnedPos, world);
    float len = length(cameraPos.xyz - worldPos.xyz);
    float outlineThickness = len * 0.005f;

    // --- アウトライン（法線方向にオフセット） ---
    skinnedPos.xyz += normalize(skinnedNormal) * outlineThickness;

    // --- 最終座標変換 ---
    worldPos = mul(skinnedPos, world);
    float4 viewPos = mul(worldPos, view);
    float4 projPos = mul(viewPos, proj);

    // --- 出力 ---
    vout.pos = projPos;
    vout.uv = vin.uv;
    vout.normal = normalize(mul(skinnedNormal, (float3x3) world));

    return vout;
}
