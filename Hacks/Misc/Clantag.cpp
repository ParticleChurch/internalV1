#include "../../Include.hpp"

Clantag* clantag = new Clantag();

void Clantag::run()
{
	// 0.6 second update timer on clantag update
	static float LastUpdatedTime = I::globalvars->m_curTime;
	if (fabsf(I::globalvars->m_curTime - LastUpdatedTime) < 0.6f)
		return;

	LastUpdatedTime = I::globalvars->m_curTime;

	static std::add_pointer_t<void __fastcall(const char*, const char*)> setClanTag = reinterpret_cast<decltype(setClanTag)>(FindPattern("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));
	if (Config::UserInfo::Premium)
		setClanTag(Config::GetText("misc-other-clantag-input").c_str(), Config::GetText("misc-other-clantag-input").c_str());
	else
		setClanTag("a4g4.com", "a4g4.com");
}
