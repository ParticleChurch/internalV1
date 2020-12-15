#pragma once
#include "../Valve/Custom/Color.hpp"
#include <map>
#include <vector>
#include "ConfigManager.hpp"
//#include "../Include.hpp"

constexpr bool CONFIG_DEBUG = true;

/*
	Web API shit
*/
constexpr int LOGIN_FLAG_LOGIN_VALID            = 1 << 0;  // was the sent login valid
constexpr int LOGIN_FLAG_ACCOUNT_PAID           = 1 << 1;  // is the account paid or free
constexpr int LOGIN_FLAG_ACCOUNT_ALREADY_IN_USE = 1 << 2;  // is the cheat being used elsewhere (this is someone trying to share an account with a friend)
constexpr int LOGIN_FLAG_ACCOUNT_IS_DEVELOPER   = 1 << 3;  // is this login the dev@a4g4.com account
constexpr int LOGIN_FLAG_BANNED                 = 1 << 4;  // is this account banned (do something like: print "fuck you" then crash csgo, lol);

constexpr int AUTH_STATUS_NONE       = 0; // not logged in
constexpr int AUTH_STATUS_PROCESSING = 1; // login is processing
constexpr int AUTH_STATUS_COMPLETE   = 2; // logged in

struct APIResponseFormat_LoginAttempt
{
	int Flags;
	unsigned int UserID;
};

namespace Config {
	struct UserInfoT {
		char AuthStatus = AUTH_STATUS_NONE;
		bool Paid = false;
		bool Developer = false;
		int UserID = INT_MAX;
		std::string Email = "";
	};
	extern UserInfoT UserInfo;

	enum class PropertyType {
		COLOR = 0,
		FLOAT,
		BOOLEAN,
		TEXT,
		TEXTINPUT,
		DROPDOWN,
		INVERTER,
		EDITGROUP,
		MULTISELECT,
		_count
	};
	enum class PropertyComplexity {
		beginner = 0,
		intermediate,
		pro,
		_count
	};
	enum class KeybindType {
		Toggle = 0,
		HoldToEnable,
		HoldToDisable,
	};
	extern std::string StringifyKeybindType(KeybindType type);
	extern std::string StringifyVK(int VirturalKey);
	struct KeybindOptions
	{
	private:
		bool Toggle = false;
		bool HoldToEnable = false;
		bool HoldToDisable = false;
	public:
		KeybindOptions(bool Toggle = false, bool HoldToEnable = false, bool HoldToDisable = false)
		{
			this->Toggle = Toggle;
			this->HoldToEnable = HoldToEnable;
			this->HoldToDisable = HoldToDisable;
		}
		int Count()
		{
			return (int)Toggle + (int)HoldToEnable + (int)HoldToDisable;
		}
		bool IsEnabled(KeybindType type)
		{
			return
				type == KeybindType::Toggle ? this->Toggle :
				type == KeybindType::HoldToEnable ? this->HoldToEnable :
				this->HoldToDisable;
		}
	};

	struct _CGroup_DontUse // only used to store string pairs in a vector
	{
		std::string Name, VisibleName;
	};

	struct CEditGroup
	{
	public:
		size_t SelectedGroup = 0;
		std::vector<_CGroup_DontUse*> Groups;
		std::string Prefix;

		// a property is considered part of this group if:
		//   - it is in the same widget
		//   - it's `Property::Name` starts with `CEditGroup::Prefix`
		// a property in the group is shown if:
		//   - the `Property::Name` has the `CEditGroup::SelectedGroup` right after its prefix
		// example:
		//   - Prefix = "gun-"
		//   - SelectedGroup = "pistol"
		//   + hidden: "gun-sniper-max-backtrack-ticks"
		//   + hidden: "gun-sniper-recoil-control"
		//   + shown : "gun-pistol-max-backtrack-ticks"
		//   + shown : "gun-pistol-recoil-control"
		//   + hidden: "gun-rifle-max-backtrack-ticks"
		//   + hidden: "gun-rifle-recoil-control"
		//   + shown (not in editgroup): "esp-grenade-box"

		CEditGroup(std::string Prefix)
		{
			this->Prefix = Prefix;
		}

		void AddGroup(std::string Name, std::string VisibleName)
		{
			this->Groups.push_back(new _CGroup_DontUse{ Name, VisibleName });
		}

		std::string GetCurrent()
		{
			_CGroup_DontUse* g = this->Groups.at(SelectedGroup);
			return g ? g->Name : "";
		}
	};

	struct CFloat
	{
	private:
		float value;
		float percision;

	public:
		float minimum, maximum;
		size_t decimals;

