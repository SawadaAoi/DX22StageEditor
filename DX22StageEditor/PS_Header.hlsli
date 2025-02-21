/* ========================================
	DX22Base/
	------------------------------------
	���C�g�p�܂Ƃ߃V�F�[�_�[�w�b�_
	------------------------------------
	����:���C�g�Ŏg���֐��A�\���̂��L�ڂ���
	------------------------------------
	LightHeader.hlsli
========================================== */

// =============== �\���̒�` =====================
// ���_�V�F�[�_�[����󂯎�邽�߂̃f�[�^���`
//struct PS_IN
//{
//	float4 pos		: SV_POSITION0;
//	float3 normal	: NORMAL0;
//	float2 uv		: TEXCOORD0;

//};

//// ���_�V�F�[�_�[����󂯎�邽�߂̃f�[�^���`
//struct PS_IN_W
//{
//	float4 pos		: SV_POSITION0;
//	float3 normal	: NORMAL0;
//	float4 uv		: TEXCOORD0;
//	float4 worldPos : TEXCOORD1;
//};

// =============== �萔��` ========================
static const int MAX_LIGHT = 20; // ���C�g�̍ő吔

// =============== �ϐ���` ========================
// �e�N�X�`�����󂯎��
Texture2D       tex : register(t0);
SamplerState    samp : register(s0); // �e�N�X�`���̌J��Ԃ��ݒ�

// ���C�g�̃f�[�^
struct LightData
{
    float lightType; // ���C�g�̎��
    float3 lightPos; // ���C�g�̍��W
    float4 lightColor; // ���C�g�̐F
    float3 lightDir; // ���C�g�̕���
    float range; // ���C�g�̉e���͈�
    float angle; // �X�|�b�g���C�g�̊p�x
    float3 dummy; // 16�o�C�g�ɑ�����ׂ̃_�~�[
};

/* ========================================
	�g�U���ˌ��擾�֐�
	-------------------------------------
	���e�F�@���ƃ��C�g�x�N�g������g�U���ˌ������߂�
	-------------------------------------
	����1�F�@���x�N�g��
	����2�F���C�g�����x�N�g��
	����3�F���C�g�̐F
	-------------------------------------
	�ߒl�F�g�U���ˌ���RGB
=========================================== */
float3 GetDiffuseLight(float3 normal, float3 lightDir, float4 lightColor)
{
	//�@��
	// normalize�����K��(�P�ʃx�N�g���ϊ�)
    float3 N = normalize(normal);

	// ���C�g�̕���
    float3 L = normalize(lightDir);
	// ���ςƂ̌v�Z���ʂ������ڂɍ����悤�ɁA���O�Ƀ��C�g�̕����𔽓]�����Ă���
    L *= -1.0f;

	// ���ς̌v�Z
    float d = dot(L, N);
    d = saturate(d); // 0������0,1���傫����1�ɂ���

	// ���̐F���v�Z
    float3 diffuseLight = lightColor.rgb * d;

    return diffuseLight;
}


/* ========================================
	���ʔ��ˌ��擾�֐�
	-------------------------------------
	���e�F�@���ƃ��C�g�x�N�g������g�U���ˌ������߂�
	-------------------------------------
	����1�F�@���x�N�g��
	����2�F���[���h���W
	����3�F���C�g�����x�N�g��
	����4�F���C�g�̐F
	-------------------------------------
	�ߒl�F�g�U���ˌ���RGB
=========================================== */
float3 GetSpecularLight(float3 normal, float4 worldPos, float3 lightDir, float4 lightColor, float3 cameraPos)
{
	// ���˃x�N�g���̌v�Z
    float3 N = normalize(normal);       // �@��
    float3 R = reflect(lightDir, N);    // ���˃x�N�g��
    R = normalize(R);

	// ����(�J����)�Ɍ������x�N�g���̌v�Z
    float3 V = cameraPos - worldPos.xyz;    //�J�������W-�I�u�W�F�N�g���g�̍��W
    V = normalize(V);

	// ���ʔ��˂̋��������߂�
    float d = dot(R, V); // ���˃x�N�g���ƃI�u�W�F�N�g���J���������x�N�g���̌������߂��قǋ����Ȃ�(1�ɋ߂Â��ق�)
	// ���ς̌��ʂ͕���(�t�����̏ꍇ)�ɂȂ�̂ŁA�����̏ꍇ��0�ɂȂ�
    if (d < 0.0f)
    {
        d = 0.0f;
    }

	// ���ʔ��˂̋������i��
    d = pow(d, 5.0f); // ��������̂�d��1�ɋ߂��Ƃ��ƁA0�ɋ߂��Ƃ��̍����傫���Ȃ�B�΂߂̒������琂�ꂽ���[�v�̗l�ȃO���t�ɂȂ�

	// ���ʔ��˂̌��̐F���v�Z
    float3 specularLight = lightColor.rgb * d;

    return specularLight;
}

