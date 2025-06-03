/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト枠線描画用ピクセルシェーダー
	------------------------------------
	説明：オブジェクトに枠線を描画するためのピクセルシェーダー
         　オブジェクト選択時に表示する
	------------------------------------
	PS_OutLine.hlsl
========================================== */


struct PS_IN
{
    float4 pos		: SV_POSITION0;
    float2 uv		: TEXCOORD0;
    float3 normal	: NORMAL0;
};


Texture2D tex : register(t0);
SamplerState samp : register(s0); // テクスチャの繰り返し設定

float4 main(PS_IN pin) : SV_TARGET
{
	// 不透明な部分を赤色で描画
    float4 color = tex.Sample(samp, pin.uv);
    if (color.a > 0.0f)
    {
        
        color.r = 1.0f; // 赤色に設定
        color.g = 0.0f; // 緑色は0
        color.b = 0.0f; // 青色は0
    }
    
    // 透明な部分はそのまま返す
    return color;
}