		CFloat(float min, float max, float value, size_t decimals = 2)
		{
			this->minimum = min;
			this->maximum = max;
			this->decimals = max(min(decimals, 6), 0);
			this->percision = (float)pow(10, this->decimals);
			this->value = value;
		}

		float get()
		{
			return this->value;
		}

		void set(float v)
		{
			v = floorf(v * this->percision + 0.5f) / this->percision;
			// clamp
			this->value = min(max(this->minimum, v), this->maximum);
		}


		std::string Stringify()
		{
			if (this->decimals < 1) return std::to_string((int)(this->value + 0.5f));

			std::string s = std::to_string(this->value);
			size_t i = s.find(".");
			if (i == std::string::npos)
				return s + "." + std::string(this->decimals, '0');

			size_t dcount = s.size() - (i + 1);
			if (dcount == this->decimals) return s;
			if (dcount < this->decimals) return s + std::string(this->decimals - dcount, '0');
			if (dcount > this->decimals) return s.substr(0, i + this->decimals + 1);
			return "ALL PATHS RETURN A VALUE DUMBASS LINTER";
		}
	};

	struct CInverter
	{
		bool State = false;
		std::string State0 = "";
		std::string State1 = "";
		CInverter(std::string State0 = "", std::string State1 = "")
		{
			this->State0 = State0;
			this->State1 = State1;
		}
		std::string Stringify()
		{
			return State ? State1 : State0;
		}
	};


	struct CDropdown
	{
	private:
		size_t SelectedIndex = 0;
	public:
		std::vector<std::string> Options;
		bool IsBindable = false;

		CDropdown(std::initializer_list<const char*> Options)
		{
			for (const char* opt : Options) this->Options.push_back(opt);
		}

		CDropdown(const CDropdown& Other)
		{
			// copy options
			for (std::string opt: Other.Options) this->Options.push_back(opt);

			// copy selections
			this->Select(Other.SelectedIndex);

			// copy bindability
			this->IsBindable = Other.IsBindable;
		}

		void Select(size_t Index)
		{
			if (Index >= this->Options.size()) return;
			this->SelectedIndex = Index;
		}

		size_t GetSelection()
		{
			return this->SelectedIndex;
		}

		std::string Stringify(size_t Index)
		{
			if (Index >= this->Options.size()) return "[null]";
			return this->Options.at(Index);
		}
	};

	struct CMultiSelector
	{
	private:
		std::vector<std::string> SelectionNames;
		std::vector<bool> SelectionValues;

	public:
		CMultiSelector(std::initializer_list<const char*> SelectionNames)
		{
			for (const char* name : SelectionNames)
			{
				this->SelectionNames.push_back(name);
				this->SelectionValues.push_back(false);
			}
		}

		CMultiSelector(const CMultiSelector& other)
		{
			for (size_t i = 0; i < other.SelectionNames.size(); i++)
			{
				this->SelectionNames.push_back(other.SelectionNames[i]);
				this->SelectionValues.push_back(other.SelectionValues[i]);
			}
		}

		uint16_t Get()
		{
			uint16_t v = 0;
			for (uint8_t i = 0; i < 16 && i < this->SelectionValues.size(); i++)
			{
				if (this->SelectionValues[i])
				{
					v |= (1 << i);
				}
			}
			return v;
		}

		bool Get(size_t Index)
		{
			if (Index >= this->SelectionValues.size()) return false;
			return this->SelectionValues[Index];
		}

		void Set(size_t Index, bool value)
		{
			if (Index >= this->SelectionNames.size()) return;
			this->SelectionValues[Index] = value;
		}

		size_t Count()
		{
			return min(this->SelectionNames.size(), this->SelectionValues.size());
		}

		std::string Stringify()
		{
			std::string out = "";
			bool atLeastOne = false;

			for (size_t i = 0; i < SelectionNames.size(); i++)
			{
				if (SelectionValues[i])
				{
					atLeastOne = true;
					out += SelectionNames[i] + ", ";
				}
			}
			if (out.size() >= 2)
			{
				out = out.substr(0, out.size() - 2);
			}

			if (!atLeastOne)
			{
				out = "[None]";
			}

			return out;
		}

		std::string Stringify(size_t Index)
		{
			if (Index >= this->SelectionNames.size()) return "[error]";
			return this->SelectionNames[Index];
		}
	};

	struct CTextInput
	{
	public:
		char* text = nullptr;
		size_t size = 0;
		CTextInput(size_t size, const char* init)
		{
			this->text = new char[size];
			this->size = size;
			if (init)
				strcpy(this->text, init);
		};
	};

	extern bool GetBool(std::string Name);
	extern float GetFloat(std::string Name);
	extern Color GetColor(std::string Name);
	extern size_t GetState(std::string Name);
	extern uint16_t GetSelections(std::string Name);
	extern std::string GetText(std::string Name);

