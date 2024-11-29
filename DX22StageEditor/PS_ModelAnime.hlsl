/* ========================================
	DX22Base/
	------------------------------------
	�A�j���[�V�������f���p�s�N�Z���V�F�[�_�[
	------------------------------------
	�����F�A�j���[�V�������f�����f���Ɏg�p����s�N�Z���V�F�[�_�[
	------------------------------------
	PS_ModelAnime.hlsl
========================================== */

// ���_�V�F�[�_�[����󂯎����͍\����
struct PS_IN
{
    float4 pos		: SV_POSITION;
    float3 normal	: NORMAL0;
    float2 uv		: TEXCOORD0;
    float4 color	: COLOR0;
    float4 pos_w	: POSITION0;
};


Texture2D tex : register(t0); // �e�N�X�`��
SamplerState samp : register(s0); // �T���v���[�X�e�[�g


// �s�N�Z���V�F�[�_�[�̃��C���֐�
float4 main(PS_IN pin) : SV_TARGET
{
    return tex.Sample(samp, pin.uv);
}