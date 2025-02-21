/* ========================================
	DX22Base/
	------------------------------------
	静的モデル用ピクセルシェーダー
	------------------------------------
	説明：静的モデルに使用するピクセルシェーダー
	------------------------------------
	PS_ModelStatic.hlsl
========================================== */

// =============== インクルード ===================
#include "PS_Header.hlsli"

// 頂点シェーダーから受け取る入力構造体
struct PS_IN
{
    float4 pos		: SV_POSITION;
    float3 normal	: NORMAL0;
    float2 uv       : TEXCOORD0;
    float4 color	: COLOR0;
    float4 pos_w    : POSITION0; // ワールド座標
};

// ライトの反射設定
cbuffer BufLightStrength : register(b0)
{
    float fDiffuseStr;
    float fSpecularStr;
    float fAmbientStr;
    int bUseLight; // ライトを参照するかどうか
};

// カメラの位置
cbuffer BufCamera : register(b1)
{
    float3 fCameraPos;
}


// シーン上のライト情報
cbuffer BufLight : register(b2)
{
    LightData lightDatas[MAX_LIGHT];
}

// ピクセルシェーダーのメイン関数
float4 main(PS_IN pin) : SV_TARGET
{
    // ライトを参照するかどうか
    if (bUseLight == 0)
    {
        return tex.Sample(samp, pin.uv);
    }
    else
    {
        float4 finalColor = tex.Sample(samp, pin.uv); // テクスチャの色
        finalColor.rgb *= 
                GetTotalLightColor(pin.normal, pin.pos_w, 
                    fCameraPos, lightDatas, fDiffuseStr, fSpecularStr, fAmbientStr).rgb; // シーン上のライトの色を反映
        
        return finalColor;
    }
}