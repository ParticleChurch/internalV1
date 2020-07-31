#pragma once
class Panel
{
public:
	const char* GetName(int panelID)
	{
		typedef const char* (__thiscall* oPanel) (void*, int);

		return GetVFunc<oPanel>(this, 36)(this, panelID);
	}
};