/* ========================================
	DX22Base/
	------------------------------------
	�A�j���[�V�������f���p���_�V�F�[�_�[
	------------------------------------
	�����F�A�j���[�V�������f���Ɏg�p���钸�_�V�F�[�_�[
	------------------------------------
	VS_ModelAnime.hlsl
========================================== */

// ���_�V�F�[�_�[���͍\����
struct VS_IN
{
    float3 pos      : POSITION;
    float3 normal   : NORMAL0;
    float2 uv       : TEXCOORD0;
    float4 color    : COLOR0;
    float4 weight   : WEIGHT0;
    uint4  index    : INDEX0;
};

// ���_�V�F�[�_�[�o�͍\����
struct VS_OUT
{
    float4 pos      : SV_POSITION;
    float3 normal   : NORMAL0;
    float2 uv       : TEXCOORD0;
    float4 color    : COLOR0;
    float4 pos_w    : POSITION0;
};

// �萔�o�b�t�@(���[���h�A�r���[�A�v���W�F�N�V�����s��)
cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

// �萔�o�b�t�@(�{�[���s��)
cbuffer Bone : register(b1)
{
    float4x4 bone[200];
};

// ���C���֐�
VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    float4x4 anime;
    // �{�[���s��̌v�Z
    anime  = bone[vin.index.x] * vin.weight.x;
    anime += bone[vin.index.y] * vin.weight.y;
    anime += bone[vin.index.z] * vin.weight.z;
    anime += bone[vin.index.w] * vin.weight.w;
    
    vout.pos    = float4(vin.pos, 1.0f);
    vout.pos    = mul(vout.pos, anime);
    vout.pos    = mul(vout.pos, world);
    vout.pos_w  = vout.pos;             // ���[���h���W���o��
    vout.pos    = mul(vout.pos, view);
    vout.pos    = mul(vout.pos, proj);
    
    vout.normal = vin.normal;
    vout.normal = mul(vout.normal, (float3x3) anime);
    vout.normal = mul(vout.normal, (float3x3) world);
    
    vout.uv     = vin.uv;
    vout.color  = vin.color;
    return vout;
};