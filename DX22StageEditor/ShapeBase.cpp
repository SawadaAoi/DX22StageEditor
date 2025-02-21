/* ========================================
	DX22Base/
	------------------------------------
	図形描画用cpp
	------------------------------------
	Shape.cpp
========================================== */

// =============== インクルード ===================
#include "ShapeBase.h"
#include "ShaderManager.h"
#include "CameraManager.h"
#include "DirectXManager.h"
#include "TextureManager.h"	
#include "LightManager.h"
#include "ShadowManager.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：無し
=========================================== */
ShapeBase::ShapeBase()
	: m_pMeshBuffer{ nullptr, nullptr }
	, m_pVS(nullptr)
	, m_pPS(nullptr)
	, m_WVP()
	, m_vPosition(0.0f, 0.0f, 0.0f)
	, m_vScale(1.0f, 1.0f, 1.0f)
	, m_qRotation({0.0f,0.0f,0.0f}, 1.0f)
	, m_fColor(1.0f, 1.0f, 1.0f)
	, m_eDrawMode(E_DrawMode::DRAW_NORMAL)
	, m_pTextures()
	, m_bIsTex(false)
	, m_bIsCulling(true)
	, m_fUvScale()
	, m_fUvOffset()
{
	// 頂点シェーダー読み込み
	m_pVS = GET_VS_DATA(VS_KEY::VS_SHAPE);
	// ピクセルシェーダー読み込み
	m_pPS = GET_PS_DATA(PS_KEY::PS_SHAPE);

	m_pTextures.resize(1);
	m_fUvScale.resize(1);
	m_fUvOffset.resize(1);
}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：解放処理
=========================================== */
ShapeBase::~ShapeBase()
{
	SAFE_DELETE(m_pMeshBuffer[E_DrawMode::DRAW_NORMAL]);
	SAFE_DELETE(m_pMeshBuffer[E_DrawMode::DRAW_WIRE_FRAME]);
}


/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
=========================================== */
void ShapeBase::Draw()
{
	DirectXManager::OnOffDepthStencilView(true);	// 深度バッファの有効化

	SetWVPMatrix();	// WVP行列の設定

	// 定数バッファ(ワールド、ビュー、プロジェクション行列)の書き込み
	m_pVS->WriteBuffer(0, m_WVP);

	// Uvスケール、オフセットの設定
	DirectX::XMFLOAT4 fUvBuf = { m_fUvScale[0].x, m_fUvScale[0].y, m_fUvOffset[0].x, m_fUvOffset[0].y};
	m_pVS->WriteBuffer(1, &fUvBuf);


	// 定数バッファ(色、表示モード、テクスチャ使用フラグ)の書き込み
	float	fData[3] = { m_fColor.x, m_fColor.y, m_fColor.z };	// 色
	int		nData[2] = { m_eDrawMode, m_bIsTex };				// 表示モード、テクスチャ使用フラグ
	m_pPS->WriteBuffer(0, fData);
	m_pPS->WriteBuffer(1, nData);
	m_pPS->SetTexture(0, m_pTextures[0]);	// テクスチャの設定

	// シェーダーのバインド(シェーダーの設定を GPU に送る)
	m_pVS->Bind();
	m_pPS->Bind();
	
	if (m_bIsCulling)
		DirectXManager::SetCullingMode(DirectXManager::CullMode::CULL_BACK);	// カリング有効
	else
		DirectXManager::SetCullingMode(DirectXManager::CullMode::CULL_NONE);	// カリング無効

	// 頂点バッファ、インデックスバッファをGPUに設定し、描画
	m_pMeshBuffer[m_eDrawMode]->Draw();	
	// カリングを元に戻す
	DirectXManager::SetCullingMode(DirectXManager::CullMode::CULL_BACK);	// カリング有効
}


/* ========================================
	WVP行列設定関数
	-------------------------------------
	内容：ワールド、ビュー、プロジェクション行列を設定する
=========================================== */
void ShapeBase::SetWVPMatrix()
{
	// ワールド行列の計算
	DirectX::XMStoreFloat4x4(&m_WVP[0],												// Float4x4に変換して格納
		DirectX::XMMatrixTranspose(													// 転地
			DirectX::XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z) *			// 拡大縮小
			m_qRotation.ToDirectXMatrix() *											// クォータニオン回転
			DirectX::XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z)				// 移動
		));

	m_WVP[1] = CAMERA_MNG_INST.GetActiveCamera()->GetViewMatrix();
	m_WVP[2] = CAMERA_MNG_INST.GetActiveCamera()->GetProjectionMatrix();

}


