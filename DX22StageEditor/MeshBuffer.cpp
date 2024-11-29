/* ========================================
	DX22Base/
	------------------------------------
	メッシュバッファ用cpp
	------------------------------------
	MeshBuffer.cpp
========================================== */

// =============== インクルード ===================
#include "MeshBuffer.h"
#include <memory>

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
		　メッシュバッファを初期化する
	-------------------------------------
	引数1：const T_MeshData& desc
=========================================== */
MeshBuffer::MeshBuffer(const T_MeshData& desc)
	: m_pVtxBuffer(NULL)
	, m_pIdxBuffer(NULL)
	, m_tMeshDesc{}
{
	HRESULT hr = E_FAIL;

	// 頂点バッファの作成
	hr = CreateVertexBuffer(desc.pVtx, desc.vtxSize, desc.vtxCount, desc.bWrite);
	if (FAILED(hr))
	{
		OutputDebugStringA("CreateVertexBuffer エラー: 頂点バッファの作成に失敗しました。\n");
		return;
	}

	// インデックスデータが存在する場合はインデックスバッファも作成
	if (desc.pIdx)
	{
		// インデックスバッファの作成
		hr = CreateIndexBuffer(desc.pIdx, desc.idxSize, desc.idxCount);
		if (FAILED(hr))
		{
			OutputDebugStringA("CreateIndexBuffer エラー: インデックスバッファの作成に失敗しました。\n");
			return;
		}
	}

	// メッシュデータを内部メンバにコピー
	m_tMeshDesc = desc;


	// ↓外部から渡されたデータが後で変更されても、内部で保持するデータが影響を受けないようにするため

	// 頂点データのメモリをコピー
	rsize_t vtxMemSize		= desc.vtxSize * desc.vtxCount;		// 頂点データのメモリサイズ
	void* pVtx				= new char[vtxMemSize];				// 頂点データのメモリ確保
	memcpy_s(pVtx, vtxMemSize, desc.pVtx, vtxMemSize);			// 頂点データのメモリコピー
	m_tMeshDesc.pVtx		= pVtx;	

	// インデックスデータのメモリをコピー
	rsize_t idxMemSize		= desc.idxSize * desc.idxCount;		// インデックスデータのメモリサイズ
	void* pIdx				= new char[idxMemSize];				// インデックスデータのメモリ確保
	memcpy_s(pIdx, idxMemSize, desc.pIdx, idxMemSize);			// インデックスデータのメモリコピー
	m_tMeshDesc.pIdx		= pIdx;	


}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：解放
=========================================== */
MeshBuffer::~MeshBuffer()
{
	// 動的に確保された頂点・インデックスデータのメモリを解放
	delete[] m_tMeshDesc.pIdx;
	delete[] m_tMeshDesc.pVtx;

	// DirectXのバッファを解放
	SAFE_RELEASE(m_pIdxBuffer);
	SAFE_RELEASE(m_pVtxBuffer);
}


/* ========================================
	コピーコンストラクタ
	-------------------------------------
	内容：コピーコンストラクタ
		　※アニメモデルのディープコピー時用
=========================================== */
MeshBuffer::MeshBuffer(const MeshBuffer& other)
	: m_pVtxBuffer(NULL)
	, m_pIdxBuffer(NULL)
	, m_tMeshDesc{}
{
	// 自身と同じデータはスルー
	if (this == &other) return;
	
	// 頂点バッファ
	this->CreateVertexBuffer(other.m_tMeshDesc.pVtx, other.m_tMeshDesc.vtxSize, other.m_tMeshDesc.vtxCount, other.m_tMeshDesc.bWrite);

	// インデックスバッファがある場合はコピー
	if (other.m_tMeshDesc.pIdx && other.m_tMeshDesc.idxSize > 0)
		this->CreateIndexBuffer(other.m_tMeshDesc.pIdx, other.m_tMeshDesc.idxSize, other.m_tMeshDesc.idxCount);	// インデックスバッファ
	

	// メッシュデータのコピー -------------------------------------
	this->m_tMeshDesc = other.m_tMeshDesc;
	
	// 頂点データのメモリをコピー
	rsize_t vtxMemSize = other.m_tMeshDesc.vtxSize * other.m_tMeshDesc.vtxCount;// 頂点データのメモリサイズ
	void* pVtx = new char[vtxMemSize];											// 頂点データのメモリ確保
	memcpy_s(pVtx, vtxMemSize, other.m_tMeshDesc.pVtx, vtxMemSize);				// 頂点データのメモリコピー
	this->m_tMeshDesc.pVtx = pVtx;
	
	// インデックスデータのメモリをコピー
	rsize_t idxMemSize = other.m_tMeshDesc.idxSize * other.m_tMeshDesc.idxCount;// インデックスデータのメモリサイズ
	void* pIdx = new char[idxMemSize];											// インデックスデータのメモリ確保
	memcpy_s(pIdx, idxMemSize, other.m_tMeshDesc.pIdx, idxMemSize);				// インデックスデータのメモリコピー
	this->m_tMeshDesc.pIdx = pIdx;

}

