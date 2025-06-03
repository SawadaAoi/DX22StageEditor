/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト枠線描画用頂点シェーダー
	------------------------------------
	説明：オブジェクトに枠線を描画するための頂点シェーダー
        　オブジェクト選択時に表示する
	------------------------------------
	VS_OutLine.hlsl
========================================== */

// 頂点シェーダー入力構造体
struct VS_IN
{
    float3 pos : POSITION0;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

// 頂点シェーダー出力構造体
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

// 定数バッファ
cbuffer bufMat : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

cbuffer bufCamera : register(b1)
{
    float4 cameraPos;
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    // ワールド行列のスケール成分抽出
    float3 scaleX = float3(world._11, world._12, world._13);
    float3 scaleY = float3(world._21, world._22, world._23);
    float3 scaleZ = float3(world._31, world._32, world._33);
    float modelScale = max(length(scaleX), max(length(scaleY), length(scaleZ)));

    // 法線をワールド空間へ変換
    float3 worldNormal = normalize(mul(vin.normal, (float3x3) world));

    // 元の頂点位置をワールド空間に変換
    float4 worldPosOriginal = mul(float4(vin.pos, 1.0f), world);

    // カメラとの距離を計算
    float cameraDistance = length(worldPosOriginal.xyz - cameraPos.xyz);

    // ==== スケーリング係数 ====

    // ベースの太さ（モデルサイズに依存しない）
    float baseOutline = 0.3f;

    // カメラ距離に応じた倍率（距離に比例して太くする）
    float cameraScale = cameraDistance * 0.03f; // 0.01f は調整用スケーリング係数

    // 総合スケーリング率
    float totalScale = 1.0f + ((baseOutline * cameraScale) / modelScale);

    // スケーリング適用（モデルの中心から外方向に拡大）
    float3 scaledPos = vin.pos * totalScale;
    float4 localPos = float4(scaledPos, 1.0f);

    // 変換処理
    float4 worldPos = mul(localPos, world);
    float4 viewPos = mul(worldPos, view);
    float4 projPos = mul(viewPos, proj);

    // 出力
    vout.pos = projPos;
    vout.uv = vin.uv;
    vout.normal = worldNormal;

    return vout;

}