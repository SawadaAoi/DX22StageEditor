/* ========================================
	DX22Base/
	------------------------------------
	�t�@�C�������Ǘ��pcpp
	------------------------------------
	FileManager.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "FileManager.h"
// �t�@�C�������p
#include <iostream>		
#include <fstream>		

#include "SceneManager.h"
#include "SceneBase.h"
#include "ObjectBase.h"

#include "ComponentTransform.h"
#include "ObjectTypeRegistry.h"

#include <Windows.h>		// ���b�Z�[�W�{�b�N�X�p
#include <unordered_map>


/* ========================================
	�t�@�C���o��(�V�[���I�u�W�F�N�g)�֐�
	-------------------------------------
	���e�F�I�u�W�F�N�g�̏����t�@�C���ɏo�͂���
	-------------------------------------
	����1�F�t�@�C���p�X
========================================== */
void FileManager::StageObjectOutput(std::string sPath)
{
	// �t�@�C�����J��
	std::ofstream file(sPath, std::ios::out | std::ios::binary);

	// �t�@�C�����J���Ȃ�������I��
	if (!file.is_open())
	{
		// ���b�Z�[�W�\��(���s)
		WIN_DATA_INOUT["OutputResult"].SetText("< Failed > " + sPath.substr(sPath.find_last_of('/') + 1));

		return;
	}

	// �f�[�^���o�͎��̃I�v�V����
	bool bCamera	= WIN_DATA_INOUT["Camera"].GetBool();			// �J�����̏o�͐ݒ�
	bool bLight		= WIN_DATA_INOUT["Light"].GetBool();			// ���C�g�̏o�͐ݒ�
	bool bTerrain	= WIN_DATA_INOUT["Terrain"].GetBool();			// �n�`�̏o�͐ݒ�
	bool bEnemy		= WIN_DATA_INOUT["Enemy"].GetBool();			// �G�L�����̏o�͐ݒ�
	bool bTransOnly = WIN_DATA_INOUT["TransformOnly"].GetBool();	// �ʒu�A��]�A�g��݂̂̏o�͐ݒ�

	// �V�[���ɑ��݂���I�u�W�F�N�g���擾
	for (auto& pObject : SceneManager::GetScene()->GetAllSceneObjects())
	{
		// ���O�`�F�b�N
		if (pObject->GetTag() == E_ObjectTag::Camera && !bCamera) continue;
		if (pObject->GetTag() == E_ObjectTag::Light && !bLight) continue;
		if (pObject->GetTag() == E_ObjectTag::Ground && !bTerrain) continue;
		if (pObject->GetTag() == E_ObjectTag::Goal && !bTerrain) continue;
		if (pObject->GetTag() == E_ObjectTag::Wall && !bTerrain) continue;
		if (pObject->GetTag() == E_ObjectTag::Enemy && !bEnemy) continue;

		if (!pObject->GetIsSave()) continue;	// �ۑ��t���O�������Ă��Ȃ��ꍇ�̓X�L�b�v

		// �I�u�W�F�N�g�̏����i�[����\����
		S_SaveDataObject data;

		// �I�u�W�F�N�g�̏����擾 ---------------------------------
		// �N���X��
		strncpy(data.cClassType, pObject->GetObjClassName().c_str(), sizeof(data.cClassType) - 1);
		data.cClassType[sizeof(data.cClassType) - 1] = '\0';

		// �ʒu�A��]�A�g��
		ComponentTransform* pTransform = pObject->GetComponent<ComponentTransform>();
		data.vPos	= pTransform->GetPosition();
		data.qRot	= pTransform->GetRotation();
		data.vScale = pTransform->GetScale();

		// �I�u�W�F�N�g��
		strncpy(data.cObjectName, pObject->GetName().c_str(), sizeof(data.cObjectName) - 1);
		data.cObjectName[sizeof(data.cObjectName) - 1] = '\0';

		// �e�I�u�W�F�N�g��
		ObjectBase* pParent = pObject->GetParentObject();
		if (pParent)
		{
			strncpy(data.cParentName, pParent->GetName().c_str(), sizeof(data.cParentName) - 1);
			data.cParentName[sizeof(data.cParentName) - 1] = '\0';
		}
		else
		{
			data.cParentName[0] = '\0';	// �e�I�u�W�F�N�g�����Ȃ��ꍇ�͋󕶎�
		}

		// �t�@�C���ɏ�������
		file.write((char*)&data, sizeof(S_SaveDataObject));

		// �ʒu�A��]�A�g��݂̂̏o�͂̏ꍇ�̓X�L�b�v
		if (bTransOnly) continue;	
		// �I�u�W�F�N�g�ʂ̃f�[�^�o��
		pObject->OutPutLocalData(file);
	}

	// ���b�Z�[�W�\��(����)
	WIN_DATA_INOUT["OutputResult"].SetText("< Success > " + sPath.substr(sPath.find_last_of('/') + 1));

	file.close();
}

