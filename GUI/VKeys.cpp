#include "VKeys.hpp"

std::vector<VK::KeyInfo> VK::Keys = {
	VK::KeyInfo{VK_LBUTTON							, "mouse1" },
	VK::KeyInfo{VK_RBUTTON							, "mouse2" },
	VK::KeyInfo{VK_CANCEL							, "cancel" },
	VK::KeyInfo{VK_MBUTTON							, "mouse3" },
	VK::KeyInfo{VK_XBUTTON1					    	, "mouse4" },
	VK::KeyInfo{VK_XBUTTON2					    	, "mouse5" },
	VK::KeyInfo{VK_BACK						    	, "backspace" },
	VK::KeyInfo{VK_TAB								, "tab" },
	VK::KeyInfo{VK_CLEAR							, "clear" },
	VK::KeyInfo{VK_RETURN							, "enter" },
	VK::KeyInfo{VK_SHIFT							, "shift" },
	VK::KeyInfo{VK_CONTROL							, "ctrl" },
	VK::KeyInfo{VK_MENU						    	, "alt" },
	VK::KeyInfo{VK_PAUSE							, "pause_break" },
	VK::KeyInfo{VK_CAPITAL							, "caps_lock" },
	VK::KeyInfo{VK_KANA						    	, "ime_kana" },
	VK::KeyInfo{VK_HANGUL							, "ime_hangul" },
	VK::KeyInfo{VK_JUNJA							, "ime_junja" },
	VK::KeyInfo{VK_FINAL							, "ime_final" },
	VK::KeyInfo{VK_HANJA							, "ime_hanja" },
	VK::KeyInfo{VK_KANJI							, "ime_kanji" },
	VK::KeyInfo{VK_ESCAPE							, "escape" },
	VK::KeyInfo{VK_CONVERT							, "ime_convert" },
	VK::KeyInfo{VK_NONCONVERT						, "ime_nonconvert" },
	VK::KeyInfo{VK_ACCEPT							, "ime_accept" },
	VK::KeyInfo{VK_MODECHANGE						, "ime_modechange" },
	VK::KeyInfo{VK_SPACE							, "space" },
	VK::KeyInfo{VK_PRIOR							, "page_up" },
	VK::KeyInfo{VK_NEXT						    	, "page_down" },
	VK::KeyInfo{VK_END								, "end" },
	VK::KeyInfo{VK_HOME						    	, "home" },
	VK::KeyInfo{VK_LEFT						    	, "left" },
	VK::KeyInfo{VK_UP								, "up" },
	VK::KeyInfo{VK_RIGHT							, "right" },
	VK::KeyInfo{VK_DOWN						    	, "down" },
	VK::KeyInfo{VK_SELECT							, "select" },
	VK::KeyInfo{VK_PRINT							, "print" },
	VK::KeyInfo{VK_EXECUTE							, "execute" },
	VK::KeyInfo{VK_SNAPSHOT					    	, "print_screen" },
	VK::KeyInfo{VK_INSERT							, "insert" },
	VK::KeyInfo{VK_DELETE							, "delete" },
	VK::KeyInfo{VK_HELP						    	, "help" },
	VK::KeyInfo{'0'							    	, "zero" },
	VK::KeyInfo{'1'							    	, "one" },
	VK::KeyInfo{'2'							    	, "two" },
	VK::KeyInfo{'3'							    	, "three" },
	VK::KeyInfo{'4'							    	, "four" },
	VK::KeyInfo{'5'							    	, "five" },
	VK::KeyInfo{'6'							    	, "six" },
	VK::KeyInfo{'7'							    	, "seven" },
	VK::KeyInfo{'8'							    	, "eight" },
	VK::KeyInfo{'9'							    	, "nine" },
	// 'A' -> "a", 'B' -> "b", ...
	VK::KeyInfo{VK_LWIN						    	, "left_win" },
	VK::KeyInfo{VK_RWIN						    	, "right_win" },
	VK::KeyInfo{VK_APPS						    	, "apps" },
	VK::KeyInfo{VK_SLEEP							, "sleep" },
	VK::KeyInfo{VK_NUMPAD0							, "numpad_0" },
	VK::KeyInfo{VK_NUMPAD1							, "numpad_1" },
	VK::KeyInfo{VK_NUMPAD2							, "numpad_2" },
	VK::KeyInfo{VK_NUMPAD3							, "numpad_3" },
	VK::KeyInfo{VK_NUMPAD4							, "numpad_4" },
	VK::KeyInfo{VK_NUMPAD5							, "numpad_5" },
	VK::KeyInfo{VK_NUMPAD6							, "numpad_6" },
	VK::KeyInfo{VK_NUMPAD7							, "numpad_7" },
	VK::KeyInfo{VK_NUMPAD8							, "numpad_8" },
	VK::KeyInfo{VK_NUMPAD9							, "numpad_9" },
	VK::KeyInfo{VK_MULTIPLY					    	, "numpad_mul" },
	VK::KeyInfo{VK_ADD								, "numpad_plus" },
	VK::KeyInfo{VK_SEPARATOR						, "separator" },
	VK::KeyInfo{VK_SUBTRACT					    	, "numpad_sub" },
	VK::KeyInfo{VK_DECIMAL							, "decimal" },
	VK::KeyInfo{VK_DIVIDE							, "numpad_div" },
	VK::KeyInfo{VK_F1								, "f1" },
	VK::KeyInfo{VK_F2								, "f2" },
	VK::KeyInfo{VK_F3								, "f3" },
	VK::KeyInfo{VK_F4								, "f4" },
	VK::KeyInfo{VK_F5								, "f5" },
	VK::KeyInfo{VK_F6								, "f6" },
	VK::KeyInfo{VK_F7								, "f7" },
	VK::KeyInfo{VK_F8								, "f8" },
	VK::KeyInfo{VK_F9								, "f9" },
	VK::KeyInfo{VK_F10								, "f10" },
	VK::KeyInfo{VK_F11								, "f11" },
	VK::KeyInfo{VK_F12								, "f12" },
	VK::KeyInfo{VK_F13								, "f13" },
	VK::KeyInfo{VK_F14								, "f14" },
	VK::KeyInfo{VK_F15								, "f15" },
	VK::KeyInfo{VK_F16								, "f16" },
	VK::KeyInfo{VK_F17								, "f17" },
	VK::KeyInfo{VK_F18								, "f18" },
	VK::KeyInfo{VK_F19								, "f19" },
	VK::KeyInfo{VK_F20								, "f20" },
	VK::KeyInfo{VK_F21								, "f21" },
	VK::KeyInfo{VK_F22								, "f22" },
	VK::KeyInfo{VK_F23								, "f23" },
	VK::KeyInfo{VK_F24								, "f24" },
	VK::KeyInfo{VK_NUMLOCK							, "num_lock" },
	VK::KeyInfo{VK_SCROLL							, "scroll_lock" },
	VK::KeyInfo{VK_OEM_NEC_EQUAL					, "num_equal" },
	VK::KeyInfo{VK_OEM_FJ_JISHO				    	, "fj_dict" },
	VK::KeyInfo{VK_OEM_FJ_MASSHOU					, "fj_unregister" },
	VK::KeyInfo{VK_OEM_FJ_TOUROKU					, "fj_register" },
	VK::KeyInfo{VK_OEM_FJ_LOYA						, "fj_left_oya" },
	VK::KeyInfo{VK_OEM_FJ_ROYA						, "fj_right_oya" },
	VK::KeyInfo{VK_LSHIFT							, "left_shift" },
	VK::KeyInfo{VK_RSHIFT							, "right_shift" },
	VK::KeyInfo{VK_LCONTROL					    	, "left_control" },
	VK::KeyInfo{VK_RCONTROL					    	, "right_control" },
	VK::KeyInfo{VK_LMENU							, "left_menu" },
	VK::KeyInfo{VK_RMENU							, "right_alt" },
	VK::KeyInfo{VK_BROWSER_BACK					    , "browser_back" },
	VK::KeyInfo{VK_BROWSER_FORWARD					, "browser_forward" },
	VK::KeyInfo{VK_BROWSER_REFRESH					, "browser_reload" },
	VK::KeyInfo{VK_BROWSER_STOP				    	, "browser_stop" },
	VK::KeyInfo{VK_BROWSER_SEARCH					, "browser_search" },
	VK::KeyInfo{VK_BROWSER_FAVORITES				, "browser_fav" },
	VK::KeyInfo{VK_BROWSER_HOME				    	, "browser_home" },
	VK::KeyInfo{VK_VOLUME_MUTE						, "vol_mute" },
	VK::KeyInfo{VK_VOLUME_DOWN						, "vol_down" },
	VK::KeyInfo{VK_VOLUME_UP						, "vol_up" },
	VK::KeyInfo{VK_MEDIA_NEXT_TRACK				    , "media_next" },
	VK::KeyInfo{VK_MEDIA_PREV_TRACK				    , "media_prev" },
	VK::KeyInfo{VK_MEDIA_STOP						, "media_stop" },
	VK::KeyInfo{VK_MEDIA_PLAY_PAUSE				    , "media_pause" },
	VK::KeyInfo{VK_LAUNCH_MAIL						, "open_mail" },
	VK::KeyInfo{VK_LAUNCH_MEDIA_SELECT				, "media_select" },
	VK::KeyInfo{VK_LAUNCH_APP1						, "open_app1" },
	VK::KeyInfo{VK_LAUNCH_APP2						, "open_app2" },
	VK::KeyInfo{VK_OEM_1							, "semicolon" },
	VK::KeyInfo{VK_OEM_PLUS						    , "equal" },
	VK::KeyInfo{VK_OEM_COMMA						, "comma" },
	VK::KeyInfo{VK_OEM_MINUS						, "dash" },
	VK::KeyInfo{VK_OEM_PERIOD						, "period" },
	VK::KeyInfo{VK_OEM_2							, "forward_slash" },
	VK::KeyInfo{VK_OEM_3							, "tilde" },
	VK::KeyInfo{VK_GAMEPAD_A						, "game_a" },
	VK::KeyInfo{VK_GAMEPAD_B						, "game_b" },
	VK::KeyInfo{VK_GAMEPAD_X						, "game_x" },
	VK::KeyInfo{VK_GAMEPAD_Y						, "game_y" },
	VK::KeyInfo{VK_GAMEPAD_RIGHT_SHOULDER			, "game_rb" },
	VK::KeyInfo{VK_GAMEPAD_LEFT_SHOULDER			, "game_lb" },
	VK::KeyInfo{VK_GAMEPAD_LEFT_TRIGGER			    , "game_lt" },
	VK::KeyInfo{VK_GAMEPAD_RIGHT_TRIGGER			, "game_rt" },
	VK::KeyInfo{VK_GAMEPAD_DPAD_UP					, "game_dpad_up" },
	VK::KeyInfo{VK_GAMEPAD_DPAD_DOWN				, "game_dpad_down" },
	VK::KeyInfo{VK_GAMEPAD_DPAD_LEFT				, "game_dpad_left" },
	VK::KeyInfo{VK_GAMEPAD_DPAD_RIGHT				, "game_dpad_right" },
	VK::KeyInfo{VK_GAMEPAD_MENU				    	, "game_menu" },
	VK::KeyInfo{VK_GAMEPAD_VIEW				    	, "game_view" },
	VK::KeyInfo{VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON	, "game_ljoy" },
	VK::KeyInfo{VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON	, "game_rjoy" },
	VK::KeyInfo{VK_GAMEPAD_LEFT_THUMBSTICK_UP		, "game_ljoy_up" },
	VK::KeyInfo{VK_GAMEPAD_LEFT_THUMBSTICK_DOWN	    , "game_ljoy_down" },
	VK::KeyInfo{VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT	, "game_ljoy_right" },
	VK::KeyInfo{VK_GAMEPAD_LEFT_THUMBSTICK_LEFT	    , "game_ljoy_left" },
	VK::KeyInfo{VK_GAMEPAD_RIGHT_THUMBSTICK_UP		, "game_rjoy_up" },
	VK::KeyInfo{VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN	, "game_rjoy_down" },
	VK::KeyInfo{VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT	, "game_rjoy_right" },
	VK::KeyInfo{VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT	, "game_rjoy_left" },
	VK::KeyInfo{VK_OEM_4							, "open_brace" },
	VK::KeyInfo{VK_OEM_5							, "back_slash" },
	VK::KeyInfo{VK_OEM_6							, "close_brace" },
	VK::KeyInfo{VK_OEM_7							, "quote" },
	VK::KeyInfo{VK_PROCESSKEY						, "process" },
	VK::KeyInfo{VK_ICO_CLEAR						, "ico_clear" },
};

