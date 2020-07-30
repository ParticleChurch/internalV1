#pragma once

class ProtectGuard
{
public:
	ProtectGuard(void* Base, size_t Len, std::uint32_t Flags)
	{
		this->Base = Base;
		this->Length = Len;
		if (!VirtualProtect(Base, Len, Flags, (PDWORD)&this->Old))
			throw std::runtime_error("Failed to protect region.");
	}
	~ProtectGuard()
	{
		VirtualProtect(this->Base, this->Length, this->Old, (PDWORD)&this->Old);
	}

private:
	void* Base;
	size_t        Length;
	std::uint32_t Old;
};

class HookManager
{
private:
	//VFT - virtual function table
	void* ClassBase;
	std::size_t     VFTLength;
	std::uintptr_t* NewVFT;
	std::uintptr_t* OldVFT;

	//estimates the virtual function tables length
	static inline std::size_t EstimateVFTLength(std::uintptr_t* VFTStart)
	{
		unsigned int i = std::size_t{}; //iterator i

		while (VFTStart[i] >= 0x00010000 &&
			VFTStart[i] < 0xFFF00000 &&
			i < 512 /* Hard coded value. Can cause problems, beware.*/) {
			i++;
		}

		return i;
	}
public:
	bool Setup(void* Base = nullptr)
	{
		if (Base != nullptr)	
			this->ClassBase = Base;

		if (this->ClassBase == nullptr)
			return false;

		this->OldVFT = *(std::uintptr_t**)this->ClassBase;
		this->VFTLength = EstimateVFTLength(this->OldVFT);

		if (this->VFTLength == 0)
			return false;

		this->NewVFT = new std::uintptr_t[this->VFTLength + 1]();

		std::memcpy(&this->NewVFT[1], this->OldVFT, this->VFTLength * sizeof(std::uintptr_t));

		try {
			auto guard = ProtectGuard{ this->ClassBase, sizeof(std::uintptr_t), PAGE_READWRITE };
			this->NewVFT[0] = this->OldVFT[-1];
			*(std::uintptr_t**)this->ClassBase = &this->NewVFT[1];
		}
		catch (...) {
			delete[] this->NewVFT;
			return false;
		}

		return true;
	}

	template<typename T>
	void HookIndex(int Index, T Function)
	{
		assert(Index >= 0 && Index <= (int)this->VFTLength);
		this->NewVFT[Index + 1] = reinterpret_cast<std::uintptr_t>(Function);
	}

	void UnhookIndex(int Index)
	{
		this->NewVFT[Index] = this->OldVFT[Index];
	}

	void UnhookAll()
	{
		try {
			if (this->OldVFT != nullptr) {
				auto guard = ProtectGuard{ this->ClassBase, sizeof(std::uintptr_t), PAGE_READWRITE };
				*(std::uintptr_t**)this->ClassBase = this->OldVFT;
				this->OldVFT = nullptr;
			}
		}
		catch (...) {
		}
	}

	template<typename T>
	T GetOriginal(int index)
	{
		return (T)this->OldVFT[index];
	}

	HookManager() : ClassBase(nullptr), VFTLength(0), NewVFT(nullptr), OldVFT(nullptr)
	{
	}

	HookManager(void* Base) : ClassBase(Base), VFTLength(0), NewVFT(nullptr), OldVFT(nullptr)
	{
	}
	
	~HookManager()
	{
		UnhookAll();

		delete[] NewVFT;
	}
};