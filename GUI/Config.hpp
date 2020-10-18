#pragma once
#include "../Valve/Custom/Color.hpp"
#include <map>
#include <vector>
//#include "../Include.hpp"

/*
	Web API shit
*/
#define LOGIN_FLAG_LOGIN_VALID              1 << 0  // was the sent login valid
#define LOGIN_FLAG_ACCOUNT_PAID             1 << 1  // is the account paid or free
#define LOGIN_FLAG_ACCOUNT_ALREADY_IN_USE   1 << 2  // is the cheat being used elsewhere (this is someone trying to share an account with a friend)
#define LOGIN_FLAG_ACCOUNT_IS_DEVELOPER     1 << 3  // is this login the dev@a4g4.com account
#define LOGIN_FLAG_BANNED                   1 << 4  // is this account banned (do something like: print "fuck you" then crash csgo, lol)

#define AUTH_STATUS_NONE                    0 // not logged in
#define AUTH_STATUS_PROCESSING              1 // login is processing
#define AUTH_STATUS_COMPLETE                2 // logged in

struct APIResponseFormat_LoginAttempt
{
	int Flags;
	unsigned int UserID;
	//uint8_t bytes[8];
};


/*
	Actual user config
*/
struct UserInfoStruct {
	char AuthStatus = AUTH_STATUS_NONE;
	bool Paid = false;
	bool Developer = false;
	int UserID = INT_MAX;
	std::string Email = "";
};

/*
Legitbot
*/
struct Config_LegitBot
{
	//General
	bool Enable = false;	//enable LegitTab
	bool EnableAim = false;	//enable aimbot
	int AimKey = 0;			//for onkey aimbot
	bool TeamCheck = false;	//dont shoot teamates

	//For Specific Weapons
	//pistol smg heavy rifle sniper
	struct WeaponChoice
	{
		//Aimbot
		int Fov = 0;				//FOV of aimbot
		int YawSpeed = 0;			//Aimbot Yaw speed
		int PitchSpeed = 0;			//Aimbot Pitch speed
		bool RecoilControl = false;	//recoil control

		//target
		std::vector<int> Hitboxes;					//which hitboxes to target
		bool HSelected[6];							//which options were selected in gui for hitboxes
		std::vector<std::string> LHitboxes;			//which options are available after hitboxes selected in gui
		std::vector<int> Priority;					//which of those hitboxes to prioritize
	}weapon[5];

	//Backtracking
	bool EnableBacktracking = false;	//if backtracking is enabled
	int BacktrackingTicks = 0;			//number of backtracking ticks

	//Triggerbot
	bool EnableTriggerbot = false;		//if triggerbot is enabled
	int TriggerbotKey = 0;				//triggerbot on key
	int Delay = 0;						//time between seeing enemy and pulling trigger
	int TriggerHitchance = 80;			//triggerbot hitchance (0-100)
};

/*
Aimbot - going off of https://cdn.discordapp.com/attachments/624039569268146187/739658790626000966/Capture.PNG
*/
struct Config_RageBot {
	//General
	bool Enable = false;	//enable RageTab
	bool EnableAim = false;	//enable aimbot
	int AimKey = 0;			//for onkey aimbot
	bool AutoShoot = false;	//for automatic aimbot
	bool SilentAim = false;	//silent aim
	bool TeamCheck = false;	//dont shoot teamate
	bool AimStep = false;	//move in increments to not get banned on casual servers
	bool AutoScope = false;	//autoscope... think about it

	//For Specific Weapons
	struct WeaponChoice
	{
		//target
		std::vector<int> Hitboxes;	//which hitboxes to target
		bool HSelected[6];							//which options were selected in gui for hitboxes
		std::vector<std::string> LHitboxes;			//which options are available after hitboxes selected in gui
		std::vector<int> Priority;	//which of those hitboxes to prioritize
		//damage
		int MinDamageVisible = 0;	//min damage to hit while player visible
		int MinDamageAutowall = 0;	//min damage to hit player while through walls
		bool BaimOnLethal = false;	//Body aim if lethal
		int OverideKey = 0;			//key to set damage to another defiend damage
		int OverideDamage = 0;		//reset damage on key
		//misc
		bool AutoSlow = false;		//if see enemy --> slow to good speed
		int ShotDelay = 0;			//delay between seeing target and shooting
		int HitChance = 100;		//radius away from center of hitbox...
		
	}weapon[7];
};

/*
Antiaim
*/
struct Config_AntiAim {
	bool Enable = false;
	//Only display these 3 next options if one of these: (Fakelag || Legit || Rage) is enabled
	bool Visualize = false;	//if the fake angle is visualized when in third person
	Color Fake;				//Color of fake when in third person
	int Opacity = 100;		//how opaque the fake player is (0 is invisible)

	//Fakelag
	struct FakeLag 
	{
		bool Enable = false;	//if fake lag is enabled
		int FakeLagTicks = 0;	//number of lagging ticks (note, if aa on, this is autoset to 1 (cuz desync))
		bool Adaptive = false;	//lags only the amount needed (not too much lag) when moving (4096 max lag distance)
		bool LagOnPeak = false; //lags when peaking (might be very useful)
	}FakeLag;
	//Legit
	struct Legit //(desync)
	{
		bool Enable = false;	//if legit AA is enabled
		int InverterKey = 0;	//the key to switch real from one side to another
	}Legit;
	//Rage
	struct Rage
	{
		bool Enable = false;		//if Rage AA is enabled
		int RealJitterAmount = 0;	//Amount of jitter added to AA
		int LeftKey = 0;			//Manual Left AA Key
		int BackKey = 0;			//Manual Back AA Key
		int RightKey = 0;			//Manual Right AA Key
		bool AddFake = false;		//Add fake angles to AA
		int FakeJitterAmount = 0;	//Amount of fake jitter added to AA (using fake angles)
	}Rage;
};