std::string VK::GetName(int code)
{
	if ('A' <= code && code <= 'Z')
	{
		char lowercase = (char)code - 'A' + 'a';
		return std::string(1, lowercase);
	}

	KeyInfo k;
	for (size_t i = 0; i < Keys.size(); i++)
	{
		k = Keys.at(i);
		if (k.code == code)
			return std::string(k.name);
	}

	// just say key_number at this point
	return "vk_" + std::to_string(code);
}

int VK::GetCode(const char* name)
{
	std::string s(name);

	if (s.size() == 1)
	{
		char c = name[0];
		if ('0' <= c && c <= '9')
			return c;
		if (('a' <= c && c <= 'z'))
			return c + 'A' - 'a';
		if (('A' <= c && c <= 'Z'))
			return c;
	}

	// lowercase
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
	const char* lowername = s.c_str();

	KeyInfo k;
	for (size_t i = 0; i < Keys.size(); i++)
	{
		k = Keys.at(i);
		if (0 == strcmp(lowername, k.name))
		{
			return k.code;
		}
	}

	if (s.rfind("vk_", 0) == 0)
	{
		std::string after = s.substr(3);
		int x = 0;
		try
		{
			return std::stoi(after);
		}
		catch (...) {}
	}
	try
	{
		return std::stoi(s);
	}
	catch (...) {}

	return 0; // unknown
}
