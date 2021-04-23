#include "../../Include.hpp"

Clantag* clantag = new Clantag();

void Clantag::run()
{
	static Config2::CState* Enable = Config2::GetState("misc-other-clantag");
	static Config2::CTextInput* Text = Config2::GetText("misc-other-clantag-input");
	static Config2::CState* Special = Config2::GetState("clantag-special");

	static std::add_pointer_t<void __fastcall(const char*, const char*)> setClanTag = reinterpret_cast<decltype(setClanTag)>(FindPattern("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));

	// If doing special clantag
	if (Special->Get())
	{
		// 0.6 second update timer on clantag update
		static float LastUpdatedTime = I::globalvars->m_curTime;
		if (fabsf(I::globalvars->m_curTime - LastUpdatedTime) < 0.2f)
			return;

		LastUpdatedTime = I::globalvars->m_curTime;

		static int value = 1;

		switch (value)
		{
		case 1:
			setClanTag("", "");
			break;
		case 2:
			setClanTag("A", "A");
			break;
		case 3:
			setClanTag("A4", "A4");
			break;
		case 4:
			setClanTag("A4G", "A4G");
			break;
		case 5:
			setClanTag("A4G4", "A4G4");
			break;
		case 6:
			setClanTag("A4G4.", "A4G4.");
			break;
		case 7:
			setClanTag("A4G4.C", "A4G4.C");
			break;
		case 8:
			setClanTag("A4G4.CO", "A4G4.CO");
			break;
		case 9:
			setClanTag("A4G4.COM", "A4G4.COM");
			break;
		case 10:
			setClanTag("4G4.COM", "4G4.COM");
			break;
		case 11:
			setClanTag("G4.COM", "G4.COM");
			break;
		case 12:
			setClanTag("4.COM", "4.COM");
			break;
		case 13:
			setClanTag(".COM", ".COM");
			break;
		case 14:
			setClanTag("COM", "COM");
			break;
		case 15:
			setClanTag("OM", "OM");
			break;
		case 16:
			setClanTag("M", "M");
			value = 0;
			break;
		}
		value++;

		return;
	}

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
