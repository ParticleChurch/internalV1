#pragma once
#include "../Valve/Custom/Color.hpp"
#include <map>
#include <vector>
#include "ConfigManager.hpp"
#include "VKeys.hpp"
#include "SkinConstants.hpp"
#include "animation.hpp"

constexpr bool CONFIG_DEBUG = true;

// trim from left
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

// trim from right
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

// trim from left & right
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
	return ltrim(rtrim(s, t), t);
}

namespace Config {
	namespace UserInfo
	{
		extern bool Authenticated; // is this user logged in at all?
		extern bool Banned; // has this user been banned
		extern bool Premium;
		extern int64_t TimeLeft; // seconds left of their premium subscription
		extern int64_t UserId;
		extern std::string Email;
		extern int64_t AccountAge; // seconds since the account has been created
		extern std::string SessionId;
		extern void Clear();
	}

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
		INVALID,
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

		bool Parse(std::string v)
		{
			try
			{
				float d = std::stof(v);
				this->set(d);
				return true;
			}
			catch (...)
			{
				return false;
			}
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
		bool Parse(std::string v)
		{
			std::transform(v.begin(), v.end(), v.begin(), [](unsigned char c) { return std::tolower(c); });

			std::string a = State0.c_str();
			std::string b = State1.c_str();

			std::transform(a.begin(), a.end(), a.begin(), [](unsigned char c) { return std::tolower(c); });
			std::transform(b.begin(), b.end(), b.begin(), [](unsigned char c) { return std::tolower(c); });

			if (v == a)
				this->State = false;
			else if (v == b)
				this->State = true;
			else if (v == "false" || v == "0")
				this->State = true;
			else if (v == "true" || v == "1")
				this->State = true;
			else
				return this->State = false;
			return true;
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

		bool Parse(std::string v)
		{
			// lowercase
			std::transform(v.begin(), v.end(), v.begin(), [](unsigned char c) { return std::tolower(c); });

			for (size_t i = 0; i < this->Options.size(); i++)
			{
				std::string x = this->Options.at(i);
				std::transform(x.begin(), x.end(), x.begin(), [](unsigned char c) { return std::tolower(c); });
				if (v == x)
				{
					this->Select(i);
					return true;
				}
			}
			return false;
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
			std::string out = "{";
			for (size_t i = 0; i < SelectionNames.size(); i++)
				if (SelectionValues[i])
					out += SelectionNames[i] + ", ";

			// remove ending comma
			if (out.size() >= 3)
				out = out.substr(0, out.size() - 2);

			return out + "}";
		}

		std::string Stringify(size_t Index)
		{
			if (Index >= this->SelectionNames.size()) return "[error]";
			return this->SelectionNames[Index];
		}

		bool Parse(std::string v)
		{
			size_t a = 0, b = 0;

			// remove braces
			a = v.find("{"); if (a == std::string::npos) return false;
			b = v.find("}", a + 1); if (b == std::string::npos) return false;
			v = v.substr(a + 1, b - a - 1);

			// lowercase
			std::transform(v.begin(), v.end(), v.begin(), [](unsigned char c) { return std::tolower(c); });

			// init everything to false
			for (size_t i = 0; i < this->SelectionValues.size(); i++)
				this->SelectionValues[i] = false;

			a = b = 0;
			std::string opt;
			size_t nFailed = 0;
			while (b != std::string::npos)
			{
				b = v.find(',', a);
				opt = v.substr(a, b - a);
				a = b + 1;

				trim(opt);
				if (opt.size() < 1) continue;

				bool found = false;
				for (size_t i = 0; i < this->Count(); i++)
				{
					std::string x = this->SelectionNames.at(i).c_str(); // copy
					std::transform(x.begin(), x.end(), x.begin(), [](unsigned char c) { return std::tolower(c); });

					if (x == opt)
					{
						// reverse failure
						this->SelectionValues[i] = true;
						found = true;
						break;
					}
				}
			}

			return nFailed == (size_t)0;
		}
	};

	struct CTextInput
	{
	public:
		char* text = nullptr;
		size_t size = 0;
		CTextInput(size_t size, const char* init)
		{
			this->text = new char[size+1];
			this->text[size] = 0; // null terminator always
			this->size = size;
			if (init)
				strncpy(this->text, init, size);
		};

		bool Parse(std::string v)
		{
			v = v.substr(0, this->size);
			strcpy(this->text, v.c_str());
			return true;
		}
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
			case Config::PropertyType::INVERTER:
				return (*(CInverter*)this->Value).Stringify();
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


		std::string Description()
		{
			switch (this->Type)
			{
			case PropertyType::COLOR:
				return "(r, g, b)";
			case PropertyType::FLOAT:
			{
				CFloat* f = (CFloat*)this->Value;
				return "number on interval [" + std::to_string(f->minimum) + ", " + std::to_string(f->maximum) + "]";
			}
			case PropertyType::MULTISELECT:
			{
				CMultiSelector* m = (CMultiSelector*)this->Value;
				std::string out = "select multiple: {";
				size_t n = m->Count();
				for (size_t i = 0; i < n; i++)
				{
					out += " " + std::to_string(i) + " = " + m->Stringify(i);
					if (i != n) out += ",";
				}
				return out + " }";
			}
			case PropertyType::BOOLEAN:
				return "true or false";
			case Config::PropertyType::TEXTINPUT:
				return "any text";
			case Config::PropertyType::DROPDOWN:
			{
				CDropdown* v = (CDropdown*)this->Value;
				std::string out = "select single: {";
				size_t n = v->Options.size();
				for (size_t i = 0; i < n; i++)
				{
					out += " " + std::to_string(i) + " = " + v->Stringify(i);
					if (i != n) out += ",";
				}
				return out + " }";
			}
			default:
				return "[error]";
			}
		}

		bool Parse(std::string v)
		{
			switch (this->Type)
			{
			case PropertyType::COLOR:
				return ((Color*)this->Value)->Parse(v);
			case PropertyType::FLOAT:
				return ((CFloat*)this->Value)->Parse(v);
			case PropertyType::MULTISELECT:
				return ((CMultiSelector*)this->Value)->Parse(v);
			case PropertyType::INVERTER:
				return ((CInverter*)this->Value)->Parse(v);
			case PropertyType::BOOLEAN:
			{
				std::transform(v.begin(), v.end(), v.begin(), [](unsigned char c) { return std::tolower(c); });
				if (v == "true" || v == "1" || v == "no")
				{
					*(bool*)this->Value = true;
					return true;
				}
				else if (v == "false" || v == "0" || v == "yes")
				{
					*(bool*)this->Value = false;
					return true;
				}
				else
					return false;
			}
			case Config::PropertyType::TEXTINPUT:
				return ((CTextInput*)this->Value)->Parse(v);
			case Config::PropertyType::DROPDOWN:
				return ((CDropdown*)this->Value)->Parse(v);
			default:
				return false;
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
	extern void ClearAllKeybinds();

	extern std::string ExportToString();
	extern size_t LoadFromString(std::string in);
}

namespace Config2
{
	struct Property;
	struct Group;
	struct Tab;

	extern std::map<std::string, Property*> PropertyTable;
	extern std::vector<Tab*> Tabs;
	extern Property* SettingKeybindFor;

	enum class PropertyType
	{
		BOOLEAN,
		FLOAT,
		PAINTKIT,
		KNIFE,
		GLOVE,
		HSTATEFUL,
		EDITGROUP,
	};
	enum class KeybindMode
	{
		TOGGLE,
		HOLDTOENABLE,
		HOLDTODISABLE,
	};
	extern std::string KeybindTypeNames[];
	struct CBoolean;
	struct CFloat;
	struct CPaintKit;
	struct CKnife;
	struct CGlove;
	struct CHorizontalState;
	struct CEditGroup;

	extern void Init();

	// getters
	extern Property* GetProperty(std::string Name);
	extern int GetKeybind(std::string Name);
	extern bool GetBoolean(std::string Name);
	extern float GetFloat(std::string Name);
	extern int GetPaintKit(std::string Name);
	extern int GetState(std::string Name);

	extern void ProcessKeys();
	extern void Free();
};

namespace UserData
{
	// ping approx. every 60 seconds
	extern double ClientTimestamp; // time according to client of last web ping
	extern double ServerTimestamp; // time according to server of last web ping
	
	extern bool Initialized; // this will be true if the main cheat gui should show up
	extern bool Authenticated; // this will be true only if the user entered a valid email/password combo
	extern std::string Email;
	extern uint64_t UserID;
	extern bool Banned;
	extern bool Premium;
	extern bool PremiumAutoRenews;
	extern double AccountRegistrationTimestamp;

	// these two should be the same if !PremiumAutoRenews
	// if PremiumAutoRenews, then PremiumExpiresTimestamp should be 24 hours after NextBillDueTimestamp
	// this is to give the server 24 hours to process the bill, to ensure no service interruptions
	extern double NextBillDueTimestamp;
	extern double PremiumExpiresTimestamp;
}

namespace Config2
{
	struct CBoolean
	{
		static const PropertyType Type = PropertyType::BOOLEAN;
		TIME_POINT TimeChanged = std::chrono::steady_clock::time_point(std::chrono::seconds(0));

		int BoundToKey = -1;
		KeybindMode BindMode = KeybindMode::TOGGLE;

		bool Value = false;
		std::string Stringify()
		{
			return this->Value ? "true" : "false";
		}

		void Flip()
		{
			this->Value = !this->Value;
			this->TimeChanged = Animation::now();
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

		float Get()
		{
			return this->Value;
		}

		float GetFactor()
		{
			return (this->Value - this->Minimum) / (this->Maximum - this->Minimum);
		}

		void Set(float v)
		{
			v = floorf(v * this->Percision + 0.5f) / (float)this->Percision;
			this->Value = min(max(this->Minimum, v), this->Maximum);
		}

		void SetFactor(float v)
		{
			this->Set(v * (this->Maximum - this->Minimum) + this->Minimum);
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

		bool Parse(std::string v)
		{
			try
			{
				float d = std::stof(v);
				this->Set(d);
				return true;
			}
			catch (...)
			{
				return false;
			}
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

		int LastState = 0;
		size_t State = 0;
		std::vector<std::string> States;
		bool Bindable;
		bool FitToWidth;

		TIME_POINT TimeChanged = std::chrono::steady_clock::time_point(std::chrono::seconds(0));

		CHorizontalState(std::vector<std::string> States, bool Bindable = true, bool FitToWidth = false)
		{
			this->States = States;
			this->Bindable = Bindable;
			this->FitToWidth = FitToWidth;
		}

		void Next()
		{
			if (++this->State >= this->States.size()) this->State = 0;
		}

		bool Set(size_t NewState)
		{
			if (NewState >= this->States.size()) NewState = this->States.size() - 1;
			if (NewState < 0) NewState = 0;
			if (NewState == this->State) return false;

			this->LastState = this->State;
			this->State = NewState;
			this->TimeChanged = Animation::now();

			return true;
		}
	};
	struct CEditGroup
	{
		static const PropertyType Type = PropertyType::EDITGROUP;

	private:
		std::vector<std::string> Titles;
		std::vector<std::vector<Property*>*> Properties;
		size_t SelectedIndex = 0;

	public:
		CEditGroup() {}
		void Add(std::string Title, Property* Prop)
		{
			std::vector<Property*>* List = nullptr;
			for (size_t i = 0; i < this->Titles.size(); i++)
			{
				if (this->Titles.at(i) == Title)
				{
					List = this->Properties.at(i);
					break;
				}
			}

			if (!List)
			{
				List = new std::vector<Property*>{};

				this->Titles.push_back(Title);
				this->Properties.push_back(List);
			}

			List->push_back(Prop);
		}
		std::vector<Property*>* GetVisibleProperties()
		{
			if (this->SelectedIndex >= this->Properties.size()) return nullptr;
			return this->Properties.at(this->SelectedIndex);
		}
		void SelectIndex(size_t i)
		{
			if (this->Properties.size() == 0)
				this->SelectedIndex = 0;
			else
				this->SelectedIndex = max(min(i, this->Properties.size() - 1), 0);
		}
		size_t GetSelectedIndex()
		{
			return this->SelectedIndex;
		}
		~CEditGroup()
		{
			for (size_t i = 0; i < this->Properties.size(); i++)
			{
				delete this->Properties[i];
			}
		}
	};

	struct Property
	{
	public:
		bool (*IsVisible)() = nullptr;
		Property* Master = nullptr;

		// meta info
		bool IsComplex = false;
		bool IsPremium = false;

		void* Value;
		PropertyType Type;

		std::string Name;
		std::string VisibleName;

		template <typename T>
		Property(std::string Name, std::string VisibleName, T* Value)
		{
			this->Name = Name;
			this->VisibleName = VisibleName;
			this->Type = Value->Type;
			this->Value = (void*)Value;

			PropertyTable.insert(std::make_pair(this->Name, this));
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
			case PropertyType::EDITGROUP:
				delete (CEditGroup*)this->Value;
				break;
			}
		}
	};
	struct Group
	{
	private:
		float LastDrawHeight = -1.f;

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
			this->Properties.push_back(x);
			return x;
		}

		float GetDrawHeight()
		{
			// return actual height
			if (this->LastDrawHeight >= 0.f)
				return this->LastDrawHeight;
			
			// try to calculate it
			float h = this->ShowTitle ? 5.f + 18.f : 0.f;
			for (size_t p = 0; p < this->Properties.size(); p++)
			{
				auto Property = this->Properties[p];
				if (Property->IsVisible && !Property->IsVisible()) continue;
				
				h += this->Padding;
				switch (Property->Type)
				{
				case Config2::PropertyType::BOOLEAN:
					h += 20;
					break;
				default:
					h += 20;
					break;
				}
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