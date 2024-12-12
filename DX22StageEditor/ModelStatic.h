/* ========================================
	DX22Base/
	------------------------------------
	静的モデル用ヘッダ
	------------------------------------
	説明：静的モデルを扱うためのクラス
	------------------------------------
	ModelStatic.h
========================================== */
#pragma once

// =============== インクルード ===================
#include <DirectXMath.h>
#include <vector>
#include "VertexShader.h"
#include "PixelShader.h"
#include "MeshBuffer.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "ObjectLight.h"


// =============== クラス定義 =====================
class ModelStatic
{
public:
	enum E_Flip
	{
		None,	// DirectX準拠(反転する
		XFlip,	// Maya準拠
		ZFlip	// DirectX準拠(Mayaから180回転した状態
	};
public:
	// 頂点構造体
	struct T_Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT4 color;
	};
	using Vertices = std::vector<T_Vertex>;		// 頂点配列
	using Indexs = std::vector<unsigned long>;	// インデックス配列
	
	// マテリアル構造体
	struct T_Material
	{
		DirectX::XMFLOAT4 diffuse;
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 specular;
		Texture* pTexture;
	};
	using Materials = std::vector<T_Material>;	// マテリアル配列

	// メッシュ構造体
	struct T_Mesh
	{
		Vertices				vertices;
		Indexs					indices;
		unsigned int			materialID;
		std::vector<uint32_t>	boneID;
		MeshBuffer*				pMesh;
	};
	using Meshes = std::vector<T_Mesh>;

public:
	ModelStatic();
	~ModelStatic();

	void Draw();
	void Reset();
	bool Load(const char* file, float scale = 1.0f, E_Flip flip = E_Flip::None);

	// ゲッター
	const T_Mesh* GetMesh(unsigned int index);
	unsigned int GetMeshSize();

	// セッター
	void SetVertexShader(VertexShader* vs);
	void SetPixelShader(PixelShader* ps);

	void SetWVPMatrix(const DirectX::XMFLOAT4X4* matWVP);	// ワールド行列の設定(頂点シェーダー用)
	void SetLightMaterial(float fDiffuse, float fSpecular, float fAmbient, bool bUseLight);		// ライトパラメータ設定(ピクセルシェーダー用)
	void SetCameraPos(Vector3<float> fCameraPos);												// カメラ位置設定(ピクセルシェーダー用)
	void SetLights(std::vector<ObjectLight*> lights);											// ライト設定(ピクセルシェーダー用)

#ifdef _DEBUG
	static std::string GetError();
#endif



private:
	void MakeMesh(const void* ptr, float scale, E_Flip flip);
	void MakeMaterial(const void* ptr, std::string directory);
private:
#ifdef _DEBUG
	static inline std::string		m_sErrorMessage;
#endif

private:
	Meshes					m_MeshList;
	Materials				m_MaterialList;
	VertexShader*			m_pVS;
	PixelShader*			m_pPS;

};


