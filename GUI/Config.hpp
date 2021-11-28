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
#include "../json.hpp"
#include "../Utils/time.hpp"
#include "ConfigConstants.hpp"

namespace Config
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

	extern nlohmann::ordered_json SearchableFeatures;
	extern nlohmann::ordered_json SearchStopwords;

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
		FUNCTION,
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
	extern void _KeyStateChanged(int index, bool currentlyPressed, bool checkPremium = true);
	extern void _BindToKey(Property* p, int index);

	// getters
	extern Property* GetProperty(const std::string Name);
	extern int GetKeybind(const std::string Name);
	extern CFloat* GetFloat(const std::string Name);
	extern CPaintKit* GetPaintKit(const std::string Name);
	extern CState* GetState(const std::string Name);
	extern CColor* GetColor(const std::string Name);
	extern CMultiSelect* GetSelected(const std::string Name);
	extern CTextInput* GetText(const std::string Name);

	// import/export theme/config
	extern bool ExportSingleProperty(Property* p, char** buffer, size_t* size, size_t* capacity);
	extern bool ImportSingleProperty(const char* buffer, size_t bufferSize, size_t* nBytesUsed, bool checkPremium = true);
	extern char* ExportTheme(size_t* nBytesOut);
	extern char* ExportConfig(size_t* nBytesOut);
	extern void ImportTheme(const char* Theme, size_t nBytes);
	extern void ImportConfig(const char* buffer, size_t nBytes, bool checkPremium = true);

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
	extern std::string SessionId;
	extern bool Authenticated;
	extern bool Premium;
	extern uint32_t PremiumTimeRemaining;
	extern uint32_t UserId;

	extern bool LoginDebounce;
	extern std::string LoginError;
	extern TIME_POINT LoginErrorTime;
	extern std::string CredentialsFile;
	struct LoginInformation
	{
		std::string Email;
		std::string Password;
	};

	extern bool PingDebounce;
	extern TIME_POINT LastPingTime;

	extern DWORD WINAPI AttemptLogin(LPVOID pInfo);
	extern DWORD WINAPI GetUnauthenticatedSession(LPVOID pInfo);
	extern DWORD WINAPI PingServer(LPVOID pInfo);
	extern bool ConnectAPI();
}

