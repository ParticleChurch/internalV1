#pragma once

typedef void* GenericFunction;
typedef GenericFunction* VMT;
class VMTManager
{
private:
	static size_t CountFunctionsInVMT(VMT vmt)
	{
		size_t output = 0;
		while (!IsBadCodePtr((FARPROC)(vmt[output])))
			++output;
		return output;
	}

public:
	size_t FunctionCount = 0;
	VMT CustomVMT;
	VMT OriginalVMT;
	VMT* ActiveVMT;

	VMTManager(VMT* pVMT)
	{
		this->ActiveVMT = pVMT;
		this->OriginalVMT = *this->ActiveVMT;
		this->FunctionCount = CountFunctionsInVMT(this->OriginalVMT);

		// create a copy
		this->CustomVMT = new GenericFunction[this->FunctionCount];
		for (size_t i = 0; i < this->FunctionCount; i++)
			this->CustomVMT[i] = this->OriginalVMT[i];

		*this->ActiveVMT = this->CustomVMT;
	}

	GenericFunction Hook(int Index, GenericFunction NewFunction)
	{
		this->CustomVMT[Index] = NewFunction;
		return this->OriginalVMT[Index];
	}

	void Unhook(int Index)
	{
		this->CustomVMT[Index] = this->OriginalVMT[Index];
	}

	void UnhookAll()
	{
		for (size_t index = 0; index < this->FunctionCount; index++)
			this->Unhook(index);
	}
	
	~VMTManager()
	{
		*this->ActiveVMT = this->OriginalVMT;
		delete[] this->CustomVMT;
	}
};