	extern void SetBool(std::string Name, bool Value);


	struct Property {
		void* Value = nullptr;
		void* FreeDefault = nullptr;
		void* PremiumDefault = nullptr;
		PropertyType Type;
		std::string Name;
		std::string VisibleName;
		bool IsPremium;
		int Complexity;

		int Indentation = 0;
		bool HasSeparatorAfter = false;

		// only applies to boolean values
		KeybindType BindType = KeybindType::Toggle;
		int KeyBind = 0;
		KeybindOptions Keybindability{}; // not a word lol

		// only applies to CFloat values
		std::string UnitLabel = "err";


		Property(PropertyType Type, bool IsPremium, int Complexity, std::string Name, std::string VisibleName, void* FreeDefault, void* PremiumDefault)
		{
			this->Type = Type;
			this->Name = Name;
			this->VisibleName = VisibleName;
			this->Value = FreeDefault;
			this->FreeDefault = FreeDefault;
			this->PremiumDefault = PremiumDefault;
			this->Complexity = Complexity;
		}

		std::string Stringify()
		{
			switch (this->Type)
			{
			case PropertyType::COLOR:
				return (*(Color*)this->Value).Stringify();
			case PropertyType::FLOAT:
				return (*(CFloat*)this->Value).Stringify();
			case PropertyType::MULTISELECT:
				return (*(CMultiSelector*)this->Value).Stringify();
			case PropertyType::BOOLEAN:
				return *(bool*)this->Value ? "true" : "false";
			case Config::PropertyType::TEXT:
				return this->VisibleName;
			case Config::PropertyType::TEXTINPUT:
				return std::string(((CTextInput*)this->Value)->text);
			case Config::PropertyType::DROPDOWN:
			{
				CDropdown* v = (CDropdown*)this->Value;
				return v->Stringify(v->GetSelection());
			}
			case Config::PropertyType::EDITGROUP:
			{
				std::string group = ((CEditGroup*)this->Value)->GetCurrent();

				while (group[group.size() - 1] == '-')
					group = group.substr(0, group.size() - 1);

				return group;
			}
			default:
				return "[error]";
			}
		}
	};

	struct Widget {
		int Height = 20; // default, will be changed on first frame rendered
		int Indentation = 0; // pixels 
		std::string Name;
		std::vector<Property*> Properties;
		std::vector<Property*> EditGroups;

		Widget(std::string Name)
		{
			this->Name = Name;
		}

		void SetIndent(int Pixels)
		{
			this->Indentation = Pixels;
		}

		void BeginIndent()
		{
			this->SetIndent(20);
		}

		void EndIndent()
		{
			this->SetIndent(0);
		}
		
		void AddSeparator()
		{
			assert(this->Properties.size() > 0);
			this->Properties.at(this->Properties.size() - 1)->HasSeparatorAfter = true;
		}

		Property* GetEditGroup(Property* p)
		{
			for (size_t i = 0; i < this->EditGroups.size(); i++)
			{
				Property* eg = this->EditGroups.at(i);
				if (p->Name._Starts_with(((CEditGroup*)eg->Value)->Prefix)) return eg;
			}
			return (Property*)0;
		}

		bool IsVisible(Property* p)
		{
			// there are 3 reasons that a property would not be visible:
			//     a. its complexity is too high
			//     b. its part of an edit group, and is not selected
			//     c. its edit group is not visible
			int Complexity = GetState("menu-complexity");

			// a
			if (p->Complexity > Complexity) return false;

			Property* egProp = this->GetEditGroup(p);
			if (!egProp) return true;

			// b
			if (egProp->Complexity > Complexity) return false;

			CEditGroup* eg = (CEditGroup*)egProp->Value;
			std::string currentGroup = eg->GetCurrent();

			// c
			if (currentGroup.length() == 0 || !p->Name._Starts_with(eg->Prefix + currentGroup)) return false;

			return true;
		}

		CEditGroup* AddEditGroup(std::string Prefix, bool IsPremium = false, int Complexity = 0)
		{
			CEditGroup* eg = new CEditGroup(Prefix);
			Property* p = new Property(PropertyType::EDITGROUP, IsPremium, Complexity, "grouped-" + Prefix + "current-shown", "", eg, eg);
			this->Properties.push_back(p);
			this->EditGroups.push_back(p);
			return eg;
		}

		// boolean switches
		Property* AddProperty(bool IsPremium, int Complexity, const char* Name, const char* VisibleName, bool FreeDefault, bool PremiumDefault, KeybindOptions KeybindTypes = KeybindOptions())
		{
			Property* p = new Property(PropertyType::BOOLEAN, IsPremium, Complexity, Name, VisibleName, new bool(FreeDefault), new bool(PremiumDefault));
			
			p->Keybindability = KeybindTypes;
			p->Indentation = this->Indentation;

			this->Properties.push_back(p);
			return p;
		}

