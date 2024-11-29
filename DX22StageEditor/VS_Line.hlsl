/* ========================================
	DX22Base/
	------------------------------------
	���`��p���_�V�F�[�_�[
	------------------------------------
	�����F��(�O���b�h����A���C��)��`�悷�邽�߂̃s�N�Z���V�F�[�_�[
	------------------------------------
	VS_Line.hlsl
========================================== */

// ���_�V�F�[�_�[���͍\����
struct VS_IN
{
    float3 pos : POSITION0;
    float4 color : COLOR0;
};
// ���_�V�F�[�_�[�o�͍\����
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

// �萔�o�b�t�@(���[���h�A�r���[�A�v���W�F�N�V�����s��)
cbuffer bufMat : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

// ���C���֐�
VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, world);
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    vout.color = vin.color;
    return vout;
}