/*
Movement
*/
struct Config_Movement {
	//General
	bool Enable = false;			//enable movement hacks
	bool BunnyHop = false;			//enable bunnyhop
	bool FastCrouch = false;		//enables faster crouching
	bool LegitAutoStrafe = false;	//enable legit autostrafe
	bool RageAutoStrafe = false;	//enable rage autostrafe
	bool SlowWalk = false;			//enable slow walk
	int SlowWalkKey = 0;			//Slow walk key
	bool SlideWalk = false;			//if the player slides around...
	//fakeduck - havent implemented but will be here
	//edgejump - havent implemented but will be here

	//Troll - may or may not add these....
	//bool AutoBlock = false;			//stands in fron of someone autoblock
	//bool StandOnHeadBlock = false;	//stands on head autoblock
};

/*
Visuals
*/
struct Config_Visuals {
	bool Enable = false;
	//General
	bool RecoilCrosshair = false;	//enables a visible recoil crosshair
	bool ThirdPerson = false;		//enables third person
	int ThirdPersonKey = 0;			//thirdperson key

	//World
	bool AsusWalls = false;		//enables see through walls
	int AsusWallVal = 100;		//how see through walls are
	bool AsusProps = false;		//enable see through props
	int AsusPropVal = 100;		//how see through props are

	//Removals
	bool NoFlash = false;	//if Flashes are hidden
	bool NoSmoke = false;	//if smokes are hidden
	bool NoScope = false;	//if weapon scopes are hidden


	//Players - esp								//note players just mean enemies
	bool Radar = false;							//displays enemy location on the radar
	bool Boxes = false;							//if boxes around players are displayed
	Color BoxColor;								//color of box
	bool Skeleton = false;						//player skeleton
	bool Name = false;							//if names are displayed
	bool Health = false;						//if health is displayed
	bool Ammo = false;							//if ammo displayed
	

	//Players - chams
	bool VisibleChams = false;		//enables visible chams
	Color VisibleColor;				//visible chams color
	bool ThroughWallChams = false;	//enables through wall chams
	Color ThroughWallColor;			//through wall chams color
};

namespace Config {
	extern UserInfoStruct UserInfo;

	extern Config_LegitBot legitbot;
	extern Config_RageBot ragebot;
	extern Config_AntiAim antiaim;
	extern Config_Movement movement;
	extern Config_Visuals visuals;

	extern unsigned int RaindropCount;
}

	namespace Config {
	enum class PropertyType {
		COLOR = 0,
		FLOAT,
		BOOLEAN,
		_count
	};

	struct CFloat
	{
	private:
		float value;
	public:
		float minimum, maximum, percision;
		CFloat(float min, float max, float value, float percision = 0.01)
		{
			this->minimum = min;
			this->maximum = max;
			this->percision = percision;
			this->value = value;
		}

		void set(float v)
		{
			v = floor(v / this->percision + 0.5f) * this->percision;
			this->value = min(max(this->minimum, v), this->maximum);
		}

		float get()
		{
			return this->value;
		}

		std::string Stringify()
		{
			return "0";
		}
	};

	struct Property {
		void* Value = nullptr;
		PropertyType Type;
		std::string Name;
		std::string VisibleName;
		std::string Description;
		bool IsPremium;

		Property(bool IsPremium, PropertyType Type, std::string Name, std::string VisibleName, std::string Description, void* Default)
		{
			this->Type = Type;
			this->Name = Name;
			this->VisibleName = VisibleName;
			this->Description = Description;
			this->Value = Default;
			this->IsPremium = IsPremium;
		}

		std::string Stringify()
		{
			switch (this->Type)
			{
			case PropertyType::COLOR:
				return (*(Color*)this->Value).Stringify();
			case PropertyType::FLOAT:
				return (*(CFloat*)this->Value).Stringify();
			case PropertyType::BOOLEAN:
				return *(bool*)this->Value ? "true" : "false";
			default:
				return "[error]";
			}
		}
	};

	struct Widget {
		int Height = 20;
		std::string Name;
		std::vector<Property*> Properties;

		Widget(std::string Name)
		{
			this->Name = Name;
		}

		Property* AddProperty(bool IsPremium, std::string Name, std::string VisibleName, std::string Description, bool* Default)
		{
			Property* p = new Property(IsPremium, PropertyType::BOOLEAN, Name, VisibleName, Description, Default);
			this->Properties.push_back(p);
			return p;
		}
		Property* AddProperty(bool IsPremium, std::string Name, std::string VisibleName, std::string Description, Color* Default)
		{
			Property* p = new Property(IsPremium, PropertyType::COLOR, Name, VisibleName, Description, Default);
			this->Properties.push_back(p);
			return p;
		}
		Property* AddProperty(bool IsPremium, std::string Name, std::string VisibleName, std::string Description, Config::CFloat* Default)
		{
			Property* p = new Property(IsPremium, PropertyType::FLOAT, Name, VisibleName, Description, Default);
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

	extern bool GetBool(std::string Name);
	extern float GetFloat(std::string Name);
	extern Color GetColor(std::string Name);

	extern void SetBool(std::string Name, bool Value);
	extern void SetFloat(std::string Name, float Value);
	extern void SetColor(std::string Name, Color Value);
}