/* ========================================
    �_�������擾�֐�
    -------------------------------------
    ���e�F�@���ƃ��C�g�x�N�g������g�U���ˌ������߂�
    -------------------------------------
    ����1�F�@���x�N�g��
    ����2�F���[���h���W
    ����3�F���C�g�̍��W
    ����4�F���C�g�̉e���͈�
    ����5�F���C�g�̐F
    -------------------------------------
    �ߒl�F�_������RGB
=========================================== */
float3 GetPointLight(float3 normal, float4 worldPos, float3 lightPos, float range, float4 lightColor,
                     float3 fCameraPos, float fDiffuseStr, float fSpecularStr)
{
    float3 pointLight = float3(0.0f, 0.0f, 0.0f);
	
    float3 N = normalize(normal); // �@��
	
	// ���C�g�̕���
    float3 L = worldPos.xyz - lightPos;
    L = normalize(L); // ���K��
	
	// ���C�g����̋���
    float distance = length(worldPos.xyz - lightPos);
	
	// �����ɂ��e���x(0�`1)
    float affect = saturate(1.0f - distance / range);
	// ��������̂�d��1�ɋ߂��Ƃ��ƁA0�ɋ߂��Ƃ��̍����傫���Ȃ�B�΂߂̒������琂�ꂽ���[�v�̗l�ȃO���t�ɂȂ�
    affect = pow(affect, 2.0f);
	
	// ���ˌ��̌v�Z
    float3 diffuse = GetDiffuseLight(normal, L, lightColor) * fDiffuseStr;
    float3 specular = GetSpecularLight(normal, worldPos, L, lightColor, fCameraPos) * fSpecularStr;
	
    pointLight.rgb = (diffuse + specular) * affect;
	
    return pointLight;
}

/* ========================================
    �X�|�b�g���C�g���擾�֐�
    -------------------------------------
    ���e�F�@���ƃ��C�g�x�N�g������g�U���ˌ������߂�
    -------------------------------------
    ����1�F�@���x�N�g��
    ����2�F���[���h���W
    ����3�F���C�g�̍��W
    ����4�F���C�g�̕���
    ����5�F���C�g�̉e���͈�
    ����6�F���C�g�̐F
    ����7�F�X�|�b�g���C�g�̊p�x
    -------------------------------------
    �ߒl�F�X�|�b�g���C�g��RGB	
=========================================== */
float3 GetSpotLight(float3 normal, float4 worldPos, float3 lightPos, float3 lightDir, float range, float4 lightColor, float angle,
                    float3 fCameraPos, float fDiffuseStr, float fSpecularStr)
{
    float3 spotLight = float3(0.0f, 0.0f, 0.0f);
    
    float3 N = normalize(normal); // �@��
    
	// ���C�g�̕���
    float3 L = worldPos.xyz - lightPos;
    L = normalize(L); // ���K��
    // ���C�g����̋���
    float distance = length(worldPos.xyz - lightPos);
	
    // �����ɂ��e���x(0�`1)
    float affect = saturate(1.0f - distance / range);
    // ��������̂�d��1�ɋ߂��Ƃ��ƁA0�ɋ߂��Ƃ��̍����傫���Ȃ�B�΂߂̒������琂�ꂽ���[�v�̗l�ȃO���t�ɂȂ�
    affect = pow(affect, 2.0f);
	
	// ���C�g�̏Ǝ˕����Ɩ@���̊p�x�����߂�
    float cosAngle = dot(lightDir, L);
    float objAngle = acos(cosAngle);
	
	// �p�x���w�肳�ꂽ�p�x�����傫���ꍇ�͉e���x��0�ɂ���
    float spotAffect = saturate(1.0f - (objAngle / angle)); // �p�x�ɂ��e���x(0�`1))
    spotAffect = pow(spotAffect, 0.5f);
	
	// ���ˌ��̌v�Z
    float3 diffuse = GetDiffuseLight(normal, L, lightColor) * fDiffuseStr;
    float3 specular = GetSpecularLight(normal, worldPos, L, lightColor, fCameraPos) * fSpecularStr;
    
    spotLight.rgb = (diffuse + specular) * affect * spotAffect;
    
    return spotLight;
}

