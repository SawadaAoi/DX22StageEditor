/* ========================================
	DX22Base/
	------------------------------------
	図形描画用ピクセルシェーダー
	------------------------------------
	説明：テスト用の図形描画用ピクセルシェーダー
	------------------------------------
	PS_Shape.hlsl
========================================== */

// =============== インクルード ===================
#include "PS_Header.hlsli"

// 頂点シェーダーから受け取る入力構造体
struct PS_IN
{
    float4 pos      : SV_POSITION;
    float3 normal   : NORMAL0; 
    float2 uv       : TEXCOORD0;
    float4 pos_w    : POSITION0;
};

// 定数バッファ(色)
cbuffer bufColor : register(b0)
{
    float3 BaseColor;   // ベースの色
};

// 定数バッファ(フラグ)
cbuffer bufFlg : register(b1)
{
    int nDrawMode;  // 描画モード(0:通常描画, 1:ワイヤーフレーム描画)
    int bUseTex;    // テクスチャ使用(0:使用しない, 1:使用する)
};

// ライトの反射設定
cbuffer BufLightStrength : register(b2)
{
    float fDiffuseStr;
    float fSpecularStr;
    float fAmbientStr;
    int bUseLight; // ライトを参照するかどうか
};

// カメラの位置
cbuffer BufCamera : register(b3)
{
    float3 fCameraPos;
}


// シーン上のライト情報
cbuffer BufLight : register(b4)
{
    LightData lightDatas[10];
}

// ピクセルシェーダーのメイン関数
float4 main(PS_IN pin) : SV_TARGET0
{
    float4 fColor    = float4(0, 0, 0, 1);  // 初期値は白
    
    // テクスチャ使用の場合
    if (bUseTex == 1)
    {
         // ライトを参照するかどうか
        if (bUseLight == 0)
        {
            fColor = tex.Sample(samp, pin.uv);
        }
        else
        {
            float4 finalColor = tex.Sample(samp, pin.uv); // テクスチャの色
            finalColor.rgb *=
                GetTotalLightColor(pin.normal, pin.pos_w,
                    fCameraPos, lightDatas, fDiffuseStr, fSpecularStr, fAmbientStr).rgb; // シーン上のライトの色を反映
        
            fColor = finalColor;
        }

    }
    // テクスチャ未使用の場合
    else
    {
        // 通常描画の場合
        if (nDrawMode == 0)
        {
            float2 fHalfGrid = floor(abs(pin.uv) * 2.0f); // 0.0～1.0の範囲を2x2のグリッドに分割(右上、左上、右下、左下の4分割にグループ分け)
            float2 fQuatGrid = floor(abs(pin.uv) * 8.0f); // 0.0～1.0の範囲を8x8のグリッドに分割(↑の64分割バージョン)

        // 斜めの位置でグループ分け(例：四分割の場合、（左上、右下）と（右上、左下）で1と0をセットする)
            float fHalf = fmod(fHalfGrid.x + fHalfGrid.y, 2.0f);
            float fQuat = fmod(fQuatGrid.x + fQuatGrid.y, 2.0f);

            fColor.rgb = ((fHalf * 0.1f) * fQuat + 0.45f) * BaseColor.rgb + (1 - fQuat) * 0.05f;
        }
         // ワイヤーフレーム描画の場合
        else if (nDrawMode == 1)
        {
            fColor.rgb = fColor.rgb + BaseColor.rgb; // ベースカラーで描画
        }
    }
    
    return fColor;
}