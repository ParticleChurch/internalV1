#pragma once

namespace VMT { // Virtual Method Table
	typedef void* GenericSubclassInstance;
	typedef void* GenericFunction; // a function is a pointer to executable memory
	typedef GenericFunction* VMT; // a VMT is an array of functions

	// just get a pointer to a virtual method
	template <typename FunctionSignature>
	inline FunctionSignature GetVirtualMethod(GenericSubclassInstance object, int index)
	{
		const VMT& vmt = *(VMT*)object;
		return static_cast<FunctionSignature>(vmt[index]);
	}

	// figure out how many functions are in the vmt
	// vmts don't have a null terminator, but there will be some random value at the end
	// which likely won't point to executable memory, that's what IsBadCodePtr checks
	inline size_t CountFunctions(VMT vmt)
	{
		size_t output = 0;

		// really fkn annoying that this causes debugger interrupt
		// but that is "by design". Fuck you microsoft
		// just step over it, nothing actually went wrong
		while (IsBadCodePtr((FARPROC)(vmt[output])) == 0)
			++output;
		
		return output;
	}

	struct Manager
	{
		size_t FunctionCount = 0;
		VMT CustomVMT; // starts as a copy of the original, but you can edit this one
		VMT OriginalVMT; // untouched original VMT, allocated by Valve
		VMT* ActiveVMT; // pointer to the VMT that is being used currently

		Manager(VMT* pVMT)
		{
			this->ActiveVMT = pVMT;
			this->OriginalVMT = *this->ActiveVMT;

			// create a copy
			this->FunctionCount = CountFunctions(this->OriginalVMT);
			this->CustomVMT = new GenericFunction[this->FunctionCount];
			memcpy(this->CustomVMT, this->OriginalVMT, this->FunctionCount * sizeof(GenericFunction));

			// use the custom vmt
			*this->ActiveVMT = this->CustomVMT;
		}

		// returns the original
		GenericFunction Hook(int Index, GenericFunction NewFunction)
		{
			this->CustomVMT[Index] = NewFunction;
			return this->OriginalVMT[Index];
		}

		// replaces the index with the origianl function
		void Unhook(int Index)
		{
			this->CustomVMT[Index] = this->OriginalVMT[Index];
		}

		void UnhookAll()
		{
			memcpy(this->CustomVMT, this->OriginalVMT, this->FunctionCount * sizeof(GenericFunction));
		}

		void PauseHooks()
		{
			*this->ActiveVMT = this->OriginalVMT;
		}

		void ResumeHooks()
		{
			*this->ActiveVMT = this->CustomVMT;
		}

		~Manager()
		{
			*this->ActiveVMT = this->OriginalVMT;
			delete[] this->CustomVMT;
		}
	};
}

/* ONLY AVAILALBE IN C++20
#define _VMT_UNPACK_UTIL(...) __VA_ARGS__
#define _VMT_UNPACK(tuple) _VMT_UNPACK_UTIL tuple
#define _VMT_PREPEND_UTIL(before, ...) before __VA_OPT__(,) __VA_ARGS__
#define _VMT_PREPEND(v, args_tuple) (_VMT_PREPEND_UTIL(v, _VMT_UNPACK(args_tuple)))
#define DECLARE_VIRTUAL_METHOD(return_type, name, index, in_args, pass_args) inline return_type name in_args noexcept \
{ \
	typedef return_type (__thiscall* vfunc) _VMT_PREPEND(void* self, in_args); \
	return VMT::GetVirtualMethod<vfunc>(this, index) _VMT_PREPEND(this, pass_args); \
}
*/