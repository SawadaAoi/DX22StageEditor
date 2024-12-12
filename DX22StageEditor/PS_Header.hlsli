/* ========================================
	DX22Base/
	------------------------------------
	ライト用まとめシェーダーヘッダ
	------------------------------------
	説明:ライトで使う関数、構造体を記載する
	------------------------------------
	LightHeader.hlsli
========================================== */

// =============== 構造体定義 =====================
// 頂点シェーダーから受け取るためのデータを定義
//struct PS_IN
//{
//	float4 pos		: SV_POSITION0;
//	float3 normal	: NORMAL0;
//	float2 uv		: TEXCOORD0;

//};

//// 頂点シェーダーから受け取るためのデータを定義
//struct PS_IN_W
//{
//	float4 pos		: SV_POSITION0;
//	float3 normal	: NORMAL0;
//	float4 uv		: TEXCOORD0;
//	float4 worldPos : TEXCOORD1;
//};

// =============== 変数定義 ========================
// テクスチャを受け取る
Texture2D		tex	 : register(t0);
SamplerState	samp : register(s0); // テクスチャの繰り返し設定

// ライトのデータ
struct LightData
{
    float lightType; // ライトの種類
    float3 lightPos; // ライトの座標
    float4 lightColor; // ライトの色
    float3 lightDir; // ライトの方向
    float range; // ライトの影響範囲
    float angle; // スポットライトの角度
    float3 dummy; // 16バイトに揃える為のダミー
};

/* ========================================
	拡散反射光取得関数
	-------------------------------------
	内容：法線とライトベクトルから拡散反射光を求める
	-------------------------------------
	引数1：法線ベクトル
	引数2：ライト方向ベクトル
	引数3：ライトの色
	-------------------------------------
	戻値：拡散反射光のRGB
=========================================== */
float3 GetDiffuseLight(float3 normal, float3 lightDir, float4 lightColor)
{
	//法線
	// normalize→正規化(単位ベクトル変換)
	float3 N = normalize(normal);

	// ライトの方向
	float3 L = normalize(lightDir);
	// 内積との計算結果が見た目に合うように、事前にライトの方向を反転させておく
	L *= -1.0f;

	// 内積の計算
	float d = dot(L, N);
	d = saturate(d);		// 0未満は0,1より大きいと1にする

	// 光の色を計算
	float3 diffuseLight = lightColor.rgb * d;

	return diffuseLight;
}


/* ========================================
	鏡面反射光取得関数
	-------------------------------------
	内容：法線とライトベクトルから拡散反射光を求める
	-------------------------------------
	引数1：法線ベクトル
	引数2：ワールド座標
	引数3：ライト方向ベクトル
	引数4：ライトの色
	-------------------------------------
	戻値：拡散反射光のRGB
=========================================== */
float3 GetSpecularLight(float3 normal, float4 worldPos, float3 lightDir, float4 lightColor, float3 cameraPos)
{
	// 反射ベクトルの計算
	float3 N = normalize(normal);		// 法線
	float3 R = reflect(lightDir, N);	// 反射ベクトル
	R = normalize(R);

	// 視線(カメラ)に向かうベクトルの計算
	float3 V = cameraPos - worldPos.xyz;	//カメラ座標-オブジェクト自身の座標
	V = normalize(V);

	// 鏡面反射の強さを求める
	float d = dot(R, V);	// 反射ベクトルとオブジェクト→カメラ方向ベクトルの向きが近いほど強くなる(1に近づくほど)
	// 内積の結果は負数(逆方向の場合)になるので、負数の場合は0になる
	if (d < 0.0f)
	{
		d = 0.0f;
	}

	// 鏡面反射の強さを絞る
	d = pow(d, 5.0f);	// 二乗をするのはdが1に近いときと、0に近いときの差が大きくなる。斜めの直線から垂れたロープの様なグラフになる

	// 鏡面反射の光の色を計算
	float3 specularLight = lightColor.rgb * d;

	return specularLight;
}

