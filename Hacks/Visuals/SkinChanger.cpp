#include "../../Include.hpp"

DWORD FindHudElement(const char* Name)
{
    static void* PHUD = *(void**)(FindPattern("client.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);
    static auto FindHudElement = (DWORD(__thiscall*)(void*, const char*))FindPattern("client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28");
    return FindHudElement(PHUD, Name);
}

void Update()
{
    static auto ForceUpdate = (void(__cdecl*)())FindPattern("engine.dll", "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
    static auto ClearHudWeaponIcon = (int(__thiscall*)(void*, int))FindPattern("client.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C");

    DWORD hudWeaponSelection = FindHudElement("CCSGO_HudWeaponSelection");
    if (hudWeaponSelection)
    {
        DWORD hudWeapons = hudWeaponSelection - 0xA0;
        int* weaponCount = (int*)(hudWeapons + 0x80);
        if (hudWeapons && weaponCount)
            while (*weaponCount)
                ClearHudWeaponIcon((void*)(hudWeapons), *weaponCount - 1);
    }

    ForceUpdate();
}

void SkinChanger::ForceSkin(Entity* Weapon, int PaintKit)
{
    // only update if something has changed
    bool NeedsUpdate =
        *Weapon->GetItemIDHigh() == 0 ||
        *Weapon->GetFallbackPaintKit() != PaintKit ||
        *Weapon->GetFallbackWear() > 0.001f ||
        *Weapon->GetFallbackStatTrak() != -1 ||
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

void SkinChanger::RunFSN()
{
    for (int i = 0; i < I::entitylist->GetHighestEntityIndex(); i++)
    {
        Entity* Weapon = I::entitylist->GetClientEntity(i);
        if (!Weapon) continue;
        if (I::entitylist->GetClientEntityFromHandle(Weapon->GetOwner()) != G::LocalPlayer) continue;
        
        WeaponId id = (WeaponId)Weapon->GetWeaponId();
        int Index = -1; ;
        if ((Index = Skins::WeaponFromId(id)) >= 0)
        {
            // this is a gun
            Skins::PaintKit* PaintKit = Config2::WeaponPaintKits.at(Index)->PaintKit;
            if (PaintKit && PaintKit->ID != 0)
                ForceSkin(Weapon, PaintKit->ID);
            else
                ClearSkin(Weapon);
        }
        else if ((Index = Skins::KnifeFromId(id)) >= 0)
        {
            *Weapon->GetItemDefinitionIndex() = (int)WeaponId::NomadKnife;
            *Weapon->GetModelIndex() = I::modelinfo->GetModelIndex(Skins::GetKnifeModel(Skins::Knife::Nomad));
            ForceSkin(Weapon, 413);
        }
    }

    HANDLE ViewModelHandle = G::LocalPlayer->GetViewModel();
    Entity* ViewModel = I::entitylist->GetClientEntityFromHandle(ViewModelHandle);
    if (ViewModel)
    {
        HANDLE WeaponHandle = ViewModel->GetWeapon();
        Entity* Weapon = I::entitylist->GetClientEntityFromHandle(WeaponHandle);
        if (Weapon && *Weapon->GetModelIndex() != *ViewModel->GetModelIndex())
        {
            *ViewModel->GetModelIndex() = *Weapon->GetModelIndex();
        }
    }
}


