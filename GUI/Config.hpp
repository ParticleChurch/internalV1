#pragma once
#include <Windows.h>
#include <fstream>
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include "../Valve/Custom/Color.hpp"
#include <map>
#include <vector>
#include "VKeys.hpp"
#include "SkinConstants.hpp"
#include "animation.hpp"
#include "HTTP.hpp"
#include "ConfigConstants.hpp"

namespace Config2
{
	constexpr unsigned char VERSION_MAJOR = 1;
	constexpr unsigned char VERSION_MINOR = 1;
	extern uint64_t GUIFramesRenderedCounter;

	struct Visibility;
	struct Property;
	struct Group;
	struct Tab;

	extern std::map<std::string, Property*> PropertyTable;
	extern std::vector<Tab*> Tabs;

	// for keybinds
	extern Property* SettingKeybindFor;
	enum class PropertyType : int
	{
		TEXTINPUT,
		LABEL,
		BOOLEAN,
		FLOAT,
		PAINTKIT,
		KNIFE,
		GLOVE,
		HSTATEFUL,
		VSTATEFUL,
		COLOR,
		MULTISELECT,
	};
	enum class KeybindMode
	{
		TOGGLE,
		HOLDTOENABLE,
		HOLDTODISABLE,
	};
	extern std::string KeybindTypeNames[];

	// property types
	struct CState;
	struct CTextInput;
	struct CLabel;
	struct CBoolean;
	struct CFloat;
	struct CPaintKit;
	struct CKnife;
	struct CGlove;
	struct CColor;
	struct CHorizontalState;
	struct CVerticalState; // lmaoooo i really called a dropdown this
	struct CMultiSelect;

	// util
	extern void _KeyStateChanged(int index, bool currentlyPressed);
	extern void _BindToKey(Property* p, int index);

	// getters
	extern Property* GetProperty(std::string Name);
	extern int GetKeybind(std::string Name);
	extern CFloat* GetFloat(std::string Name);
	extern CPaintKit* GetPaintKit(std::string Name);
	extern CState* GetState(std::string Name);
	extern CColor* GetColor(std::string Name);
	extern CMultiSelect* GetSelected(std::string Name);
	extern CTextInput* GetText(std::string Name);

	// import/export theme/config
	extern bool ExportSingleProperty(Property* p, char** buffer, size_t* size, size_t* capacity);
	extern bool ImportSingleProperty(const char* buffer, size_t bufferSize, size_t* nBytesUsed);
	extern char* ExportTheme(size_t* nBytesOut);
	extern char* ExportConfig(size_t* nBytesOut);
	extern void ImportTheme(const char* Theme, size_t nBytes);
	extern void ImportConfig(const char* buffer, size_t nBytes);

	// file openers
	extern void PromptImportThemeFile();
	extern void PromptExportThemeFile();
	extern void PromptImportConfigFile();
	extern void PromptExportConfigFile();

	// config functionality
	extern void Init();
	extern void ProcessKeys();
	extern void Free();

	extern std::vector<CPaintKit*> WeaponPaintKits;
};

namespace UserData
{
	// ping approx. every 60 seconds
	extern TIME_POINT LastSuccessfulServerContact;
	extern TIME_POINT LastServerContactAttempt;

	extern bool Initialized; // this will be true if the main cheat gui should show up
	extern bool Authenticated; // this will be true only if the user entered a valid email/password combo
	extern std::string Email;
	extern std::string SessionID;
	extern uint64_t UserID;
	extern bool Premium;

	extern bool BusyAttemptingLogin;
	extern size_t LoginAttemptCounter;
	extern size_t PingAttemptCounter;
	extern std::string LoginError;
	extern TIME_POINT LoginErrorOriginTime;
	
	extern std::string CredentialsFile;
	struct LoginInformation
	{
		std::string Email;
		std::string Password;
	};
	extern DWORD WINAPI AttemptLogin(LPVOID pInfo);
	extern DWORD WINAPI PingServer(LPVOID pInfo);
}

namespace Config2
{
	struct CState
	{
		// note that this class does not have a type
		// use the wrappers instead, like CBoolean or CHorizontalState
	private:
		int Minimum = 0;
		int Maximum = 1;
		int Value = 0;
		int LastValue = 0;
		TIME_POINT TimeChanged = std::chrono::steady_clock::time_point(std::chrono::seconds(0));

	public:
		CState(int min, int max, int val)
		{
			this->Minimum = min;
			this->Maximum = max;
			this->Value = val;
			this->LastValue = val;
		}

		double GetTimeSinceChange()
		{
			return Animation::delta(Animation::now(), this->TimeChanged);
		}

		int GetLastValue()
		{
			return this->LastValue;
		}

