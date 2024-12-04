/* ========================================
	DX22Base/
	------------------------------------
	�f�o�b�O���j���[�pcpp
	------------------------------------
	DebugMenu.cpp
========================================== */
#ifdef _DEBUG


// =============== �C���N���[�h ===================
#include "DebugMenu.h"

// imgui
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>

#include "Input.h"
#include "DirectXManager.h"

// =============== �萔��` =======================
const char* SAVE_DATA_PATH = "Assets/DebugMenu.csv";


// =============== ���O��� ===================
namespace DebugUI {



/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
=========================================== */
Menu::Menu()
{
}

/* ========================================
	�f�X�g���N�^�֐�
	-------------------------------------
	���e�F�I������
=========================================== */
Menu::~Menu()
{
}


/* ========================================
	�������֐�
	-------------------------------------
	���e�F������
=========================================== */
void Menu::Init()
{
	// ImGui������
	IMGUI_CHECKVERSION();	// ImGui���C�u�����ƃA�v���P�[�V�����R�[�h�̊ԂŌ݊��������邩�m�F
	ImGui::CreateContext();	// ImGui�R���e�L�X�g�̍쐬(�E�B�W�F�b�g�̏�ԁA�X�^�C���A�f�[�^��ێ�)

	
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// �L�[�{�[�h����
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// �Q�[���p�b�h����
	ImGui::StyleColorsDark();				// �����ڂ̐ݒ�

	ImGui_ImplWin32_Init(GetActiveWindow());
	ImGui_ImplDX11_Init(DirectXManager::GetDevice(), DirectXManager::GetContext());

	// Dummy�f�[�^������
	Item::dummy = Item::CreateValue("None", Item::Label);
	
	// �����E�B���h�E�̍쐬
	Window window;
	window.name = WIN_NAME[0];
	m_WindowList.push_back(window);

	// �t�@�C���ǂݏo��
	FILE* fp;
	fopen_s(&fp, SAVE_DATA_PATH, "rt");
	if (fp) {
		const int size = 1024;	// 1�s�̍ő啶����
		char line[size];
		m_SaveDataList.clear();

		// 1�s���ǂݍ���
		while (fgets(line, size, fp) != NULL) {
			// �Z�����e�擾
			std::vector<std::string> cells;
			*strstr(line, "\n") = ',';		// ���s�������J���}�ɕϊ�

			char* start = line;
			char* end = strstr(start, ",");	

			// �J���}�ŋ�؂�ꂽ�Z�����擾
			do {
				*end = '\0';				
				cells.push_back(start);	
				start = end + 1;
				end = strstr(start, ",");
			} while (end);

			// �f�[�^�ǉ�
			SaveData data;
			data.kind = Item::StrToKind(cells[0]);
			data.path = cells[1];
			data.value = cells[2];
			m_SaveDataList.push_back(data);
		}
		fclose(fp);
	}
}

/* ========================================
	�I�������֐�
	-------------------------------------
	���e�F�I������
=========================================== */
void Menu::Uninit()
{
	// �_�~�[�̍폜
	delete Item::dummy;

	// �ۑ��f�[�^�̍쐬
	std::string data;

	auto it = m_WindowList.begin();
	while (it != m_WindowList.end())
	{
		// �E�B���h�E��O���[�v���K�w�ƂȂ�悤�Ƀp�X���쐬
		std::string path = it->name + "/";

		// �������݂��K�v�ȃf�[�^��ۑ�
		auto itemIt = it->items.begin();
		while (itemIt != it->items.end())
		{
			DataWrite(data, path, *itemIt);
			++itemIt;
		}

		// �E�B���h�E�̃N���A
		it->Clear();
		++it;
	}

	// �ۑ�����
	FILE* fp;
	fopen_s(&fp, SAVE_DATA_PATH, "wt");
	if (fp) {
		fwrite(data.c_str(), data.size(), 1, fp);
		fclose(fp);
	}

	// ImGui�I������
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


/* ========================================
	�X�V�����֐�
	-------------------------------------
	���e�F�X�V
=========================================== */
void Menu::Update()
{
	auto& mainWindow = m_WindowList[0];	// ���C���E�B���h�E

	// �f�o�b�O���j���[�̕\���؂�ւ�
	if (Input::IsKeyPress(VK_SHIFT) && Input::IsKeyPress(VK_SPACE))
	{
		if (Input::IsKeyTrigger(VK_RETURN))
		{
			//mainWindow.enable ^= true;
			for (int i = 1; i < m_WindowList.size(); i++)
			{
				m_WindowList[i].enable ^= true;
			}
		}

			
	}

	// Menu�̊e�E�B���h�E�̕\���؂�ւ�
	//int cnt = 1;						// 0�͎��g�̃E�B���h�E
	//auto it = mainWindow.items.begin();	
	//while (it != mainWindow.items.end())
	//{
	//	m_WindowList[cnt].enable = (*it)->GetBool();
	//	++it;
	//	++cnt;
	//}

}


/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`��
=========================================== */
void Menu::Draw()
{
	// ���C���E�B�h�E���L���ɂȂ��Ă��Ȃ���Δ�\��
	/*if (!m_WindowList[0].enable)
		return;*/

	// Imgui�̕`�揀��
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// �E�B���h�E���̕\�����ڂ�`��
	auto windowIt = m_WindowList.begin();
	while (windowIt != m_WindowList.end())
	{
		// �E�B���h�E���L���ł���Ε`��
		if (windowIt->enable)
		{
			// �E�B���h�E���̕`��J�n-------------------------------------
			//ImGui::Begin(windowIt->name.c_str());
			ImGui::Begin(windowIt->name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);	// �傫���A�ʒu�Œ�
			// �A�C�e�����Ƃɕ`��
			auto itemIt = windowIt->items.begin();
			while (itemIt != windowIt->items.end())
			{
				DrawImgui(*itemIt);
				++itemIt;
			}

			ImGui::End();
			// �E�B���h�E���̕`��I��-------------------------------------

		}
		++windowIt;
	}

	// ImGui�̕\��
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


/* ========================================
	�E�B���h�E�����֐�
	-------------------------------------
	���e�F�E�B���h�E����
	-------------------------------------
	�����Fconst char* name		�E�B���h�E��
=========================================== */
Window& Menu::Create(const char* name)
{		
	// �ʃE�B���h�E�ݒ�
	Window window;
	window.name		= name;
	window.enable	= false;
	window.items.clear();
	m_WindowList.push_back(window);

	// �����E�B���h�E�ɒǉ�
	Item* label = Item::CreateValue(name, Item::Bool, true);
	DataRead(m_WindowList[0].name + "/", label);			// �ۑ��f�[�^�̓ǂݍ���
	m_WindowList[0].items.push_back(label);					// ���C���E�B���h�E�ɒǉ�

	return m_WindowList.back();
}

/* ========================================
	�E�B���h�E�擾�֐�
	-------------------------------------
	���e�F�E�B���h�E�擾
	-------------------------------------
	�����Fconst char* name		�E�B���h�E��
=========================================== */
Window& Menu::Get(const char* name)
{
	// �E�B���h�E�̒T��
	auto winIt = std::find_if(m_WindowList.begin(), m_WindowList.end(),
		[&name](const Window& obj) {
			return strcmp(obj.name.c_str(), name) == 0;
		});

	// ������Ȃ���΃_�~�[��ԋp
	if (winIt == m_WindowList.end())
		return Window::dummy;

	// ���������E�B���h�E��ԋp
	return *winIt;
}

/* ========================================
	�e���ڕ`��֐�
	-------------------------------------
	���e�F�E�B���h�E���̊e����
		�@(�`�F�b�N�{�b�N�X�A�{�^���Ȃ�)�̕`��
	-------------------------------------
	�����FItem* item		�\���A�C�e��
=========================================== */
void Menu::DrawImgui(Item* item)
{
	// �e�v�f�̎擾
	bool isValue	= (typeid(ItemValue) == typeid(*item));		// �l����
	bool isBind		= (typeid(ItemBind) == typeid(*item));		// �ϐ��R�t������
	bool isCallback = (typeid(ItemCallback) == typeid(*item));	// �R�[���o�b�N����
	ItemValue*		pValue		= static_cast<ItemValue*>(item);
	ItemBind*		pBind		= static_cast<ItemBind*>(item);
	ItemCallback*	pCallback	= static_cast<ItemCallback*>(item);

	// ��O�̍��ڂƉ����тɕ\��
	if (item->m_bSameLine)	ImGui::SameLine();

	// ���ڂ̎��
	switch (item->GetKind())
	{
	// ���ږ��݂̂̕\��
	case Item::Label:
		ImGui::Text("%s", item->GetName());
		break;

	case Item::Text:
		if (isValue)
		{
			ImGui::Text("%s : %s", item->GetName(), &pValue->m_uValue.cText);
		}
		else if (isBind)
		{
			ImGui::Text("%s : %s", item->GetName(), reinterpret_cast<std::string* > (pBind->m_pBindVar)->c_str());
		}
		break;
	// �`�F�b�N�t���O�̕\��
	case Item::Bool:
		if (isValue)
		{
			ImGui::Checkbox(item->GetName(), &pValue->m_uValue.bValid);
		}
		else if (isBind)
		{
			bool* bValidPtr = reinterpret_cast<bool*>(pBind->m_pBindVar);	// �R�Â��ϐ��̃|�C���^
			ImGui::Checkbox(item->GetName(), bValidPtr);					// �`�F�b�N�{�b�N�X�̕\��
			item->m_uValue.bValid = *bValidPtr;								// �ۑ��p�̒l���X�V
		}
		else if (isCallback) 
		{
			pCallback->m_CallbackFunc(false, &pCallback->m_uValue.bValid);
			if (ImGui::Checkbox(item->GetName(), &pCallback->m_uValue.bValid))
				pCallback->m_CallbackFunc(true, &pCallback->m_uValue.bValid);
		}
		break;
	// �������ڂ̕\��
	case Item::Int:
		if (isValue)
		{
			ImGui::InputInt(item->GetName(), &pValue->m_uValue.nValue);
		}
		else if (isBind)
		{
			int* nValuePtr = reinterpret_cast<int*>(pBind->m_pBindVar);
			ImGui::InputInt(item->GetName(), nValuePtr);
			item->m_uValue.nValue = *nValuePtr;
		}
		else if (isCallback) 
		{
			pCallback->m_CallbackFunc(false, &pCallback->m_uValue.nValue);
			if (ImGui::InputInt(item->GetName(), &pCallback->m_uValue.nValue))
				pCallback->m_CallbackFunc(true, &pCallback->m_uValue.nValue);
		}
		break;
	// �������ڂ̕\��
	case Item::Float:
		if (isValue)
		{
			ImGui::InputFloat(item->GetName(), &pValue->m_uValue.fValue);
		}
		else if (isBind)
		{
			float* fValuePtr = reinterpret_cast<float*>(pBind->m_pBindVar);
			ImGui::InputFloat(item->GetName(), fValuePtr);
			item->m_uValue.fValue = *fValuePtr;
		}
		else if (isCallback) 
		{
			pCallback->m_CallbackFunc(false, &pCallback->m_uValue.fValue);
			if (ImGui::InputFloat(item->GetName(), &pCallback->m_uValue.fValue))
				pCallback->m_CallbackFunc(true, &pCallback->m_uValue.fValue);
		}
		break;
	// �x�N�g�����ڂ̕\��
	case Item::Vector:
		if (isValue)
		{
			ImGui::InputFloat3(item->GetName(), &pValue->m_uValue.fVector.x, "%.4f");
		}
		else if (isBind)
		{
			ImGui::InputFloat3(item->GetName(), reinterpret_cast<float*>(pBind->m_pBindVar), "%.4f");
			item->m_uValue.fVector = DirectX::XMFLOAT3(
				reinterpret_cast<float*>(pBind->m_pBindVar)[0],
				reinterpret_cast<float*>(pBind->m_pBindVar)[1],
				reinterpret_cast<float*>(pBind->m_pBindVar)[2]
			);
		}
		else if (isCallback) 
		{
			pCallback->m_CallbackFunc(false, &pCallback->m_uValue.fVector.x);
			if (ImGui::InputFloat3(item->GetName(), &pCallback->m_uValue.fVector.x, "%.4f"))
				pCallback->m_CallbackFunc(true, &pCallback->m_uValue.fVector.x);
		}
		break;

	// �x�N�g��2D���ڂ̕\��
	case Item::Vector2:
		if (isValue)
		{
			ImGui::InputFloat2(item->GetName(), &pValue->m_uValue.fVector2.x, "%.4f");
		}
		else if (isBind)
		{
			ImGui::InputFloat2(item->GetName(), reinterpret_cast<float*>(pBind->m_pBindVar), "%.4f");
			item->m_uValue.fVector2 = DirectX::XMFLOAT2(
				reinterpret_cast<float*>(pBind->m_pBindVar)[0],
				reinterpret_cast<float*>(pBind->m_pBindVar)[1]
			);
		}
		else if (isCallback)
		{
			pCallback->m_CallbackFunc(false, &pCallback->m_uValue.fVector2.x);
			if (ImGui::InputFloat2(item->GetName(), &pCallback->m_uValue.fVector2.x, "%.4f"))
				pCallback->m_CallbackFunc(true, &pCallback->m_uValue.fVector2.x);
		}
		break;

	// �F���ڂ̕\��
	case Item::Color:
		if (isValue)
		{
			ImGui::ColorEdit4(item->GetName(), &pValue->m_uValue.fColor.x);
		}
		else if (isBind)
		{
			ImGui::ColorEdit4(item->GetName(), reinterpret_cast<float*>(pBind->m_pBindVar));
			item->m_uValue.fColor = DirectX::XMFLOAT4(
				reinterpret_cast<float*>(pBind->m_pBindVar)[0],
				reinterpret_cast<float*>(pBind->m_pBindVar)[1],
				reinterpret_cast<float*>(pBind->m_pBindVar)[2],
				reinterpret_cast<float*>(pBind->m_pBindVar)[3]
			);
		}
		else if (isCallback) 
		{
			pCallback->m_CallbackFunc(false, &pCallback->m_uValue.fColor.x);
			if(ImGui::ColorEdit4(item->GetName(), &pCallback->m_uValue.fColor.x))
				pCallback->m_CallbackFunc(true, &pCallback->m_uValue.fColor.x);
		}
		break;
	// �p�X���ڂ̕\��
	case Item::Path:
		if (isValue) // �ʏ�\��
		{
			ImGui::InputText(item->GetName(), static_cast<ItemValue*>(item)->m_uValue.cPath, MAX_PATH);
		}
		else if (isBind) // �R�Â����ڂ̕\��
		{
			ImGui::InputText(item->GetName(), reinterpret_cast<char*>(static_cast<ItemBind*>(item)->m_pBindVar), MAX_PATH);
		}
		break;
	// �{�^���̕\��
	case Item::Kind::Command:
		// �{�^���������ꂽ��R�[���o�b�N�֐������s
		if (ImGui::Button(item->GetName()))
		{
			pCallback->m_CallbackFunc(false, nullptr);
		}
		break;
	// �O���[�v���ڂ̕\��
	case Item::Group:
		{
			ItemGroup* ptr = reinterpret_cast<ItemGroup*>(item);
			// �\�����ڂ��Ȃ���Ε\�����Ȃ�
			if (ptr->m_pItemGroup.empty()) break;

			// �O���[�v���W�J����ĂȂ���Ε\�����Ȃ�
			if (!ImGui::CollapsingHeader(item->GetName(), ImGuiTreeNodeFlags_DefaultOpen)) break;

			// �O���[�v���̍��ڂ��ċA�ŕ\��
			auto it = ptr->m_pItemGroup.begin();
			while (it != ptr->m_pItemGroup.end())
			{
				DrawImgui(*it);
				++it;
			}
		}
		break;
	// ���X�g���ڂ̕\��
	case Item::List:
		{
			ItemList* ptr = reinterpret_cast<ItemList*>(item);
			// �\�����ڂ��Ȃ���Ε\�����Ȃ�
			if (ptr->m_sItemList.empty()) break;

			// �\�����ڂ̍\�z
			static const char* pList[100];
			auto it = ptr->m_sItemList.begin();
			for (int i = 0; i < ptr->m_sItemList.size() && i < 100; ++i, ++it)
				pList[i] = it->c_str();

			// ���X�g�{�b�N�X
			if (!ptr->m_bIsDropDown)
			{
				// �\��
				if (ImGui::ListBox(item->GetName(), &ptr->m_uValue.nValue, pList, static_cast<int>(ptr->m_sItemList.size())))
				{
					// �֐�������ΑI�������������s
					if (ptr->m_CallbackFunc && ptr->m_uValue.nValue >= 0)
						ptr->m_CallbackFunc(pList[ptr->m_uValue.nValue]);
				}
			}
			// �h���b�v�_�E�����X�g
			else
			{
				// �\��
				if (ImGui::Combo(item->GetName(), &ptr->m_uValue.nValue, pList, static_cast<int>(ptr->m_sItemList.size())))
				{
					// �֐�������ΑI�������������s
					if (ptr->m_CallbackFunc && ptr->m_uValue.nValue >= 0)
						ptr->m_CallbackFunc(pList[ptr->m_uValue.nValue]);
				}
			}
		}
		break;
	}
}



/* ========================================
	�f�[�^�����֐�
	-------------------------------------
	���e�F�f�[�^�̏�������
	-------------------------------------
	����1�Fstring& data	�������݃f�[�^
	����2�Fstring path	�p�X
	����3�FItem* item	�A�C�e��
=========================================== */
void Menu::DataWrite(std::string& data, std::string path, Item* item)
{
	// �O���[�v�A�C�e���ł���΁A�p�X���X�V���čċA�Ăяo��
	if (item->GetKind() == Item::Group)
	{
		ItemGroup* groupItem= static_cast<ItemGroup*>(item);

		// �p�X�̍X�V
		path = path + groupItem->GetName() + "/";

		// �O���[�v���A�C�e���̃f�[�^��������
		auto it = groupItem->m_pItemGroup.begin();
		while (it != groupItem->m_pItemGroup.end())
		{
			DataWrite(data, path, *it);
			++it;
		}

		// �ȍ~�̏����͍s��Ȃ�
		return;
	}


	// �Z�[�u���Ȃ��ꍇ�͈ȉ��̏������s��Ȃ�
	if (!item->m_bSave)	return;

	// ���ڂ̏�������
	data += Item::KindToStr(item->GetKind()) + ",";
	// �\�����̏�������
	data += path + item->GetName() + ",";
	// ���ڕʂ̏�������
	switch (item->GetKind())
	{
	default:
		data += "0";
	case Item::Bool:
		data += item->m_uValue.bValid ? "1" : "0";
		break;
	case Item::Int:
		data += std::to_string(item->m_uValue.nValue);
		break;
	case Item::Float:
		data += std::to_string(item->m_uValue.fValue);
		break;
	case Item::Color:
		data += std::to_string(item->m_uValue.fColor.x) + "/";
		data += std::to_string(item->m_uValue.fColor.y) + "/";
		data += std::to_string(item->m_uValue.fColor.z) + "/";
		data += std::to_string(item->m_uValue.fColor.w);
		break;
	case Item::List:
		data += std::to_string(item->m_uValue.nValue);
		break;
	}
	data += "\n";
}

/* ========================================
	�f�[�^�Ǎ��֐�
	-------------------------------------
	���e�F�f�[�^�̓ǂݍ���
	-------------------------------------
	����1�Fstring path	�p�X
	����2�FItem* item	�A�C�e��
=========================================== */
void Menu::DataRead(std::string path, Item* item)
{
	// �O���[�v�A�C�e���ł���΁A�p�X���X�V���čċA�Ăяo��
	if (item->GetKind() == Item::Group)
	{
		ItemGroup* groupItem = static_cast<ItemGroup*>(item);
		// �p�X�̍X�V
		path = path + groupItem->GetName() + "/";

		// �O���[�v���A�C�e���̃f�[�^�ǂݎ��
		auto it = groupItem->m_pItemGroup.begin();
		while (it != groupItem->m_pItemGroup.end())
		{
			DataRead(path, *it);
			++it;
		}

		// �ȍ~�̏����͍s��Ȃ�
		return;
	}

	// �Z�[�u���Ȃ��ꍇ�͈ȉ��̏������s��Ȃ�
	if (!item->m_bSave)	return;

	// �p�X�ƍ��ږ�����ǂݎ��f�[�^�����쐬
	path += item->GetName();

	// �f�[�^���ɑΉ�����f�[�^��T��
	auto dataIt = std::find_if(m_SaveDataList.begin(), m_SaveDataList.end(),
		[&path](const SaveData& data) {
			return data.path == path;
		});

	// �Ȃ���΃f�[�^�̏㏑�����s��Ȃ�
	if (dataIt == m_SaveDataList.end()) return;

	// ���ڂɉ����ăf�[�^�̓ǂݎ��
	switch (item->GetKind())
	{
	case Item::Bool:
		item->m_uValue.bValid = atoi(dataIt->value.c_str()) > 0;
		break;
	case Item::Int:
		item->m_uValue.nValue = atoi(dataIt->value.c_str());
		break;
	case Item::Float:
		item->m_uValue.fValue = strtof(dataIt->value.c_str(), nullptr);
		break;
	case Item::Color: {
		const char* top[] = {
			dataIt->value.c_str(),
			strstr(top[0], "/") + 1,
			strstr(top[1], "/") + 1,
			strstr(top[2], "/") + 1,
		};
		item->m_uValue.fColor = DirectX::XMFLOAT4(
			strtof(top[0], nullptr),
			strtof(top[1], nullptr),
			strtof(top[2], nullptr),
			strtof(top[3], nullptr)
		);
		} break;
	case Item::List:
		ItemList* pList = static_cast<ItemList*>(item);
		item->m_uValue.nValue = atoi(dataIt->value.c_str());
		if (pList->m_CallbackFunc) {
			auto it = pList->m_sItemList.begin();
			for (int i = 0; i < pList->m_uValue.nValue; ++i)
				it++;
			pList->m_CallbackFunc(it->c_str());
		}
		break;
	}
}


} // namespace debug
#endif // _DEBUG