/* ========================================
    �e�`�F�b�N�֐�
    -------------------------------------
    ���e�F�e�̃`�F�b�N��
    -------------------------------------
    ����1�F���[���h���W
    ����2�F�e�̒��S���W
    ����3�F�e�̔��a
    -------------------------------------
    �ߒl�F�F�̌����l
=========================================== */
float CheckShadow(float3 normal, float4 worldPos, float3 shadowCenter, float shadowRadius)
{
    // ������x�N�g���Ɩ@���̓��ς����߂�
    float dotValue = dot(normal, float3(0, 1, 0));
    if(dotValue < 0.0f) return 1.0f; // �@�����t�����̏ꍇ�͉e����
    
     // ���g���������ʒu�ɂ���ꍇ�͉e����
    if (shadowCenter.y < worldPos.y)    return 1.0f;
    
    // �ォ�猩�����W�ł̋��������߂�
    float distanceXZ = length(worldPos.xz - shadowCenter.xz);
    
     // ��������������Ɖe�̑傫��������������
    float distanceHeight    = length(shadowCenter.y - worldPos.y);      // �����̍�
    shadowRadius            = shadowRadius - (distanceHeight * 0.2f);   // �~�̑傫�� - �����̍� * �W��
    
    // �e�͈̔͊O�̏ꍇ�͉e����
    if (distanceXZ > shadowRadius)  return 1.0f;
   
    return 0.5f;
}

/* ========================================
    ���C�g���v�F�擾�֐�
    -------------------------------------
    ���e�F���C�g�̍��v�F���擾����
    -------------------------------------
    ����1�F�@���x�N�g��
    ����2�F���[���h���W
    -------------------------------------
    �ߒl�F���C�g�̍��v�F
=========================================== */
float4 GetTotalLightColor(float3 normal, float4 worldPos, float3 fCameraPos,
    LightData lightDatas[MAX_LIGHT], float fDiffuseStr, float fSpecularStr, float fAmbientStr)
{
    float4 LightTotal = float4(0, 0, 0, 1);
    float fShadowPower = 1.0f;
        
    for (int i = 0; i < MAX_LIGHT; i++)
    {
        LightData light = lightDatas[i];
            
            // ���C�g�̎�ނ�0�̏ꍇ(���݂��Ȃ�)
        if (light.lightType == 0.0f)
            continue;
            
            // �������C�g
        if (light.lightType == 1.0f)
        {
            LightTotal.rgb +=
                    (GetDiffuseLight(normal, light.lightDir, light.lightColor) * fDiffuseStr) +
                    (GetSpecularLight(normal, worldPos, light.lightDir, light.lightColor, fCameraPos) * fSpecularStr);
        }
            // �_����
        else if (light.lightType == 2.0f)
        {
            LightTotal.rgb +=
                    GetPointLight(normal, worldPos, light.lightPos, light.range, light.lightColor,
                                    fCameraPos, fDiffuseStr, fSpecularStr);
        }
        // �X�|�b�g���C�g
        else if (light.lightType == 3.0f)
        {
            LightTotal.rgb += GetSpotLight(normal, worldPos,
                    light.lightPos, light.lightDir, light.range, light.lightColor, light.angle,
                    fCameraPos, fDiffuseStr, fSpecularStr);
        }
        // �e
        else if (light.lightType == 4.0f)
        {
            fShadowPower *= CheckShadow(normal, worldPos, light.lightPos, light.range);
        }

    }
    
    LightTotal.rgb += fAmbientStr; // ���������Z
		
    LightTotal.rgb *= fShadowPower; // �e�̉e���x�𔽉f
    
    return LightTotal;
}