		void Increment()
		{
			this->LastValue = this->Value;
			if (++this->Value > this->Maximum) this->Value = this->Minimum;
			this->TimeChanged = Animation::now();
		}

		void Invert()
		{
			this->LastValue = this->Value;
			this->Value = this->Maximum - (this->Value - this->Minimum);
			this->TimeChanged = Animation::now();
		}

		int Get()
		{
			return this->Value;
		}

		void Set(int value)
		{
			if (value == this->Value) return;

			this->LastValue = this->Value;
			if (value <= this->Minimum) this->Value = this->Minimum;
			else if (value >= this->Maximum) this->Value = this->Maximum;
			else this->Value = value;
			this->TimeChanged = Animation::now();
		}
	};
	struct CTextInput
	{
		static const PropertyType Type = PropertyType::TEXTINPUT;

		size_t DataSize = 0;
		char* Data;
		CTextInput(size_t NumChars = 256)
		{
			this->Data = new char[NumChars];
			if (this->Data) ZeroMemory(this->Data, this->DataSize = NumChars);
			else this->DataSize = 0;
		}
		~CTextInput()
		{
			if (this->Data) delete this->Data;
		}
		std::string Get()
		{
			return std::string(this->Data, this->DataSize);
		}
	};
	struct CLabel
	{
		static const PropertyType Type = PropertyType::LABEL;
		// placeholder for when you just want the text to show up
	};
	struct CBoolean
	{
		static const PropertyType Type = PropertyType::BOOLEAN;

	public:
		CState Value = CState(0, 1, 0);
		bool Bindable = true;
		int BoundToKey = -1;
		KeybindMode BindMode = KeybindMode::TOGGLE;

		CBoolean(bool Bindable = true)
		{
			this->Bindable = Bindable;
		}

		std::string Stringify()
		{
			return this->Value.Get() ? "true" : "false";
		}
	};
	struct CFloat
	{
		static const PropertyType Type = PropertyType::FLOAT;

	private:
		float Value;
		float Minimum, Maximum;
		int Decimals;
		int Percision;

	public:
		std::string Unit = "";

		CFloat(float Min = 0, float Max = 1, int Decimals = 2, std::string Unit = "")
		{
			this->Minimum = Min;
			this->Maximum = Max;
			this->Decimals = max(min(Decimals, 6), 0);
			this->Percision = (int)pow(10, this->Decimals);
			this->Unit = Unit;

			this->Set(Min);
		}

		std::string Stringify()
		{
			std::string s = std::to_string((int64_t)((double)this->Value * (double)this->Percision + 0.5));
			if (this->Decimals <= 0) return s;
			if (s == "0")
			{
				s = "0.";
				for (int i = 0; i < this->Decimals; i++) s += "0";
				return s;
			}
			while (s.length() <= (size_t)this->Decimals) s = "0" + s;

			return s.substr(0, s.length() - this->Decimals) + "." + s.substr(s.length() - this->Decimals, this->Decimals);
		}

		float GetFactor()
		{
			return (this->Value - this->Minimum) / (this->Maximum - this->Minimum);
		}

		void SetFactor(float v)
		{
			this->Set(v * (this->Maximum - this->Minimum) + this->Minimum);
		}

		float Get()
		{
			return this->Value;
		}

		void Set(float v)
		{
			v = floorf(v * this->Percision + 0.5f) / (float)this->Percision;
			this->Value = min(max(this->Minimum, v), this->Maximum);
		}
	};
	struct CPaintKit
	{
		static const PropertyType Type = PropertyType::PAINTKIT;

		int Mode = 0; // 0 == PaintKit.Weapons, 1 == PaintKit.Knives, 2 == PaintKit.Gloves
		int Version = 0; // Index in mode

		Skins::PaintKit* PaintKit;

		CPaintKit(Skins::PaintKit* PaintKit = nullptr)
		{
			if (PaintKit)
				this->PaintKit = PaintKit;
			else
				this->PaintKit = &Skins::PaintKits[0];
		}

		void ClearSelection()
		{
			this->PaintKit = &Skins::PaintKits[0];
			this->Mode = 0;
			this->Version = 0;
		}

