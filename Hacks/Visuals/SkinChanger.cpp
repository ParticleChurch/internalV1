#include "../../Include.hpp"

void Update()
{
    static auto ForceUpdate = (void(__cdecl*)())FindPattern("engine.dll", "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
    ForceUpdate();
    // the same problems occur when using delta tick instead
    // I::clientstate->m_delta_tick = -1;
}

void SkinChanger::ForceSkin(Entity* Weapon, int PaintKit)
{

    bool NeedsUpdate =
        *Weapon->GetItemIDHigh() == 0 ||
        *Weapon->GetFallbackPaintKit() != PaintKit ||
        *Weapon->GetFallbackWear() > 0.001f ||
        *Weapon->GetFallbackStatTrak() >= 0 ||
        *Weapon->GetFallbackOriginalOwnerXuidLow() != 0 ||
        *Weapon->GetFallbackOriginalOwnerXuidHigh() != 0 ||
        strcmp(Weapon->GetCustomName(), "www.a4g4.com");
    if (!NeedsUpdate) return;

    *Weapon->GetFallbackStatTrak() = -1;
    *Weapon->GetFallbackPaintKit() = PaintKit;
    *Weapon->GetFallbackOriginalOwnerXuidLow() = 0;
    *Weapon->GetFallbackOriginalOwnerXuidHigh() = 0;
    *Weapon->GetFallbackWear() = 0.0001f;
    memcpy(Weapon->GetCustomName(), "www.a4g4.com", sizeof("www.a4g4.com"));
    *Weapon->GetItemIDHigh() = -1;
    Update();
}

void SkinChanger::ClearSkin(Entity* Weapon)
{
    if (*Weapon->GetItemIDHigh() == 0) return; // has no skin

    *Weapon->GetItemIDHigh() = 0;
    *Weapon->GetFallbackStatTrak() = -1;
    *Weapon->GetFallbackPaintKit() = 0;
    *Weapon->GetFallbackWear() = 0.f;
    ZeroMemory(Weapon->GetCustomName(), 32);
    Update();
}

void SkinChanger::FSNStart()
{
    for (int i = 0; i < I::entitylist->GetHighestEntityIndex(); i++)
    {
        Entity* Weapon = I::entitylist->GetClientEntity(i);
        if (!Weapon) continue;
        if (I::entitylist->GetClientEntityFromHandle(Weapon->GetOwner()) != G::LocalPlayer) continue;
        
        WeaponId_ WeaponId = (WeaponId_)Weapon->GetWeaponId();
        int WeaponIndex = Skins::WeaponFromId(WeaponId);
        if (WeaponIndex < 0) continue; // this is not a weapon

        std::string ConfigName = "skinchanger-weapon-" + TextService::RemoveWhitespace(TextService::ToLowercase(Skins::WeaponNames[WeaponIndex]));
        Skins::PaintKit* PaintKit = Config2::GetPaintKit(ConfigName)->PaintKit;
        if (PaintKit && PaintKit->ID != 0)
            ForceSkin(Weapon, PaintKit->ID);
        else
            ClearSkin(Weapon);
    }
}