namespace Config
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
		void(*OnChange)() = nullptr;

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

			if (this->OnChange) this->OnChange();
		}

		void Invert()
		{
			this->LastValue = this->Value;
			this->Value = this->Maximum - (this->Value - this->Minimum);
			this->TimeChanged = Animation::now();

			if (this->OnChange) this->OnChange();
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

			if (this->OnChange) this->OnChange();
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
	struct CFunction
	{
		static const PropertyType Type = PropertyType::FUNCTION;

	public:
		bool Bindable = true;
		int BoundToKey = -1;
		std::string Label = "";
		void(*Callback)() = nullptr;

		CFunction(std::string Label, bool Bindable = true)
		{
			this->Bindable = Bindable;
			this->Label = Label;
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

		const Skins::PaintKit* PaintKit;

		CPaintKit(const Skins::PaintKit* PaintKit = nullptr)
		{
			this->ClearSelection();
			if (PaintKit)
				this->PaintKit = PaintKit;
		}

		void ClearSelection()
		{
			this->PaintKit = Skins::PaintKits + 0;
			this->Mode = 0;
			this->Version = 0;
		}

		std::string Stringify()
		{
			if (!this->PaintKit) return "-";

			switch (this->Mode)
			{
			case 0:
				return Skins::WeaponNames[(int)this->PaintKit->Weapons[this->Version]] + " | " + this->PaintKit->VisibleName;
			case 1:
				return Skins::KnifeNames[(int)this->PaintKit->Knives[this->Version]] + " | " + this->PaintKit->VisibleName;
			case 2:
				return Skins::GloveNames[(int)this->PaintKit->Gloves[this->Version]] + " | " + this->PaintKit->VisibleName;
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
		int PixelWidth = -1;


		CHorizontalState(std::vector<std::string> States, bool Bindable = false, int PixelWidth = -1)
		{
			this->Value = CState(0, (int)States.size() - 1, 0);
			this->StateNames = States;
			this->Bindable = Bindable;
			this->PixelWidth = PixelWidth;
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
				L::Info("AYOOO YOU CAN'T SET MORE THAN 64 THINGS");
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
			{
				this->LastCalculationValue = false;
				return false;
			}
			this->LastCalculationValue = true;
			return true;
		}
	};
	struct Property
	{
	public:
		Property* Master = nullptr;
		Visibility Visible;
		Property* VisibilityLinked = nullptr;

		Group* Parent = nullptr;

		// meta info
		bool IsPremium = false;

		void* Value;
		PropertyType Type;

		std::string Name;
		std::string VisibleName;
		std::string(*GetWarning)() = []() { return std::string(""); };

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
		Property* CurrentMaster = nullptr;

	public:
		int Padding = 5;
		int Rounding = 5;
		std::string Name;
		bool ShowName = true;

		Tab* Parent = nullptr;

		std::vector<Property*> Properties;

		Group(std::string Name)
		{
			this->Name = Name;
		}

		template <typename T>
		Property* Add(std::string Name, std::string VisibleName, T* Value)
		{
			Property* x = new Property(Name, VisibleName, Value);
			x->Parent = this;
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

		size_t CountVisibleProperties()
		{
			size_t c = 0;
			for (size_t p = 0; p < this->Properties.size(); p++)
			{
				if (!this->Properties[p]->IsVisible()) continue;
				c++;
			}
			return c;
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

		struct {
			bool Scroll = false;
			Property* Property = nullptr;
			TIME_POINT Time = std::chrono::steady_clock::time_point(std::chrono::seconds(0));
		} Highlight;

		Tab(std::string Name)
		{
			this->Name = Name;
			Tabs.push_back(this);
		}
		Group* Add(std::string Title)
		{
			Group* x = new Group(Title);
			x->Parent = this;
			this->Groups.push_back(x);
			return x;
		}
	};
}

/*
	Includes
*/
#include <vector>
#include <string>
#include <set>
#include <map>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

/*
	Keybinds
*/
namespace Config2 {
	namespace Keybind {
		struct Keybind;

		typedef int Key; // corresponds to VirtualKey codes
		extern std::map<Key, std::set<Keybind*>> keybinds;

		enum class Mode {
			Trigger = 0,
			Toggle,
			EnableWhileHolding,
			DisableWhileHolding,
		};

		struct Keybind {
		private:
			Key key = 0;

			bool isPressed = false;
			bool toggle = false;
			bool trigger = false;

			void setKeyState(bool isDown)
			{
				static bool lastWasDown = !isDown;

				if (lastWasDown == isDown) return;
				lastWasDown = isDown;

				this->trigger = isDown;
				this->isPressed = isDown;

				if (isDown) this->toggle ^= 1;
			}

			friend inline void update();

		public:
			Mode mode = Mode::Trigger;

			Keybind(Mode mode = Mode::Toggle, Key key = 0) : mode(mode)
			{
				this->changeKey(key);
			}

			void changeKey(Key key)
			{
				// remove current bind
				{
					auto it = keybinds.find(this->key);
					if (it != keybinds.end() && it->second.find(this) != it->second.end())
					{
						it->second.erase(this);

						if (it->second.empty())
							keybinds.erase(this->key);
					}
				}

				// add new bind
				{
					this->key = key;
					auto it = keybinds.find(this->key);
					if (it == keybinds.end())
						keybinds.insert(std::pair<Key, std::set<Keybind*>>(this->key, { this }));
					else
						it->second.insert(this);
				}

				// unbind
				if (key == 0)
				{
					this->setKeyState(false);
				}
			}

			/*
				if mode = Mode::Trigger
					will return true if the even should trigger right now
				else
					will return true if the state should be 1, otherwise return false
			*/
			bool consume()
			{
				switch (this->mode)
				{
				case Mode::Trigger:
				{
					bool v = this->trigger;
					this->trigger = false;
					return v;
				}
				case Mode::EnableWhileHolding:
					return this->isPressed;
				case Mode::DisableWhileHolding:
					return !this->isPressed;
				case Mode::Toggle:
					return this->toggle;
				}

				// should not be possible
				return false;
			}
		};

		inline void update()
		{
			for (const auto& [key, binds] : keybinds)
			{
				// not bound
				if (key == 0) continue;

				bool isDown = GetAsyncKeyState(key) < 0;
				for (const auto& bind : binds)
					bind->setKeyState(isDown);
			}
		}
	}
}

/*
	Property type definitions
*/
#define FIRST_PROP_ARGS Group* group, std::string codeName, std::string visibleName
#define FIRST_PROP_ARGS_FWD group, codeName, visibleName
#define LAST_PROP_ARGS Visibility visibility = {}
#define LAST_PROP_ARGS_FWD visibility

namespace Config2 {
	struct Property;
	struct State;
	struct Boolean;
	struct FloatingPoint;

	struct Group;
	struct Tab;
	extern std::vector<Tab*> tabs;

	struct Tab {
		std::string codeName;
		std::string visibleName;
		std::vector<Group*> groups;

		Tab(std::string codeName, std::string visibleName, std::vector<Group*> groups = {}) : codeName(codeName), visibleName(visibleName), groups(groups)
		{
			tabs.push_back(this);
		}
	};

	struct Group {
		std::string codeName;
		std::string visibleName;
		std::vector<Property*> properties;

		Group(Tab* tab, std::string codeName, std::string visibleName, std::vector<Property*> properties = {}) : codeName(codeName), visibleName(visibleName), properties(properties)
		{
			tab->groups.push_back(this);
		}
	};

	struct Visibility
	{
		size_t cachedOnFrame = (size_t)-1;
		bool cachedValue = false;

		Property* chain = nullptr;

		struct {
			State* prop = nullptr;
			int equals = 0;
		} stateful;

		struct {
			FloatingPoint* prop = nullptr;
			float greaterThan = -INFINITY;
			float lessThan = INFINITY;
		} floating;

		inline Visibility(State* prop, int equals)
		{
			this->stateful.prop = prop;
			this->stateful.equals = equals;
		}
		inline Visibility(Boolean* prop, bool equals = true) : Visibility((State*)prop, (int)equals) {}

		inline Visibility(FloatingPoint* prop, float greaterThan = -INFINITY, float lessThan = INFINITY)
		{
			this->floating.prop = prop;
			this->floating.greaterThan = greaterThan;
			this->floating.lessThan = lessThan;
		}

		inline Visibility(Property* chain)
		{
			this->chain = chain;
		}

		inline Visibility() = default;
	};

	struct Property {
		Keybind::Keybind bind;

		std::string codeName;
		std::string visibleName;

		Property(FIRST_PROP_ARGS, LAST_PROP_ARGS) : codeName(codeName), visibleName(visibleName), visibility(visibility)
		{
			group->properties.push_back(this);
		}

		virtual void draw() = 0;
		virtual std::string toString() = 0;
		virtual bool fromString(const std::string& s) = 0;
		virtual void processKeybind() = 0;

	protected:
		Visibility visibility;

	public:
		inline bool isVisible();
	};

	struct State : public Property {
	protected:
		int value = 0;
		int min = 0, max = 0;

		int lastValue = 0;
		float timeChanged = 0.f;

	public:
		State(FIRST_PROP_ARGS, int min, int max, LAST_PROP_ARGS) : Property(FIRST_PROP_ARGS_FWD, LAST_PROP_ARGS_FWD)
		{
			this->min = min;
			this->max = max;
			this->value = min;
			this->lastValue = this->value;
		}

		virtual std::string toString() 
		{
			return std::to_string(this->value);
		}
		virtual bool fromString(const std::string& s)
		{
			try {
				this->set(std::stoi(s));
				return true;
			} catch(...) {
				return false;
			}
		}
		virtual void processKeybind()
		{
			this->set(this->bind.consume() ? this->min : this->max);
		}

		const int& getLastValue() { return this->lastValue; }
		const float& getTimeChanged() { return this->timeChanged; }
		const int& get() { return this->value; }
		void set(int val) {
			if (this->value == val) return;

			this->lastValue = this->value;
			this->value = std::clamp(val, this->min, this->max);
			this->timeChanged = getCurrentTimestamp();
		}

		const int& operator*()
		{
			return this->get();
		}
	};

	struct FloatingPoint : public Property {
	protected:
		float value = 0.f;
		float min = 0.f, max = 1.f;
		float step = 0.f;

		float lastValue = 0.f;
		float timeChanged = 0.f;

	public:
		FloatingPoint(FIRST_PROP_ARGS, float min, float max, float step = 0.f, LAST_PROP_ARGS) : Property(FIRST_PROP_ARGS_FWD, LAST_PROP_ARGS_FWD)
		{
			this->min = min;
			this->max = max;
			this->step = step;
			this->value = min;
			this->lastValue = this->value;
		}
		FloatingPoint(FIRST_PROP_ARGS, float min, float max, LAST_PROP_ARGS) : FloatingPoint(FIRST_PROP_ARGS_FWD, min, max, 0.f, LAST_PROP_ARGS_FWD) {}

		virtual std::string toString() 
		{
			return std::to_string(this->value);
		}
		virtual bool fromString(const std::string& s)
		{
			try {
				this->set(std::stof(s));
				return true;
			} catch(...) {
				return false;
			}
		}
		virtual void processKeybind()
		{
			this->set(this->bind.consume() ? this->min : this->max);
		}

		const float& getLastValue() { return this->lastValue; }
		const float& getTimeChanged() { return this->timeChanged; }
		const float& get() { return this->value; }
		void set(float val) {
			if (this->step > 0.f)
			{
				int steps = (int)((val - this->min) / this->step + 0.5f);
				val = this->min + steps * this->step;
			}

			this->lastValue = this->value;
			this->value = std::clamp(val, this->min, this->max);
			this->timeChanged = getCurrentTimestamp();
		}


		const float& operator*()
		{
			return this->get();
		}
	};

	struct Slider : FloatingPoint
	{
	protected:
		std::string unit;

	public:
		Slider(
			FIRST_PROP_ARGS, 
			float min, 
			float max, 
			float step = 0.f,
			std::string unit = "", 
			LAST_PROP_ARGS
		) : FloatingPoint(FIRST_PROP_ARGS_FWD, min, max, step, LAST_PROP_ARGS_FWD)
		{
			this->unit = unit;
		}
		Slider(FIRST_PROP_ARGS, float min, float max, std::string unit = "", LAST_PROP_ARGS) : Slider(FIRST_PROP_ARGS_FWD, min, max, 0.f, unit, LAST_PROP_ARGS_FWD) {}
		Slider(FIRST_PROP_ARGS, float min, float max, LAST_PROP_ARGS) : Slider(FIRST_PROP_ARGS_FWD, min, max, 0.f, "", LAST_PROP_ARGS_FWD) {}
	};

	struct Boolean : public State
	{
		Boolean(FIRST_PROP_ARGS, LAST_PROP_ARGS) : State(FIRST_PROP_ARGS_FWD, 0, 1, LAST_PROP_ARGS_FWD) {}

		const bool& get() { return this->value; }

		virtual void draw()
		{
			// TODO
		}

		const bool& operator*()
		{
			return this->get();
		}
	};

	struct NamedState : public State
	{
	protected:
		std::vector<std::string> values;

	public:
		NamedState(FIRST_PROP_ARGS, std::initializer_list<std::string> values, LAST_PROP_ARGS) : State(FIRST_PROP_ARGS_FWD, 0, values.size(), LAST_PROP_ARGS_FWD), values{ values } { }

		// more efficient than toString because doesn't make a copy
		const std::string& getString()
		{
			return this->values.at(this->value);
		}

		// use getString instead if possible
		virtual std::string toString()
		{
			return this->values.at(this->value);
		}

		virtual bool fromString(const std::string& s)
		{
			for (int i = 0; i < this->max; i++)
			{
				if (this->values.at(i) == s)
				{
					this->value = i;
					return true;
				}
			}

			return false;
		}
	};

	struct SingleDropdown : public NamedState
	{
		using NamedState::NamedState;

		virtual void draw()
		{
			// TODO
		}
	};

	struct HorizontalSelect : public NamedState
	{
		using NamedState::NamedState;

		virtual void draw()
		{
			// TODO
		}
	};

	struct BigHorizontalSelect : public HorizontalSelect
	{
		using HorizontalSelect::HorizontalSelect;

		virtual void draw()
		{
			// TODO
		}
	};

	inline bool Property::isVisible()
	{
		if (Config::GUIFramesRenderedCounter == this->visibility.cachedOnFrame) return this->visibility.cachedValue;

		const Visibility& vis = this->visibility;
		this->visibility.cachedValue =
			(!vis.chain || vis.chain->isVisible()) &&
			(!vis.stateful.prop || vis.stateful.prop->get() == vis.stateful.equals) &&
			(!vis.floating.prop || (vis.floating.greaterThan <= vis.floating.prop->get() && vis.floating.prop->get() <= vis.floating.lessThan));

		this->visibility.cachedOnFrame = Config::GUIFramesRenderedCounter;
		return this->visibility.cachedValue;
	}
}

#undef FIRST_PROP_ARGS 
#undef FIRST_PROP_ARGS_FWD 
#undef LAST_PROP_ARGS 
#undef LAST_PROP_ARGS_FWD

/*
	Property declarations
*/
#include "ConfigDefinitions.hpp"
namespace Config2 {
	namespace Enums {
		enum OffenceMode : int {
			Legit = 0,
			Rage,
		};
	}
	
	namespace Properties {
		DECLARE_CONFIG
	}
}

/*
	Config utility
*/
namespace Config2 {
	rapidjson::Document parseConfig(const char* bytes, size_t byteCount, bool* isValidOutput = nullptr, std::string* errorOutput = nullptr);
	std::vector<std::string> findValidTabsInConfig(const rapidjson::Document& json);
	std::vector<std::string> loadConfig(const rapidjson::Document& json, std::vector<Tab*> includeTabs);
	rapidjson::Document dumpConfig(std::vector<Tab*> includeTabs);
}