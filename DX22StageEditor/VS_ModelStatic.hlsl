/* ========================================
	DX22Base/
	------------------------------------
	�ÓI���f���p���_�V�F�[�_�[
	------------------------------------
	�����F�ÓI���f���Ɏg�p���钸�_�V�F�[�_�[
	------------------------------------
	VS_ModelStatic.hlsl
========================================== */

// ���_�V�F�[�_�[���͍\����
struct VS_IN
{
    float3 pos : POSITION0;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

// ���_�V�F�[�_�[�o�͍\����
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

// �萔�o�b�t�@(���[���h�A�r���[�A�v���W�F�N�V�����s��)
cbuffer WVP : register(b0)
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
    vout.pos = mul(vout.pos, world);    // ���[�J�����W���烏�[���h���W�֕ϊ�
    vout.pos = mul(vout.pos, view);     // ���[���h���W����r���[���W�֕ϊ�
    vout.pos = mul(vout.pos, proj);     // �r���[���W����v���W�F�N�V�������W�֕ϊ�
	
    vout.normal = mul(vin.normal, (float3x3) world);    // �@���x�N�g�������[���h���W�ɕϊ�
    vout.uv     = vin.uv;                               // UV���W�����̂܂܏o��

    return vout;
}