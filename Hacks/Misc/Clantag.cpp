#include "../../Include.hpp"

Clantag* clantag = new Clantag();

void Clantag::run()
{
	static Config2::CState* Enable = Config2::GetState("misc-other-clantag");
	static Config2::CTextInput* Text = Config2::GetText("misc-other-clantag-input");

	// 0.6 second update timer on clantag update
	static float LastUpdatedTime = I::globalvars->m_curTime;
	if (fabsf(I::globalvars->m_curTime - LastUpdatedTime) < 0.6f)
		return;

	LastUpdatedTime = I::globalvars->m_curTime;

	/*std::string clantag = Config::GetText("misc-other-clantag-input");
	static int brugh = 0;
	brugh++;
	if (brugh > clantag.length()) brugh = 0;
	std::rotate(clantag.begin(), clantag.begin() + brugh, clantag.end());*/

	static std::add_pointer_t<void __fastcall(const char*, const char*)> setClanTag = reinterpret_cast<decltype(setClanTag)>(FindPattern("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));

	if (Enable->Get())
		setClanTag(Text->Get().c_str(), Text->Get().c_str());
	else
		setClanTag("", "");
}

void Clantag::reset()
{
	static std::add_pointer_t<void __fastcall(const char*, const char*)> setClanTag = reinterpret_cast<decltype(setClanTag)>(FindPattern("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));
	setClanTag("", "");
}
