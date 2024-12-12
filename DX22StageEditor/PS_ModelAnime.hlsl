/* ========================================
	DX22Base/
	------------------------------------
	�A�j���[�V�������f���p�s�N�Z���V�F�[�_�[
	------------------------------------
	�����F�A�j���[�V�������f�����f���Ɏg�p����s�N�Z���V�F�[�_�[
	------------------------------------
	PS_ModelAnime.hlsl
========================================== */

// =============== �C���N���[�h ===================
#include "PS_Header.hlsli"

// ���_�V�F�[�_�[����󂯎����͍\����
struct PS_IN
{
    float4 pos		: SV_POSITION;	// ���_���W
    float3 normal	: NORMAL0;		// �@��
    float2 uv		: TEXCOORD0;	// UV���W
    float4 color	: COLOR0;		// ���_�J���[
    float4 pos_w	: POSITION0;	// ���[���h���W
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
    LightData lightDatas[10];
}


// ���C���֐�
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