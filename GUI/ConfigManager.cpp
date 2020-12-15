#include "ConfigManager.hpp"

std::string ConfigManager::GetFile()
{
	char filename[MAX_PATH];
	ZeroMemory(&filename, MAX_PATH);

	OPENFILENAME ofn{};
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select Your Config";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn)) return "";
	return std::string(filename);
}