/* ========================================
	�t�@�C������(�V�[���I�u�W�F�N�g)�֐�
	-------------------------------------
	���e�F�t�@�C������I�u�W�F�N�g�̏���ǂݍ���
	-------------------------------------
	����1�F�t�@�C���p�X
========================================== */
void FileManager::StageObjectInput(std::string sPath)
{
	// �e�I�u�W�F�N�g�̃}�b�v
	std::unordered_map<ObjectBase*, std::string> mapObjectParent;

	// �t�@�C�����J��
	std::ifstream file(sPath, std::ios::in | std::ios::binary);

	// �t�@�C�����J���Ȃ�������I��
	if (!file.is_open())
	{
		// ���b�Z�[�W�\��(���s)
		WIN_DATA_INOUT["InputResult "].SetText("< Failed > " + sPath.substr(sPath.find_last_of('/') + 1));
		return;
	}

	SceneBase* pScene = SceneManager::GetScene();	// �V�[���擾

	// �f�[�^���o�͎��̃I�v�V����
	bool bCamera	= WIN_DATA_INOUT["Camera"].GetBool();			// �J�����̏o�͐ݒ�
	bool bLight		= WIN_DATA_INOUT["Light"].GetBool();			// ���C�g�̏o�͐ݒ�
	bool bTerrain	= WIN_DATA_INOUT["Terrain"].GetBool();			// �n�`�̏o�͐ݒ�
	bool bEnemy		= WIN_DATA_INOUT["Enemy"].GetBool();			// �G�L�����̏o�͐ݒ�
	bool bTransOnly = WIN_DATA_INOUT["TransformOnly"].GetBool();	// �ʒu�A��]�A�g��݂̂̏o�͐ݒ�

	// �t�@�C���̏I�[�܂œǂݍ���
	while (!file.eof())
	{
		S_SaveDataObject data;
		file.read((char*)&data, sizeof(S_SaveDataObject));

		// �t�@�C���̏I�[�ɓ��B������I��
		if (file.eof())
		{
			break;
		}

		// �I�u�W�F�N�g�̐���(�n�����N���X�����琶��)
		ObjectBase* pObject = OBJ_TYPE_REGISTRY_INST.CreateObject(data.cClassType);

		if (pObject)
		{
			pObject->Init(pScene->CreateUniqueName(data.cObjectName));	// �I�u�W�F�N�g������(���O�d��������)

			// ���O�`�F�b�N
			if (pObject->GetTag() == E_ObjectTag::Camera && !bCamera) continue;
			if (pObject->GetTag() == E_ObjectTag::Light && !bLight) continue;
			if (pObject->GetTag() == E_ObjectTag::Ground && !bTerrain) continue;
			if (pObject->GetTag() == E_ObjectTag::Goal && !bTerrain) continue;
			if (pObject->GetTag() == E_ObjectTag::Wall && !bTerrain) continue;
			if (pObject->GetTag() == E_ObjectTag::Enemy && !bEnemy) continue;

			// �ʒu�A��]�A�g��̐ݒ�
			ComponentTransform* pTransform = pObject->GetComponent<ComponentTransform>();
			pTransform->SetLocalPosition(data.vPos);
			pTransform->SetLocalRotation(data.qRot);
			pTransform->SetLocalScale(data.vScale);

			// �V�[���ɒǉ�
			pScene->AddSceneObjectBase(pObject);
			// �e�I�u�W�F�N�g�}�b�v�ɒǉ�
			mapObjectParent[pObject] = data.cParentName;

			if (bTransOnly) continue;	// �ʒu�A��]�A�g��݂̂̏o�͂̏ꍇ�̓X�L�b�v
			// �I�u�W�F�N�g�ʂ̃f�[�^����
			pObject->InputLocalData(file);
		}
 	}

	// �e�q�֌W�̐ݒ�
	for (auto& object : mapObjectParent)
	{
		std::string sParentName = object.second;
		// �e�I�u�W�F�N�g�����Ȃ��ꍇ�̓X�L�b�v
		if (sParentName.empty()) continue;
		// �e�I�u�W�F�N�g�擾
		ObjectBase* pParent = pScene->FindSceneObject(sParentName);

		// �e�I�u�W�F�N�g�����݂��Ă�����e�q�֌W��ݒ�
		if (pParent)
		{
			object.first->SetParentObject(pParent);
		}
	}

	// ���b�Z�[�W�\��(����)
	WIN_DATA_INOUT["InputResult "].SetText("< Success > " + sPath.substr(sPath.find_last_of('/') + 1));

	file.close();

}
