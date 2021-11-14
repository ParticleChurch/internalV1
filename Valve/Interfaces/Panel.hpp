#pragma once
class Panel
{
public:
	DECLARE_VIRTUAL_METHOD(const char*, GetName, 36, (int panelID), (panelID));
};