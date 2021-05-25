#pragma once

class CHudElement {
public:
	bool IsActive()
	{
		return *(bool*)((DWORD)this + 13);
	}
};