/* ========================================
	DX22Base/
	------------------------------------
	UI�摜�p�s�N�Z���V�F�[�_�[
	------------------------------------
	�����FUI�̉摜��`�悷�邽�߂̃s�N�Z���V�F�[�_�[
	------------------------------------
	PS_Sprite.hlsl
========================================== */

// �s�N�Z�����͍\����
struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

Texture2D		tex  : register(t0);	// �e�N�X�`��
SamplerState	samp : register(s0);	// �T���v���[�X�e�[�g

// ���C���֐�
float4 main(PS_IN pin) : SV_TARGET
{
    return tex.Sample(samp, pin.uv) * pin.color;
}