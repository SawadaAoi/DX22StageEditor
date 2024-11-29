/* ========================================
	DX22Base/
	------------------------------------
	静的モデル用ピクセルシェーダー
	------------------------------------
	説明：静的モデルに使用するピクセルシェーダー
	------------------------------------
	PS_ModelStatic.hlsl
========================================== */

// 頂点シェーダーから受け取る入力構造体
struct PS_IN
{
    float4 pos		: SV_POSITION;
    float3 normal	: NORMAL0;
    float2 uv		: TEXCOORD0;
};


Texture2D tex : register(t0);		// テクスチャ
SamplerState samp : register(s0);	// サンプラーステート


// ピクセルシェーダーのメイン関数
float4 main(PS_IN pin) : SV_TARGET
{
    return tex.Sample(samp, pin.uv);
}