		std::string Stringify()
		{
			if (!this->PaintKit) return "-";

			switch (this->Mode)
			{
			case 0:
				if ((size_t)this->Version < this->PaintKit->Weapons.size())
					return Skins::WeaponNames[(int)this->PaintKit->Weapons[this->Version]] + " | " + this->PaintKit->VisibleName;
				else
					return this->PaintKit->VisibleName;
			case 1:
				if ((size_t)this->Version < this->PaintKit->Knives.size())
					return Skins::KnifeNames[(int)this->PaintKit->Knives[this->Version]] + " | " + this->PaintKit->VisibleName;
				else
					return this->PaintKit->VisibleName;
			case 2:
				if ((size_t)this->Version < this->PaintKit->Gloves.size())
					return Skins::GloveNames[(int)this->PaintKit->Gloves[this->Version]] + " | " + this->PaintKit->VisibleName;
				else
					return this->PaintKit->VisibleName;
			default:
				return this->PaintKit->VisibleName;
			}
		}
	};
	struct CKnife
	{
		static const PropertyType Type = PropertyType::KNIFE;
	};
	struct CGlove
	{
		static const PropertyType Type = PropertyType::GLOVE;
	};
	struct CHorizontalState
	{
		static const PropertyType Type = PropertyType::HSTATEFUL;

	public:
		std::vector<std::string> StateNames;
		CState Value = CState(0, 0, 0);
		bool Bindable = false;
		int BoundToKey = -1;


		CHorizontalState(std::vector<std::string> States, bool Bindable = false)
		{
			this->Value = CState(0, (int)States.size() - 1, 0);
			this->StateNames = States;
			this->Bindable = Bindable;
		}
	};
	struct CVerticalState
	{
		static const PropertyType Type = PropertyType::VSTATEFUL;

	public:
		std::vector<std::string> StateNames;
		CState Value = CState(0, 0, 0);
		bool Bindable = true;
		int BoundToKey = -1;

		CVerticalState(std::vector<std::string> States, bool Bindable = false)
		{
			this->Value = CState(0, (int)States.size() - 1, 0);
			this->StateNames = States;
			this->Bindable = Bindable;
		}
	};
	struct CColor
	{
		static const PropertyType Type = PropertyType::COLOR;

	private:
		unsigned char R = 0;
		unsigned char G = 0;
		unsigned char B = 0;
		unsigned char A = 255; // 255 = opaque, 0 = transparent
		bool HasAlpha = false;

	public:
		CColor(bool HasAlpha = false)
		{
			this->R = this->G = this->B = 0;
			this->A = 255;
			this->HasAlpha = HasAlpha;
		}
		CColor(unsigned char R, unsigned char G, unsigned char B)
		{
			this->R = R;
			this->G = G;
			this->B = B;
			this->A = 255;
			this->HasAlpha = false;
		}
		CColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
		{
			this->R = R;
			this->G = G;
			this->B = B;
			this->A = A;
			this->HasAlpha = true;
		}

		__forceinline CColor ModulateAlpha(float f)
		{
			return CColor(this->R, this->G, this->B, (unsigned char)((float)this->A * f + 0.5f));
		}
		__forceinline CColor ModulateAlpha(unsigned char f)
		{
			return this->ModulateAlpha((float)f / 255.f);
		}

		float Lightness()
		{
			return ((float)this->R + (float)this->G + (float)this->B) / 3.f;
		}

		std::string Stringify()
		{
			if (this->HasAlpha)
				return "rgba(" + std::to_string((int)this->R) + ", " + std::to_string((int)this->G) + ", " + std::to_string((int)this->B) + ", " + std::to_string((int)this->A) + ")";
			else
				return "rgb(" + std::to_string((int)this->R) + ", " + std::to_string((int)this->G) + ", " + std::to_string((int)this->B) + ")";
		}

		__forceinline uint32_t As32Bit()
		{
			return (uint32_t)this->R << 0 | (uint32_t)this->G << 8 | (uint32_t)this->B << 16 | (uint32_t)this->A << 24;
		}

		operator ImVec4 ()
		{
			return ImVec4((float)this->R / 255.f, (float)this->G / 255.f, (float)this->B / 255.f, (float)this->A / 255.f);
		}

		operator ImU32 ()
		{
			return this->As32Bit();
		}

		unsigned char GetR() { return this->R; }
		unsigned char GetG() { return this->G; }
		unsigned char GetB() { return this->B; }
		unsigned char GetA() { return this->A; }
		unsigned char GetHasAlpha() { return this->HasAlpha; }
		void SetR(unsigned char value) { this->R = value; }
		void SetG(unsigned char value) { this->G = value; }
		void SetB(unsigned char value) { this->B = value; }
		void SetA(unsigned char value) { if (this->HasAlpha) this->A = value; }
	};
	struct CMultiSelect
	{
		static const PropertyType Type = PropertyType::MULTISELECT;
	
	public:
		uint64_t Mask = 0;
		std::vector<std::string> StateNames;

		CMultiSelect(std::vector<std::string> States)
		{
			StateNames = States;
			Mask = 0;
		}

		__forceinline size_t CountSelected()
		{
			size_t out = 0;
			for (size_t i = 0; i < this->StateNames.size(); i++)
				if (Get(i))
					out++;
			return out;
		}