/* ========================================
	ゲッター(座標)関数
	-------------------------------------
	戻値：Vector3<float> fPos
=========================================== */
Vector3<float> ShapeBase::GetPosition()
{
	return m_vPosition;
}

/* ========================================
	ゲッター(サイズ)関数
	-------------------------------------
	戻値：Vector3<float> fSize
=========================================== */
Vector3<float> ShapeBase::GetScale()
{
	return m_vScale;
}

/* ========================================
	ゲッター(回転)関数
	-------------------------------------
	戻値：Quaternion m_fRotation
=========================================== */
Quaternion ShapeBase::GetRotation()
{
	return m_qRotation;
}

/* ========================================
	ゲッター(表示モード)関数
	-------------------------------------
	戻値：E_DrawMode eMode
=========================================== */
Vector3<float> ShapeBase::GetColor()
{
	return m_fColor;
}

/* ========================================
	ゲッター(表示モード)関数
	-------------------------------------
	戻値：E_DrawMode eMode
=========================================== */
ShapeBase::E_DrawMode ShapeBase::GetDrawMode()
{
	return m_eDrawMode;
}

/* ========================================
	ゲッター(テクスチャ使用フラグ)関数
	-------------------------------------
	戻値：bool m_bIsTex
=========================================== */
bool ShapeBase::GetIsTexture()
{
	return m_bIsTex;
}

/* ========================================
	ゲッター(カリング設定)関数
	-------------------------------------
	戻値：bool m_bIsCulling
=========================================== */
bool ShapeBase::GetIsCulling()
{
	return m_bIsCulling;
}

/* ========================================
	ゲッター(Uvスケール)関数
	-------------------------------------
	引数：int 配列番号
	-------------------------------------
	戻値：Vector2<float> m_vUvScale
=========================================== */
Vector2<float> ShapeBase::GetUvScale(int nIndex)
{
	// インデックスがテクスチャ配列のサイズを超えていたら0にセット
	if (nIndex >= m_pTextures.size() || nIndex < 0)
	{
		nIndex = 0;
	}

	return m_fUvScale.at(nIndex);
}

/* ========================================
	ゲッター(Uvスケール)関数
	-------------------------------------
	引数：int 配列番号
	-------------------------------------
	戻値：Vector2<float> m_vUvScale
=========================================== */
Vector2<float> ShapeBase::GetUvOffset(int nIndex)
{
	// インデックスがテクスチャ配列のサイズを超えていたら0にセット
	if (nIndex >= m_pTextures.size() || nIndex < 0)
	{
		nIndex = 0;
	}

	return m_fUvOffset.at(nIndex);
}


/* ========================================
	セッター(座標)関数
	-------------------------------------
	引数：Vector3<float> fPos
=========================================== */
void ShapeBase::SetPosition(Vector3<float> fPos)
{
	m_vPosition = fPos;
}

/* ========================================
	セッター(サイズ)関数
	-------------------------------------
	引数：Vector3<float> fSize
=========================================== */
void ShapeBase::SetScale(Vector3<float> fSize)
{
	m_vScale = fSize;
}

/* ========================================
	セッター(回転)関数
	-------------------------------------
	引数：Quaternion fRot
=========================================== */
void ShapeBase::SetRotation(Quaternion fRot)
{
	m_qRotation = fRot;
}

/* ========================================
	セッター(色)関数
	-------------------------------------
	引数：Vector3<float> fColor
=========================================== */
void ShapeBase::SetColor(Vector3<float> fColor)
{
	m_fColor = fColor;
}

/* ========================================
	セッター(表示モード)関数
	-------------------------------------
	引数：E_DrawMode eMode
=========================================== */
void ShapeBase::SetDrawMode(E_DrawMode eMode)
{
	m_eDrawMode = eMode;
}

/* ========================================
	セッター(テクスチャ)関数
	-------------------------------------
	引数：Texture* pTexture
	引数：int nIndex
=========================================== */
void ShapeBase::SetTexture(Texture* pTexture, int nIndex)
{
	// インデックスがテクスチャ配列のサイズを超えていたら0にセット
	if (nIndex >= m_pTextures.size() || nIndex < 0)
	{
		nIndex = 0;
	}
		
	m_pTextures[nIndex] = pTexture;
}

/* ========================================
	セッター(テクスチャ使用フラグ)関数
	-------------------------------------
	引数：int bIsTex
=========================================== */
void ShapeBase::SetUseTexture(int bIsTex)
{
	m_bIsTex = bIsTex;
}


