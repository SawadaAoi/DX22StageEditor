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

#include "ComponentTransform.h"
#include "ObjectTypeRegistry.h"

#include <Windows.h>	// ���b�Z�[�W�{�b�N�X�p


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

	// �V�[���ɑ��݂���I�u�W�F�N�g���擾
	for (auto& object : SceneManager::GetScene()->GetAllSceneObjects())
	{
		S_SaveDataObject data;

		// �I�u�W�F�N�g�̏����擾 ---------------------------------
		// �N���X��
		strncpy(data.cClassType, object->GetObjClassName().c_str(), sizeof(data.cClassType) - 1);
		data.cClassType[sizeof(data.cClassType) - 1] = '\0';

		// �ʒu�A��]�A�g��
		ComponentTransform* pTransform = object->GetComponent<ComponentTransform>();
		data.vPos = pTransform->GetLocalPosition();
		data.qRot = pTransform->GetLocalRotation();
		data.vScale = pTransform->GetLocalScale();

		// �I�u�W�F�N�g��
		strncpy(data.cObjectName, object->GetName().c_str(), sizeof(data.cObjectName) - 1);
		data.cObjectName[sizeof(data.cObjectName) - 1] = '\0';

		// �e�I�u�W�F�N�g��
		ObjectBase* pParent = object->GetParentObject();
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
			
			// �ʒu�A��]�A�g��̐ݒ�
			ComponentTransform* pTransform = pObject->GetComponent<ComponentTransform>();
			pTransform->SetLocalPosition(data.vPos);
			pTransform->SetLocalRotation(data.qRot);
			pTransform->SetLocalScale(data.vScale);

			// �V�[���ɒǉ�
			pScene->AddSceneObjectBase(pObject);
		}
 	}

	// �e�q�֌W�̐ݒ� -----------------------------------------------

	// �t�@�C���̈ʒu��擪�ɖ߂�
	// �t�@�C���̃I�u�W�F�N�g��S�ēo�^���Ă���e�q�֌W��ݒ肷�邽��
	file.clear();
	file.seekg(0, std::ios::beg);

	// �t�@�C���̏I�[�܂œǂݍ���
	while (!file.eof())
	{
		S_SaveDataObject data;
		file.read((char*)&data, sizeof(S_SaveDataObject));

		if (file.eof())
		{
			break;
		}

		// �e�q�֌W�̐ݒ�
		std::string sObjectName = data.cObjectName;
		std::string sParentName = data.cParentName;

		// �e�I�u�W�F�N�g�����Ȃ��ꍇ�̓X�L�b�v
		if (sParentName.empty())	continue;

		// �I�u�W�F�N�g�Ɛe�I�u�W�F�N�g���擾
		ObjectBase* pObject = pScene->FindSceneObject(sObjectName);
		ObjectBase* pParent = pScene->FindSceneObject(sParentName);

		// �ǂ�������݂��Ă�����e�q�֌W��ݒ�
		if (pObject && pParent)
		{
			pObject->SetParentObject(pParent);
		}
	}

	// ���b�Z�[�W�\��(����)
	WIN_DATA_INOUT["InputResult "].SetText("< Success > " + sPath.substr(sPath.find_last_of('/') + 1));

	file.close();

}
