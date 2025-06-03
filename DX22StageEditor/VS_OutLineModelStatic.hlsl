/* ========================================
	CatRobotGame/
	------------------------------------
    3Dモデル(静的)枠線描画用頂点シェーダー
	------------------------------------
	説明：3Dモデルに枠線を描画するための頂点シェーダー
	------------------------------------
    VS_OutLineModelStatic.hlsl
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


VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    vout.pos = float4(vin.pos, 1.0f);
    
    // カメラとの距離に応じてアウトラインの太さを変更
    float4 worldPos = mul(vout.pos, world);
    float len = length(cameraPos.xyz - worldPos.xyz);
    float bold = len * 0.01f;
    
    vout.pos += float4(vin.normal * bold, 0.0f); // 法線方向に移動
    
    vout.pos = mul(vout.pos, world); // ローカル座標からワールド座標へ変換
    vout.pos = mul(vout.pos, view); // ワールド座標からビュー座標へ変換
    vout.pos = mul(vout.pos, proj); // ビュー座標からプロジェクション座標へ変換
	
    vout.normal = mul(vin.normal, (float3x3) world); // 法線ベクトルをワールド座標に変換
    vout.uv = vin.uv; // UV座標をそのまま出力

    return vout;
}
