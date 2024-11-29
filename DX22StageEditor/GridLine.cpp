/* ========================================
	DX22Base/
	------------------------------------
	�O���b�h���`��pcpp
	------------------------------------
	GridLine.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "GridLine.h"
#include "ColorVec3.h"		// �F��`
#include "DebugMenu.h"		// �f�o�b�O���j���[

// =============== �萔��` =======================
const int	GRID_LINE_NUM	= 10;		// 1�����ɂ�����O���b�h���̐�
const float GRID_CELL_SIZE	= 1.0f;	// �O���b�h��1�}�X�̑傫��

// �O���b�h���̐F
const Vector3<float> LINE_COLOR_NORMAL = ColorVec3::GRAY;	// �O���b�h���̐F(�D�F)
const Vector3<float> LINE_COLOR_AXIS[3] = {
	ColorVec3::RED,		// X���̐F(��)
	ColorVec3::GREEN,	// Y���̐F(��)
	ColorVec3::BLUE		// Z���̐F(��)
};


/* ========================================
	�������֐�
	-------------------------------------
	���e�F������
=========================================== */
void GridLine::Init()
{
	m_pGridLine.reset();
	m_pAxisLine.reset();

	m_bIsEnable = true;
	m_fGridSize = GRID_LINE_NUM;
	m_fGridMargin = GRID_CELL_SIZE;
	m_fGridColor = LINE_COLOR_NORMAL;
	m_bIsAxis = true;

#ifdef _DEBUG
	m_bIsEnable = WIN_BASIC_SETTING["Grid"]["Enable"].GetBool();
	m_fGridSize = WIN_BASIC_SETTING["Grid"]["Size"].GetFloat();
	m_fGridMargin = WIN_BASIC_SETTING["Grid"]["Margin"].GetFloat();
	DirectX::XMFLOAT4 color = WIN_BASIC_SETTING["Grid"]["Color"].GetColor();
	m_fGridColor = { color.x, color.y, color.z };
	m_bIsAxis = WIN_BASIC_SETTING["Grid"]["Axis"].GetBool();
#endif

	m_pGridLine = std::make_unique<ShapeLine>(m_fGridSize * m_fGridSize);

	m_pAxisLine = std::make_unique<ShapeLine>(3);	// XYZ���̂�

	MakeGrid();		// �O���b�h���쐬
	MakeGridAxis();	// ���O���b�h���쐬
	
}


/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`��
=========================================== */
void GridLine::Draw()
{
	if (m_bIsEnable)
	{
		m_pGridLine->Draw();

		if (m_bIsAxis)
		{
			m_pAxisLine->Draw();
		}
	}

	
}

/* ========================================
	�O���b�h���쐬�֐�
	-------------------------------------
	���e�F�O���b�h�����쐬
=========================================== */
void GridLine::MakeGrid()
{
	// �O���b�h���̒���
	float fLineLength = m_fGridSize * m_fGridMargin / 2.0f;		// �v���X�ƃ}�C�i�X�̗����ɕ`�悷��ׂɔ����ɂ���

	// ���ȊO�̃O���b�h����`��
	for (int i = 1; i <= (m_fGridSize / 2); i++)
	{
		float fGridLinePos = i * m_fGridMargin;

		// X����
		// �v���X��(��)
		m_pGridLine->AddLine(
			Vector3<float>(-fLineLength, 0.0f, fGridLinePos),	// �J�n�_(���[)
			Vector3<float>( fLineLength, 0.0f, fGridLinePos),	// �I���_(�E�[)
			m_fGridColor);

		// �}�C�i�X��(��O)
		m_pGridLine->AddLine(
			Vector3<float>(-fLineLength, 0.0f, -fGridLinePos),	// �J�n�_(���[)
			Vector3<float>( fLineLength, 0.0f, -fGridLinePos),	// �I���_(�E�[)
			m_fGridColor);

		// Z����
		// �v���X��(�E)
		m_pGridLine->AddLine(
			Vector3<float>(fGridLinePos, 0.0f,  fLineLength),	// �J�n�_(��ʉ��[)
			Vector3<float>(fGridLinePos, 0.0f, -fLineLength),	// �I���_(��ʎ�O�[)
			m_fGridColor);

		// �}�C�i�X��(��)
		m_pGridLine->AddLine(
			Vector3<float>(-fGridLinePos, 0.0f,  fLineLength),	// �J�n�_(��ʉ��[)
			Vector3<float>(-fGridLinePos, 0.0f, -fLineLength),	// �I���_(��ʎ�O�[)
			m_fGridColor);
	}


}

/* ========================================
	���O���b�h���쐬�֐�
	-------------------------------------
	���e�FXYZ���̃O���b�h�����쐬
=========================================== */
void GridLine::MakeGridAxis()
{
	// �O���b�h���̒���
	float fLineLength = m_fGridSize * m_fGridMargin / 2.0f;		// �v���X�ƃ}�C�i�X�̗����ɕ`�悷��ׂɔ����ɂ���

	// ���̃O���b�h����`��
	// X��
	m_pAxisLine->AddLine(
		Vector3<float>(-fLineLength, 0.0f, 0.0f),
		Vector3<float>( fLineLength, 0.0f, 0.0f),
		LINE_COLOR_AXIS[0]);

	// Y��
	m_pAxisLine->AddLine(
		Vector3<float>(0.0f, -fLineLength, 0.0f),
		Vector3<float>(0.0f,  fLineLength, 0.0f),
		LINE_COLOR_AXIS[1]);

	// Z��
	m_pAxisLine->AddLine(
		Vector3<float>(0.0f, 0.0f, -fLineLength),
		Vector3<float>(0.0f, 0.0f,  fLineLength),
		LINE_COLOR_AXIS[2]);
}