/* ========================================
	頂点バッファ作成関数
	-------------------------------------
	内容：頂点バッファを作成する
	-------------------------------------
	引数1：頂点データ
	引数2：頂点のサイズ
	引数3：頂点数
	引数4：動的な頂点データの場合はtrue(頂点が動的に動くオブジェクト等)
	-------------------------------------
	戻り値：HRESULT
=========================================== */
HRESULT MeshBuffer::CreateVertexBuffer(const void* pVtx, UINT size, UINT count, bool isWrite)
{
	// バッファの情報を設定
	D3D11_BUFFER_DESC bufDesc;
	ZeroMemory(&bufDesc, sizeof(bufDesc));
	bufDesc.ByteWidth			= size * count;				// バッファのサイズ (頂点1つのサイズ × 頂点数)
	bufDesc.Usage				= D3D11_USAGE_DEFAULT;		// デフォルト使用
	bufDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとして使用
	bufDesc.MiscFlags			= 0;						// その他のフラグ
	bufDesc.StructureByteStride = 0;						// 構造化バッファの場合のサイズ


	// 書き込み可能なバッファの場合は、動的バッファとして設定
	if (isWrite)
	{
		bufDesc.Usage			= D3D11_USAGE_DYNAMIC;		// 動的バッファに設定
		bufDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;	// CPUからの書き込みを許可
	}

	// 初期データの設定
	D3D11_SUBRESOURCE_DATA subResource;
	ZeroMemory(&subResource, sizeof(subResource));
	subResource.pSysMem				= pVtx;	// 頂点データ
	subResource.SysMemPitch			= 0;	// メモリの1行のサイズ
	subResource.SysMemSlicePitch	= 0;	// メモリの1スライスのサイズ

	// 頂点バッファを作成
	HRESULT hr = DirectXManager::GetDevice()->CreateBuffer(&bufDesc, &subResource, &m_pVtxBuffer);

	return hr;
}

/* ========================================
	インデックスバッファ作成関数
	-------------------------------------
	内容：インデックスバッファを作成する
		　(インデックスとは、頂点の番号を指定して描画するためのデータ)
	-------------------------------------
	引数1：インデックスデータ
	引数2：インデックスのサイズ
	引数3：インデックス数
	-------------------------------------
	戻り値：HRESULT
=========================================== */
HRESULT MeshBuffer::CreateIndexBuffer(const void* pIdx, UINT size, UINT count)
{
	// インデックスサイズが2か4かを確認
	//(インデックスのサイズは一般的に 16 ビット（UINT16）または 32 ビット（UINT32）が使用される)
	if (size != 2 && size != 4) {
		OutputDebugStringA("CreateIndexBuffer エラー: インデックス サイズが無効です。2 バイトまたは 4 バイトにする必要があります。\n");
		return E_FAIL; // エラーを返す
	}

	// バッファ情報の設定
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth			= size * count;				// インデックスのサイズ×数
	bufDesc.Usage				= D3D11_USAGE_DEFAULT;		// デフォルト使用
	bufDesc.BindFlags			= D3D11_BIND_INDEX_BUFFER;	// インデックスバッファとして使用
	bufDesc.CPUAccessFlags		= 0;						// CPUアクセスなし
	bufDesc.MiscFlags			= 0;						// その他のフラグ
	bufDesc.StructureByteStride = 0;						// 構造化バッファの場合のサイズ

	// 初期データの設定
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem				= pIdx;	// インデックスデータ
	subResource.SysMemPitch			= 0;	// メモリの1行のサイズ
	subResource.SysMemSlicePitch	= 0;	// メモリの1スライスのサイズ

	// インデックスバッファを作成
	HRESULT hr;
	hr = DirectXManager::GetDevice()->CreateBuffer(&bufDesc, &subResource, &m_pIdxBuffer);

	return hr;
}