		// float values (use decimals = 0 for ints)
		Property* AddProperty(bool IsPremium, int Complexity, const char* Name, const char* VisibleName, const char* Unit, float Min, float Max, int Decimals, float FreeDefault, float PremiumDefault)
		{
			Property* p = new Property(PropertyType::FLOAT, IsPremium, Complexity, Name, VisibleName, new CFloat(Min, Max, FreeDefault, Decimals), new CFloat(Min, Max, PremiumDefault, Decimals));

			p->UnitLabel = Unit;
			p->Indentation = this->Indentation;

			this->Properties.push_back(p);
			return p;
		}

		// text input values
		Property* AddProperty(bool IsPremium, int Complexity, const char* Name, const char* VisibleName, int maxLength, const char* defaultInput)
		{
			CTextInput* free = new CTextInput(maxLength, defaultInput);
			CTextInput* premium = new CTextInput(maxLength, defaultInput);

			Property* p = new Property(PropertyType::TEXTINPUT, IsPremium, Complexity, Name, VisibleName, free, premium);

			p->Indentation = this->Indentation;

			this->Properties.push_back(p);
			return p;
		}

		// color values
		Property* AddProperty(bool IsPremium, int Complexity, const char* Name, const char* VisibleName, Color* FreeDefault, Color* PremiumDefault = nullptr)
		{
			if (!PremiumDefault) PremiumDefault = new Color(*FreeDefault);

			Property* p = new Property(PropertyType::COLOR, IsPremium, Complexity, Name, VisibleName, FreeDefault, PremiumDefault);

			p->Indentation = this->Indentation;

			this->Properties.push_back(p);
			return p;
		}

		// inverter values
		Property* AddProperty(bool IsPremium, int Complexity, const char* Name, const char* VisibleName, CInverter* Inverter = nullptr)
		{
			if (!Inverter) Inverter = new CInverter();

			Property* p = new Property(PropertyType::INVERTER, IsPremium, Complexity, Name, VisibleName, Inverter, Inverter);

			p->Indentation = this->Indentation;

			this->Properties.push_back(p);
			return p;
		}

		// dropdown list values
		Property* AddProperty(bool IsPremium, int Complexity, const char* Name, const char* VisibleName, CDropdown List, bool IsBindable = false, size_t FreeDefault = 0, size_t PremiumDefault = 0)
		{
			List.IsBindable = IsBindable;

			CDropdown* Free = new CDropdown(List);
			CDropdown* Premium = new CDropdown(List);
			Free->Select(FreeDefault);
			Premium->Select(PremiumDefault);

			Property* p = new Property(PropertyType::DROPDOWN, IsPremium, Complexity, Name, VisibleName, Free, Premium);

			p->Indentation = this->Indentation;

			this->Properties.push_back(p);
			return p;
		}

		// multi selectors
		Property* AddProperty(bool IsPremium, int Complexity, const char* Name, const char* VisibleName, CMultiSelector Items)
		{
			CMultiSelector* Free = new CMultiSelector(Items);
			CMultiSelector* Premium = new CMultiSelector(Items);

			Property* p = new Property(PropertyType::MULTISELECT, IsPremium, Complexity, Name, VisibleName, Free, Premium);

			p->Indentation = this->Indentation;

			this->Properties.push_back(p);
			return p;
		}
		Property* AddText(const char* Text, const char* Name = "__text-label-YOU-ARENT-SUPPOSED-TO-SEE-THIS-EVER-LOL-SORRY__")
		{
			Property* p = new Property(PropertyType::TEXT, false, 0, Name, Text, (void*)Text, (void*)Text);

			p->Indentation = this->Indentation;

			this->Properties.push_back(p);
			return p;
		}
	};

	struct Tab {
		std::string Name;
		std::vector<Widget*> Widgets;

		Tab(std::string Name)
		{
			this->Name = Name;
		}

		Widget* AddWidget(std::string Name)
		{
			Widget* w = new Widget(Name);
			this->Widgets.push_back(w);
			return w;
		}
	};

	extern std::vector<Tab*> Tabs;
	extern std::map<std::string, Property*> PropertyLookup;

	extern void Init();

	/* Keybind Stuff */
	extern std::map<int, std::vector<Property*>*> KeybindMap; // virtural keycode: list of properties bound to that keycode
	extern void Bind(Property* Prop, WPARAM KeyCode);
	extern void Unbind(Property* Prop, bool __FORCE = false /* ONLY Config::Bind SHOULD SET ME TO TRUE*/);
	extern void KeyPressed(WPARAM KeyCode);
	extern void KeyReleased(WPARAM KeyCode);
}