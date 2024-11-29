/* ========================================
	DX22Base/
	------------------------------------
	UI画像用ピクセルシェーダー
	------------------------------------
	説明：UIの画像を描画するためのピクセルシェーダー
	------------------------------------
	PS_Sprite.hlsl
========================================== */

// ピクセル入力構造体
struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

Texture2D		tex  : register(t0);	// テクスチャ
SamplerState	samp : register(s0);	// サンプラーステート

// メイン関数
float4 main(PS_IN pin) : SV_TARGET
{
    return tex.Sample(samp, pin.uv) * pin.color;
}