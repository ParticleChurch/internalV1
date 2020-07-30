#pragma once
class VMTManager
{
private:
	DWORD* CustomTable;
	bool	InitComplete;
	DWORD* OriginalTable;
	DWORD* Instance;

	int		MethodCount(DWORD* InstancePointer) 
	{
		DWORD* VMT = (DWORD*)*InstancePointer;
		int Index = 0;
		int Amount = 0;
		while (!IsBadCodePtr((FARPROC)VMT[Index]))
		{
			if (!IsBadCodePtr((FARPROC)VMT[Index]))
			{
				Amount++;
				Index++;
			}
		}

		return Amount;
	}

public:
	bool	Initialise(DWORD* InstancePointer) // Pass a &class
	{
		// Store the instance pointers and such, and work out how big the table is
		Instance = InstancePointer;
		OriginalTable = (DWORD*)*InstancePointer;
		int VMTSize = MethodCount(InstancePointer);
		size_t TableBytes = VMTSize * 4;

		// Allocate some memory and copy the table
		CustomTable = (DWORD*)malloc(TableBytes + 8);
		if (!CustomTable) return false;
		memcpy((void*)CustomTable, (void*)OriginalTable, VMTSize * 4);

		// Change the pointer
		*InstancePointer = (DWORD)CustomTable;

		InitComplete = true;
		return true;
	}

	DWORD	HookMethod(DWORD NewFunction, int Index)
	{
		if (InitComplete)
		{
			CustomTable[Index] = NewFunction;
			return OriginalTable[Index];
		}
		else
			return NULL;
	}
	void	UnhookMethod(int Index)
	{
		if (InitComplete)
			CustomTable[Index] = OriginalTable[Index];
		return;
	}

	void	RestoreOriginal()
	{
		if (InitComplete)
		{
			*Instance = (DWORD)OriginalTable;
		}
		return;
	}
	void	RestoreCustom()
	{
		if (InitComplete)
		{
			*Instance = (DWORD)CustomTable;
		}
		return;
	}

	template<typename T>
	T GetMethod(size_t nIndex)
	{
		return (T)OriginalTable[nIndex];
	}

	DWORD	GetOriginalFunction(int Index)
	{
		return OriginalTable[Index];
	}
};