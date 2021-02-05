#include "ConfigManager.hpp"
#define MULTILINE(...) #__VA_ARGS__

std::vector<ConfigManager::ConfigData> ConfigManager::Configs = {
	ConfigManager::ConfigData{
		false,
		"Everything Disabled",
		ConfigManager::ConfigAllDisabled
	},
	ConfigManager::ConfigData{
		false,
		"Default Legit",
		ConfigManager::ConfigDefaultLegit
	},
	ConfigManager::ConfigData{
		false,
		"Default Rage",
		ConfigManager::ConfigDefualtRage
	},
};

std::string ConfigManager::GetOpenFile()
{
	char filename[MAX_PATH];
	ZeroMemory(&filename, MAX_PATH);

	OPENFILENAME ofn{};
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Text Files (.cfg, .txt, .church)\0*.cfg;*.txt;*.church\0All Files\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Open Config";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_NODEREFERENCELINKS | OFN_EXPLORER;

	if (!GetOpenFileName(&ofn)) return "";
	return std::string(filename, filename + MAX_PATH);
}

std::string ConfigManager::GetSaveFile()
{
	char filename[MAX_PATH];
	ZeroMemory(&filename, MAX_PATH);
	strcpy(filename, "a4g4.com");

	OPENFILENAME ofn{};
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Text Files (.cfg, .txt, .church)\0*.cfg;*.txt;*.church\0All Files\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Save Config";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_NODEREFERENCELINKS | OFN_EXPLORER | OFN_OVERWRITEPROMPT;

	if (!GetSaveFileName(&ofn)) return "";
	return std::string(filename, filename + MAX_PATH);
	return GetOpenFile();
}

DWORD WINAPI ConfigManager::ImportThreaded(void* _)
{
	std::string filename = GetOpenFile();
	if (filename == "") return 1;

	std::ifstream file(filename.c_str());
	if (!file.is_open()) return 2;
	file.seekg(0, std::ios::end);
	std::streampos bytes = file.tellg();
	if (bytes > 100000) return 3; // too large
	file.seekg(0);
	file.clear();

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string str = buffer.str();

	std::string baseName = PathFindFileName(filename.c_str());

	// attempt to remove extension
	{
		size_t last = baseName.find_last_of('.');
		if (last > 0 && last != std::string::npos)
		{
			baseName = baseName.substr(0, last);
		}
	}

	ConfigData cdata{
		true,
		baseName,
		str
	};
	Configs.push_back(cdata);

	L::Log(("Imported " + std::to_string(str.size()) + " bytes from : " + filename).c_str());
	return 0;
}

DWORD WINAPI ConfigManager::ExportThreaded(void* _)
{
	// convert all properties to string
	std::string s = Config::ExportToString();
	std::string filename = GetSaveFile();
	if (filename == "") return 1;

	std::fstream file;
	file.open(filename.c_str(), std::fstream::out | std::fstream::trunc);
	if (!file.is_open()) return 2;

	file.write(s.c_str(), s.size());

	file.close();

	L::Log(("Exported " + std::to_string(s.size()) + " bytes to : " + filename).c_str());
	return 0;
}

void ConfigManager::Import()
{
	HANDLE t = CreateThread(NULL, 0, ImportThreaded, NULL, NULL, NULL);
	if (t) CloseHandle(t);
}

void ConfigManager::Export()
{
	HANDLE t = CreateThread(NULL, 0, ExportThreaded, NULL, NULL, NULL);
	if (t) CloseHandle(t);
}

bool ConfigManager::Remove(size_t i)
{
	if (i >= Configs.size()) return false;
	Configs.erase(Configs.begin() + i);
	return true;
}

void ConfigManager::Load(ConfigManager::ConfigData cdata)
{
	// clean first
	if (Configs.size() > 0)
		Config::LoadFromString(Configs[0].Value);
	Config::ClearAllKeybinds();

	size_t nProperties = Config::LoadFromString(cdata.Value);
	L::Log(("Loaded " + std::to_string(nProperties) + " properties").c_str());
	// todo notify the user
}