/* ========================================
	描画関数
	-------------------------------------
	内容：頂点バッファとインデックスバッファを使って描画
	-------------------------------------
	引数1：count	描画する頂点数(0の場合は全ての頂点を描画)
=========================================== */
void MeshBuffer::Draw(int count)
{
	// デバイスコンテキストを取得
	ID3D11DeviceContext* pContext = DirectXManager::GetContext();	

	UINT stride = m_tMeshDesc.vtxSize;  // 頂点1つ分のサイズ
	UINT offset = 0;					// オフセット(頂点バッファ内での開始位置)

	// トポロジー(プリミティブの種類)を設定
	pContext->IASetPrimitiveTopology(m_tMeshDesc.topology);
	// 頂点バッファをセット
	pContext->IASetVertexBuffers(0, 1, &m_pVtxBuffer, &stride, &offset);

	// インデックスバッファがある場合、インデックス付き描画
	if (m_tMeshDesc.idxCount > 0)
	{
		DXGI_FORMAT format;
		ZeroMemory(&format, sizeof(format));
		// インデックスサイズによってフォーマットを設定
		switch (m_tMeshDesc.idxSize)
		{
		case 4: format = DXGI_FORMAT_R32_UINT; break;
		case 2: format = DXGI_FORMAT_R16_UINT; break;
		}
		// インデックスバッファをセット
		pContext->IASetIndexBuffer(m_pIdxBuffer, format, 0);
		pContext->DrawIndexed(count ? count : m_tMeshDesc.idxCount, 0, 0);  // 指定された数か、全てのインデックスを描画
	}
	else
	{
		// インデックスバッファがない場合、頂点のみで描画
		pContext->Draw(count ? count : m_tMeshDesc.vtxCount, 0);
	}

}

/* ========================================
	新規頂点データ書込関数
	-------------------------------------
	内容：頂点データの書き込み
	-------------------------------------
	引数1：頂点バッファのポインタ
	-------------------------------------
	戻り値：HRESULT
=========================================== */
HRESULT MeshBuffer::WriteVertexData(void* pVtx)
{
	// ポインタが無効な場合はエラーを返す
	if (!pVtx) { return E_POINTER; }

	// 動的バッファでない場合はエラーを返す
	if (!m_tMeshDesc.bWrite) { return E_FAIL; }

	ID3D11DeviceContext* pContext = DirectXManager::GetContext();	// デバイスコンテキストを取得
	D3D11_MAPPED_SUBRESOURCE mapResource;							// マップリソース(頂点データが格納される)

	// 頂点バッファにデータをマッピングして書き込み
	HRESULT hr = pContext->Map(m_pVtxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	if (FAILED(hr)) {
		OutputDebugStringA("頂点バッファのマッピングに失敗しました.\n");
		return hr;
	}

	// 新しい頂点データをコピー
	rsize_t size = m_tMeshDesc.vtxCount * m_tMeshDesc.vtxSize;	// 頂点データのサイズ
	memcpy_s(mapResource.pData, size, pVtx, size);				// 頂点データのコピー(pVtx→mapResource.pData)

	pContext->Unmap(m_pVtxBuffer, 0);	// マップ解除

	return S_OK;
}

/* ========================================
	メッシュデータ取得関数
	-------------------------------------
	内容：メッシュデータの取得
	-------------------------------------
	戻り値：T_MeshData
=========================================== */
MeshBuffer::T_MeshData MeshBuffer::GetDesc()
{
	return m_tMeshDesc;
}
