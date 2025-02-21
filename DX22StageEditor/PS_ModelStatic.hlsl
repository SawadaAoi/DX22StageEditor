/* ========================================
	DX22Base/
	------------------------------------
	�ÓI���f���p�s�N�Z���V�F�[�_�[
	------------------------------------
	�����F�ÓI���f���Ɏg�p����s�N�Z���V�F�[�_�[
	------------------------------------
	PS_ModelStatic.hlsl
========================================== */

// =============== �C���N���[�h ===================
#include "PS_Header.hlsli"

// ���_�V�F�[�_�[����󂯎����͍\����
struct PS_IN
{
    float4 pos		: SV_POSITION;
    float3 normal	: NORMAL0;
    float2 uv       : TEXCOORD0;
    float4 color	: COLOR0;
    float4 pos_w    : POSITION0; // ���[���h���W
};

// ���C�g�̔��ːݒ�
cbuffer BufLightStrength : register(b0)
{
    float fDiffuseStr;
    float fSpecularStr;
    float fAmbientStr;
    int bUseLight; // ���C�g���Q�Ƃ��邩�ǂ���
};

// �J�����̈ʒu
cbuffer BufCamera : register(b1)
{
    float3 fCameraPos;
}


// �V�[����̃��C�g���
cbuffer BufLight : register(b2)
{
    LightData lightDatas[MAX_LIGHT];
}

// �s�N�Z���V�F�[�_�[�̃��C���֐�
float4 main(PS_IN pin) : SV_TARGET
{
    // ���C�g���Q�Ƃ��邩�ǂ���
    if (bUseLight == 0)
    {
        return tex.Sample(samp, pin.uv);
    }
    else
    {
        float4 finalColor = tex.Sample(samp, pin.uv); // �e�N�X�`���̐F
        finalColor.rgb *= 
                GetTotalLightColor(pin.normal, pin.pos_w, 
                    fCameraPos, lightDatas, fDiffuseStr, fSpecularStr, fAmbientStr).rgb; // �V�[����̃��C�g�̐F�𔽉f
        
        return finalColor;
    }
}