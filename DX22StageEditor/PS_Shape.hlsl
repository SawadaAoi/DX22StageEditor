/* ========================================
	DX22Base/
	------------------------------------
	�}�`�`��p�s�N�Z���V�F�[�_�[
	------------------------------------
	�����F�e�X�g�p�̐}�`�`��p�s�N�Z���V�F�[�_�[
	------------------------------------
	PS_Shape.hlsl
========================================== */

// =============== �C���N���[�h ===================
#include "PS_Header.hlsli"

// ���_�V�F�[�_�[����󂯎����͍\����
struct PS_IN
{
    float4 pos      : SV_POSITION;
    float3 normal   : NORMAL0; 
    float2 uv       : TEXCOORD0;
    float4 pos_w    : POSITION0;
};

// �萔�o�b�t�@(�F)
cbuffer bufColor : register(b0)
{
    float3 BaseColor;   // �x�[�X�̐F
};

// �萔�o�b�t�@(�t���O)
cbuffer bufFlg : register(b1)
{
    int nDrawMode;  // �`�惂�[�h(0:�ʏ�`��, 1:���C���[�t���[���`��)
    int bUseTex;    // �e�N�X�`���g�p(0:�g�p���Ȃ�, 1:�g�p����)
};

// ���C�g�̔��ːݒ�
cbuffer BufLightStrength : register(b2)
{
    float fDiffuseStr;
    float fSpecularStr;
    float fAmbientStr;
    int bUseLight; // ���C�g���Q�Ƃ��邩�ǂ���
};

// �J�����̈ʒu
cbuffer BufCamera : register(b3)
{
    float3 fCameraPos;
}


// �V�[����̃��C�g���
cbuffer BufLight : register(b4)
{
    LightData lightDatas[10];
}

// �s�N�Z���V�F�[�_�[�̃��C���֐�
float4 main(PS_IN pin) : SV_TARGET0
{
    float4 fColor    = float4(0, 0, 0, 1);  // �����l�͔�
    
    // �e�N�X�`���g�p�̏ꍇ
    if (bUseTex == 1)
    {
         // ���C�g���Q�Ƃ��邩�ǂ���
        if (bUseLight == 0)
        {
            fColor = tex.Sample(samp, pin.uv);
        }
        else
        {
            float4 finalColor = tex.Sample(samp, pin.uv); // �e�N�X�`���̐F
            finalColor.rgb *=
                GetTotalLightColor(pin.normal, pin.pos_w,
                    fCameraPos, lightDatas, fDiffuseStr, fSpecularStr, fAmbientStr).rgb; // �V�[����̃��C�g�̐F�𔽉f
        
            fColor = finalColor;
        }

    }
    // �e�N�X�`�����g�p�̏ꍇ
    else
    {
        // �ʏ�`��̏ꍇ
        if (nDrawMode == 0)
        {
            float2 fHalfGrid = floor(abs(pin.uv) * 2.0f); // 0.0�`1.0�͈̔͂�2x2�̃O���b�h�ɕ���(�E��A����A�E���A������4�����ɃO���[�v����)
            float2 fQuatGrid = floor(abs(pin.uv) * 8.0f); // 0.0�`1.0�͈̔͂�8x8�̃O���b�h�ɕ���(����64�����o�[�W����)

        // �΂߂̈ʒu�ŃO���[�v����(��F�l�����̏ꍇ�A�i����A�E���j�Ɓi�E��A�����j��1��0���Z�b�g����)
            float fHalf = fmod(fHalfGrid.x + fHalfGrid.y, 2.0f);
            float fQuat = fmod(fQuatGrid.x + fQuatGrid.y, 2.0f);

            fColor.rgb = ((fHalf * 0.1f) * fQuat + 0.45f) * BaseColor.rgb + (1 - fQuat) * 0.05f;
        }
         // ���C���[�t���[���`��̏ꍇ
        else if (nDrawMode == 1)
        {
            fColor.rgb = fColor.rgb + BaseColor.rgb; // �x�[�X�J���[�ŕ`��
        }
    }
    
    return fColor;
}