		__forceinline bool Get(size_t index)
		{
			if (index > 63)
			{
				L::Log("AYOOO YOU CAN'T SET MORE THAN 64 THINGS");
				return false;
			}
			return this->Mask & ((uint64_t)1 << index);
		}

		__forceinline void Set(size_t index, bool value)
		{
			if (Get(index) == value) return;
			this->Mask ^= ((uint64_t)1 << index);
		}
	};

	struct Visibility
	{
	private:
		uint64_t LastCalculationFrame = 0;
		bool LastCalculationValue = true;

	public:
		bool (*Lambda)() = nullptr;
		CState* State = nullptr;
		int StateEquals = 0;

		Visibility(bool (*Lambda)() = nullptr, CState* State = nullptr, int StateEquals = 0) : Lambda{ Lambda }, State{ State }, StateEquals{ StateEquals }{}
		bool Calculate()
		{
			if (this->LastCalculationFrame == GUIFramesRenderedCounter) return this->LastCalculationValue;

			this->LastCalculationFrame = GUIFramesRenderedCounter;
			if (
				// state says not visible
				(this->State && this->State->Get() != this->StateEquals)
				|| // or lambda says not visible
				(this->Lambda && !this->Lambda())
				)
				return this->LastCalculationValue = false;
			return this->LastCalculationValue = true;
		}
	};
	struct Property
	{
	public:
		Property* Master = nullptr;
		Visibility Visible;
		Property* VisibilityLinked = nullptr;

		// meta info
		bool IsComplex = false;
		bool IsPremium = false;

		void* Value;
		PropertyType Type;

		std::string Name;
		std::string VisibleName;

		template <typename T>
		Property(std::string Name, std::string VisibleName, T* Value) : Visible{}
		{
			this->Name = Name;
			this->VisibleName = VisibleName;
			this->Type = Value->Type;
			this->Value = (void*)Value;

			PropertyTable.insert(std::make_pair(this->Name, this));
		}

		bool IsVisible()
		{
			if (this->VisibilityLinked)
				return this->VisibilityLinked->IsVisible() && this->Visible.Calculate();
			else
				return this->Visible.Calculate();
		}

		std::string Stringify()
		{
			switch (this->Type)
			{
			case PropertyType::BOOLEAN:
				return ((CBoolean*)this->Value)->Stringify();
			case PropertyType::FLOAT:
				return ((CBoolean*)this->Value)->Stringify();
			}
			return "[error: unexpected type]";
		}

		~Property()
		{
			if (!this->Value) return;
			switch (this->Type)
			{
			case PropertyType::BOOLEAN:
				delete (CBoolean*)this->Value;
				break;
			case PropertyType::FLOAT:
				delete (CFloat*)this->Value;
				break;
			}
		}
	};
	struct Group
	{
	private:
		float LastDrawHeight = -1.f;
		Property* CurrentMaster = nullptr;

	public:
		int Padding = 5;
		int Rounding = 5;
		std::string Title;
		bool ShowTitle = true;

		std::vector<Property*> Properties;

		Group(std::string Title)
		{
			this->Title = Title;
		}

		template <typename T>
		Property* Add(std::string Name, std::string VisibleName, T* Value)
		{
			Property* x = new Property(Name, VisibleName, Value);
			if (this->CurrentMaster)
				x->Master = this->CurrentMaster;
			this->Properties.push_back(x);
			return x;
		}

		void BeginMaster(Property* Master)
		{
			this->CurrentMaster = Master;
		}
		void EndMaster()
		{
			this->CurrentMaster = nullptr;
		}

		float GetDrawHeight()
		{
			// return actual height
			//if (this->LastDrawHeight >= 0.f) return this->LastDrawHeight;
			
			// try to calculate it
			float h = this->ShowTitle ? 5.f + 18.f : 0.f;
			for (size_t p = 0; p < this->Properties.size(); p++)
			{
				auto Property = this->Properties[p];
				if (!Property->IsVisible()) continue;
				
				h += this->Padding + 20;
			}
			h += this->Padding;

			return h;
		}

		void SetDrawHeight(float DrawHeight)
		{
			this->LastDrawHeight = DrawHeight;
		}
	};
	struct Tab
	{
		std::string Name;
		std::vector<Group*> Groups;

		bool InitialPaint = true;
		int ScrollHeight = 0;
		int TopPadding = 0;
		int VerticalPadding = 10;
		int HorizontalPadding = 10;

		Tab(std::string Name)
		{
			this->Name = Name;
			Tabs.push_back(this);
		}
		Group* Add(std::string Title)
		{
			Group* x = new Group(Title);
			this->Groups.push_back(x);
			return x;
		}
	};
}