/* ========================================
	セッター(ライト設定(マテリアル))関数
	-------------------------------------
	引数1：float	拡散光
	引数2：float	鏡面反射光
	引数3：float	環境光
	引数4：bool		ライト使用フラグ
=========================================== */
void ShapeBase::SetLightMaterial(float fDiffuse, float fSpecular, float fAmbient, bool bUseLight)
{
	DirectX::XMFLOAT4 param[] =
	{
		{fDiffuse, fSpecular, fAmbient, bUseLight ? 1.0f : 0.0f}
	};

	m_pPS->WriteBuffer(2, param);
}

/* ========================================
	セッター(カメラ座標)関数
	-------------------------------------
	引数1：Vector3<float>	カメラ座標
=========================================== */
void ShapeBase::SetCameraPos(Vector3<float> fCameraPos)
{
	m_pPS->WriteBuffer(3, &fCameraPos);
}

/* ========================================
	セッター(ライト情報)関数
	-------------------------------------
	引数：std::vector<ObjectLight*>	シーン上のライトリスト
=========================================== */
void ShapeBase::SetLights(std::vector<ObjectLight*> lights)
{
	DirectX::XMFLOAT4 param[MAX_LIGHT_NUM][4];

	// 初期化
	for (int i = 0; i < MAX_LIGHT_NUM; i++)
	{
		param[i][0] = { 0.0f, 0.0f, 0.0f, 0.0f };
		param[i][1] = { 0.0f, 0.0f, 0.0f, 0.0f };
		param[i][2] = { 0.0f, 0.0f, 0.0f, 0.0f };
		param[i][3] = { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	// ライト情報をセット
	for (int i = 0; i < lights.size(); ++i)
	{
		float			lightType = static_cast<float>(lights[i]->GetLightType());
		Vector3<float>	lightPos = lights[i]->GetPosition();
		Vector3<float>	lightColor = lights[i]->GetColor();
		Vector3<float>	lightDir = lights[i]->GetDirection();

		param[i][0] = { lightType, lightPos.x, lightPos.y, lightPos.z };			// ライトタイプ、ライト位置
		param[i][1] = { lightColor.x, lightColor.y, lightColor.z, 1.0f };			// ライトカラー
		param[i][2] = { lightDir.x, lightDir.y, lightDir.z, lights[i]->GetRange() };// ライト方向、ライト有効範囲
		param[i][3] = { lights[i]->GetAngle(), 0.0f, 0.0f, 0.0f };					// スポットライト角度
	}

	// 丸影の情報をセット
	int nParamNum = lights.size();
	std::vector<ComponentShadow*> shadows = SHADOW_MNG_INST.GetShadowList();
	for (int i = 0; i < shadows.size(); i++)
	{
		if (nParamNum >= MAX_LIGHT_NUM) break;	// ライト、影合計数が最大数を超えたら終了

		Vector3<float>	vShadowPos = shadows.at(i)->GetPosition();

		param[nParamNum][0] = { 4.0f, vShadowPos.x, vShadowPos.y, vShadowPos.z };	// ライトタイプ(影は固定で4)、影の基準座標
		param[nParamNum][1] = { 0.0f, 0.0f, 0.0f, 0.0f };							// ライトカラー(未使用)
		param[nParamNum][2] = { 0.0f, 0.0f, 0.0f, shadows[i]->GetCircleSize() };	// ライト方向(未使用)、落影の円の大きさ
		param[nParamNum][3] = { 0.0f, 0.0f, 0.0f, 0.0f };							// スポットライト角度(未使用)

		nParamNum++;
	}

	m_pPS->WriteBuffer(4, param);
}

/* ========================================
	セッター(カリング設定)関数
	-------------------------------------
	引数：bool bCulling
=========================================== */
void ShapeBase::SetIsCulling(bool bCulling)
{
	m_bIsCulling = bCulling;
}

/* ========================================
	セッター(Uvオフセット)関数
	-------------------------------------
	引数1：Vector2<float> fOffset
	引数2：int nIndex
=========================================== */
void ShapeBase::SetUvScale(Vector2<float> fScale, int nIndex)
{
	// インデックスがテクスチャ配列のサイズを超えていたら0にセット
	if (nIndex >= m_pTextures.size() || nIndex < 0)
	{
		nIndex = 0;
	}

	m_fUvScale.at(nIndex) = fScale;
}

/* ========================================
	セッター(Uvオフセット)関数
	-------------------------------------
	引数1：Vector2<float> fOffset
	引数2：int nIndex
=========================================== */
void ShapeBase::SetUvOffset(Vector2<float> fOffset, int nIndex)
{
	// インデックスがテクスチャ配列のサイズを超えていたら0にセット
	if (nIndex >= m_pTextures.size() || nIndex < 0)
	{
		nIndex = 0;
	}

	m_fUvOffset.at(nIndex) = fOffset;
}