#include "../../Include.hpp"

KillSay* killsay = new KillSay();

void KillSay::run(GameEvent* event)
{
	if (!Config::GetBool("misc-other-killsay"))
		return;

	if (StrHash::HashRuntime(event->GetName()) == StrHash::Hash("player_death"))
	{
		/*
		player_death
		Note: When a client dies

		Name:	player_death
		Structure:
		short	userid	user ID who died
		short	attacker	user ID who killed
		short	assister	user ID who assisted in the kill
		bool	assistedflash	assister helped with a flash
		string	weapon	weapon name killer used
		string	weapon_itemid	inventory item id of weapon killer used
		string	weapon_fauxitemid	faux item id of weapon killer used
		string	weapon_originalowner_xuid
		bool	headshot	singals a headshot
		short	dominated	did killer dominate victim with this kill
		short	revenge	did killer get revenge on victim with this kill
		short	wipe	To do: check if indicates on a squad wipeout in Danger Zone
		short	penetrated	number of objects shot penetrated before killing target
		bool	noreplay	if replay data is unavailable, this will be present and set to false
		bool	noscope	kill happened without a scope, used for death notice icon
		bool	thrusmoke	hitscan weapon went through smoke grenade
		bool	attackerblind	attacker was blind from flashbang
		*/
		int attacker = I::engine->GetPlayerForUserID(event->GetInt("attacker"));
		int victim = I::engine->GetPlayerForUserID(event->GetInt("userid"));

		// if the attacker isn't the local player, or if the person dying is localplayer, return
		if (attacker != G::LocalPlayerIndex ||
			victim == G::LocalPlayerIndex)
			return;

		
		player_info_t info;
		if (!I::engine->GetPlayerInfo(victim, &info))
			return;

		// string parse p
		std::string line = ("say " + Config::GetText("misc-other-killsay-input"));
		size_t start_pos = line.find("%p");
		if (start_pos == std::string::npos)
			return;
		line.replace(start_pos, 2, info.name);

		I::engine->ClientCmd_Unrestricted(line.c_str()); //kill say
	}
}
