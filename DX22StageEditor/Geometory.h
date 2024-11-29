/* ========================================
	DX22Base/
	------------------------------------
	�}�`�\���p�w�b�_
	------------------------------------
	�����F�}�`�\���p�̃N���X
		�@�����A�����́A�~���A���̕`����s��
	------------------------------------
	Geometory.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include <DirectXMath.h>
#include "VertexShader.h"
#include "PixelShader.h"
#include "MeshBuffer.h"


// =============== �N���X�̒�` ===================
class Geometory
{
private:

	// �����`��p�\����
	struct LineVertex
	{
		float pos[3];
		float color[4];
	};
	// ���_�\����
	struct Vertex
	{
		float pos[3];	// ���W
		float uv[2];	// �e�N�X�`�����W
	};
public:
	static void Init();
	static void Uninit();

	static void SetWorld(DirectX::XMFLOAT4X4 world);
	static void SetView(DirectX::XMFLOAT4X4 view);
	static void SetProjection(DirectX::XMFLOAT4X4 proj);

	static void AddLine(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end,
		DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	static void DrawLines();
	static void DrawBox();
	static void DrawCylinder();
	static void DrawSphere();

private:
	static void MakeVS();
	static void MakePS();
	static void MakeLineShader();
	static void MakeLine();

private:
	static void MakeBox();
	static void MakeCylinder();
	static void MakeSphere();

private:
	static const int MAX_LINE_NUM = 1000;
	static const int CIRCLE_DETAIL = 16;
private:
	static inline MeshBuffer* m_pBox;
	static inline MeshBuffer* m_pCylinder;
	static inline MeshBuffer* m_pSphere;
	static inline MeshBuffer* m_pLines;
	static inline Shader* m_pVS;
	static inline Shader* m_pPS;
	static inline Shader* m_pLineShader[2];
	static inline DirectX::XMFLOAT4X4 m_WVP[3];
	static inline void* m_pLineVtx;
	static inline int m_lineCnt;
};

