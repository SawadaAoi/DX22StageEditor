/* ========================================
	DX22Base/
	------------------------------------
	�}�`�`��p�s�N�Z�����_�V�F�[�_�[
	------------------------------------
	�����F�e�X�g�p�̐}�`�`��p���_�V�F�[�_�[
	------------------------------------
	VS_Shape.hlsl
========================================== */

// ���_�V�F�[�_�[���͍\����
struct VS_IN
{
    float3 pos      : POSITION0;
    float3 normal   : NORMAL0; // �f�[�^�̗p�r���`����
    float2 uv       : TEXCOORD0;
};

// ���_�V�F�[�_�[�o�͍\����
struct VS_OUT
{
    float4 pos      : SV_POSITION;
    float3 normal   : NORMAL0; // �f�[�^�̗p�r���`����
    float2 uv       : TEXCOORD0;
    float4 pos_w    : POSITION0;
};

// �萔�o�b�t�@
// �`�悷�郂�f���̃��[���h�ϊ��s��A�r���[�ϊ��s��A�v���W�F�N�V�����ϊ��s��
cbuffer bufMat : register(b0)
{
    float4x4 world; // ���[���h�ϊ��s��
    float4x4 view;  // �r���[�ϊ��s��
    float4x4 proj;  // �v���W�F�N�V�����ϊ��s��
};

// ���C���֐�
VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    vout.pos    = float4(vin.pos, 1.0f);
    vout.pos    = mul(vout.pos, world);
    vout.pos_w  = vout.pos;                 // ���[���h���W���o��
    vout.pos    = mul(vout.pos, view);
    vout.pos    = mul(vout.pos, proj);
    
    vout.normal = mul(vin.normal, (float3x3)world);
    vout.uv     = vin.uv;
    
    return vout;
}