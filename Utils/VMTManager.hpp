#pragma once

typedef void* GenericFunction;
typedef GenericFunction* VMT;
class VMTManager
{
	static size_t CountFunctionsInVMT(VMT vmt)
	{
		size_t output = 0;

		// while the function at this index is a valid function
		while (!IsBadCodePtr((FARPROC)vmt[output]))
			++output;

		return output;
	}

private:
	size_t FunctionCount = 0;
	VMT OriginalVMT;
	VMT CurrentVMT;

public:
	VMTManager(VMT* pVMT)
	{
		this->CurrentVMT = *pVMT;
		this->FunctionCount = CountFunctionsInVMT(this->CurrentVMT);

		// create a copy
		this->OriginalVMT = new GenericFunction[this->FunctionCount];
		for (size_t i = 0; i < this->FunctionCount; i++)
			this->OriginalVMT[i] = this->CurrentVMT[i];
	}

	GenericFunction Hook(int Index, GenericFunction NewFunction)
	{
		this->CurrentVMT[Index] = NewFunction;
		return this->OriginalVMT[Index];
	}

	void Unhook(int Index)
	{
		this->CurrentVMT[Index] = this->OriginalVMT[Index];
	}

	void UnhookAll()
	{
		for (size_t index = 0; index < this->FunctionCount; index++)
			this->Unhook(index);
	}
	
	~VMTManager()
	{
		this->UnhookAll();
		delete[] this->OriginalVMT;
	}
};