#include "../../Include.hpp"

SkinChanger* skinchanger = new SkinChanger();

void SkinChanger::ForceSkin(Entity* weap, int paintkit)
{
    if (paintkit == 0)
        return;

    *weap->GetFallbackPaintKit() = paintkit;
    *weap->GetEntityQuality() = 0;
    *weap->GetFallbackSeed() = 0;
    *weap->GetFallbackStatTrak() = -1;
    *weap->GetFallbackWear() = 0;
    *weap->GetItemIDHigh() = -1;
}

void Update()
{
    using ForceUpdateFn = void(__cdecl*)();
    static auto ForceUpdate = (ForceUpdateFn)FindPattern("engine.dll", "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
    ForceUpdate();
}

void SkinChanger::run(int stage)
{
    static int lastWeapon = -1;
    static int lastPaintKit = -1;
    if (stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START) return;

    Entity* weapon = G::LocalPlayer->GetActiveWeapon();
    if (!weapon) return;

    int wp = Skins::LookupWeaponId((WeaponId_)weapon->GetWeaponId());
    if (wp < 0) return;

    std::string name = "skinchanger-weapon-" + TextService::RemoveWhitespace(TextService::ToLowercase(Skins::WeaponNames[wp]));
    int pk = Config2::GetPaintKit(name)->PaintKit->ID;
    if (pk == -1) pk = 0;

    ForceSkin(weapon, pk);
    if (pk == lastPaintKit && wp == lastWeapon) return;

    Update();
    lastPaintKit = pk;
    lastWeapon = wp;
}