/* ========================================
    点光源光取得関数
    -------------------------------------
    内容：法線とライトベクトルから拡散反射光を求める
    -------------------------------------
    引数1：法線ベクトル
    引数2：ワールド座標
    引数3：ライトの座標
    引数4：ライトの影響範囲
    引数5：ライトの色
    -------------------------------------
    戻値：点光源のRGB
=========================================== */
float3 GetPointLight(float3 normal, float4 worldPos, float3 lightPos, float range, float4 lightColor,
                     float3 fCameraPos, float fDiffuseStr, float fSpecularStr)
{
	float3 pointLight = float3(0.0f, 0.0f, 0.0f);
	
	float3 N = normalize(normal);		// 法線
	
	// ライトの方向
	float3 L = worldPos.xyz - lightPos;	
	L = normalize(L);					// 正規化
	
	// ライトからの距離
	float distance = length(worldPos.xyz - lightPos);	
	
	// 距離による影響度(0～1)
    float affect = saturate(1.0f - distance / range);	
	// 二乗をするのはdが1に近いときと、0に近いときの差が大きくなる。斜めの直線から垂れたロープの様なグラフになる
    affect = pow(affect, 2.0f);
	
	// 反射光の計算
    float3 diffuse = GetDiffuseLight(normal, L, lightColor) * fDiffuseStr;
	float3 specular = GetSpecularLight(normal, worldPos, L, lightColor, fCameraPos) * fSpecularStr;
	
    pointLight.rgb = (diffuse + specular) * affect;
	
    return pointLight;
}

/* ========================================
    スポットライト光取得関数
    -------------------------------------
    内容：法線とライトベクトルから拡散反射光を求める
    -------------------------------------
    引数1：法線ベクトル
    引数2：ワールド座標
    引数3：ライトの座標
    引数4：ライトの方向
    引数5：ライトの影響範囲
    引数6：ライトの色
    引数7：スポットライトの角度
    -------------------------------------
    戻値：スポットライトのRGB	
=========================================== */
float3 GetSpotLight(float3 normal, float4 worldPos, float3 lightPos, float3 lightDir, float range, float4 lightColor, float angle,
                    float3 fCameraPos, float fDiffuseStr, float fSpecularStr)
{
    float3 spotLight = float3(0.0f, 0.0f, 0.0f);
    
    float3 N = normalize(normal);		// 法線
    
	// ライトの方向
    float3 L = worldPos.xyz - lightPos; 
    L = normalize(L);	// 正規化
    // ライトからの距離
    float distance = length(worldPos.xyz - lightPos);	
	
    // 距離による影響度(0～1)
    float affect = saturate(1.0f - distance / range);	
    // 二乗をするのはdが1に近いときと、0に近いときの差が大きくなる。斜めの直線から垂れたロープの様なグラフになる
    affect = pow(affect, 2.0f);
	
	// ライトの照射方向と法線の角度を求める
	float cosAngle = dot(lightDir, L);
	float objAngle = acos(cosAngle);
	
	// 角度が指定された角度よりも大きい場合は影響度を0にする
    float spotAffect = saturate(1.0f - (objAngle / angle));	// 角度による影響度(0～1))
    spotAffect = pow(spotAffect, 0.5f);
	
	// 反射光の計算
    float3 diffuse	= GetDiffuseLight(normal, L, lightColor) * fDiffuseStr;
    float3 specular = GetSpecularLight(normal, worldPos, L, lightColor, fCameraPos) * fSpecularStr;
    
    spotLight.rgb = (diffuse + specular) * affect * spotAffect;
    
    return spotLight;
}

/* ========================================
    ライト合計色取得関数
    -------------------------------------
    内容：ライトの合計色を取得する
    -------------------------------------
    引数1：法線ベクトル
    引数2：ワールド座標
    -------------------------------------
    戻値：ライトの合計色
=========================================== */
float4 GetTotalLightColor(float3 normal, float4 worldPos, float3 fCameraPos, 
    LightData lightDatas[10], float fDiffuseStr, float fSpecularStr, float fAmbientStr)
{
    float4 LightTotal = float4(0, 0, 0, 0);
        
    for (int i = 0; i < 10; i++)
    {
        LightData light = lightDatas[i];
            
            // ライトの種類が0の場合(存在しない)
        if (light.lightType == 0.0f)
            continue;
            
            // 方向ライト
        if (light.lightType == 1.0f)
        {
            LightTotal.rgb +=
                    (GetDiffuseLight(normal, light.lightDir, light.lightColor) * fDiffuseStr) +
                    (GetSpecularLight(normal, worldPos, light.lightDir, light.lightColor, fCameraPos) * fSpecularStr);
        }
            // 点光源
        else if (light.lightType == 2.0f)
        {
            LightTotal.rgb += 
                    GetPointLight(normal, worldPos, light.lightPos, light.range, light.lightColor, 
                                    fCameraPos, fDiffuseStr, fSpecularStr);
        }
            // スポットライト
        else if (light.lightType == 3.0f)
        {
            LightTotal.rgb += GetSpotLight(normal, worldPos,
                    light.lightPos, light.lightDir, light.range, light.lightColor, light.angle,
                    fCameraPos, fDiffuseStr, fSpecularStr);
        }

    }
    LightTotal.rgb += fAmbientStr; // 環境光を加算
	
	
	return LightTotal;
}