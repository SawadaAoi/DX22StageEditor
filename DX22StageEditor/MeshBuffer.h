/* ========================================
	DX22Base/
	------------------------------------
	メッシュバッファ用ヘッダ
	------------------------------------
	説明：メッシュバッファを管理するクラス
	(頂点バッファ、インデックスバッファ等)
	------------------------------------
	MeshBuffer.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "DirectXManager.h"

// =============== クラス ===================
class MeshBuffer
{
public:

	// メッシュデータの構造体
	struct T_MeshData
	{
		const void*					pVtx;		// 頂点データのポインタ
		UINT						vtxSize;	// 各頂点のサイズ	
		UINT						vtxCount;	// 頂点数	
		bool						bWrite;		// 頂点データが書き込み可能か
		const void*					pIdx;		// インデックスデータのポインタ
		UINT						idxSize;	// 各インデックスのサイズ
		UINT						idxCount;	// インデックス数
		D3D11_PRIMITIVE_TOPOLOGY	topology;	// プリミティブトポロジー（例: 三角形リストやラインリスト）
	};
public:
	MeshBuffer(const T_MeshData& desc);
	~MeshBuffer();

	// コピーコンストラクタ
	MeshBuffer(const MeshBuffer& other);

	void Draw(int count = 0);
	HRESULT WriteVertexData(void* pVtx);

	// メッシュデータの取得
	T_MeshData GetDesc();

private:
	HRESULT CreateVertexBuffer(const void* pIdx, UINT size, UINT count, bool isWrite);
	HRESULT CreateIndexBuffer(const void* pVtx, UINT size, UINT count);

private:
	ID3D11Buffer* m_pVtxBuffer;		// 頂点バッファ
	ID3D11Buffer* m_pIdxBuffer;		// インデックスバッファ
	T_MeshData	m_tMeshDesc;		// メッシュデータの構造体

};

