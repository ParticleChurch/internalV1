#include "../../Include.hpp"

void Update()
{
    static auto ForceUpdate = (void(__cdecl*)())FindPattern("engine.dll", "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
    //ForceUpdate();
    I::clientstate->m_delta_tick = -1;
}

int TransformSequence(int OriginalSequence, Skins::Knife OriginalKnife, Skins::Knife NewKnife)
{
    // get a generic sequence to represent the item
    Skins::Sequence::Generic GeneralSequence = Skins::Sequence::Generic::Unknown;
    switch (OriginalKnife)
    {
    default:
        return OriginalSequence;
    case Skins::Knife::Nomad         : GeneralSequence = Skins::Sequence::Nomad         [OriginalSequence]; break;
    case Skins::Knife::Skeleton      : GeneralSequence = Skins::Sequence::Skeleton      [OriginalSequence]; break;
    case Skins::Knife::Survival      : GeneralSequence = Skins::Sequence::Survival      [OriginalSequence]; break;
    case Skins::Knife::Paracord      : GeneralSequence = Skins::Sequence::Paracord      [OriginalSequence]; break;
    case Skins::Knife::Classic       : GeneralSequence = Skins::Sequence::Classic       [OriginalSequence]; break;
    case Skins::Knife::Bowie         : GeneralSequence = Skins::Sequence::Bowie         [OriginalSequence]; break;
    case Skins::Knife::Butterfly     : GeneralSequence = Skins::Sequence::Butterfly     [OriginalSequence]; break;
    case Skins::Knife::Falchion      : GeneralSequence = Skins::Sequence::Falchion      [OriginalSequence]; break;
    case Skins::Knife::Flip          : GeneralSequence = Skins::Sequence::Flip          [OriginalSequence]; break;
    case Skins::Knife::Gut           : GeneralSequence = Skins::Sequence::Gut           [OriginalSequence]; break;
    case Skins::Knife::Huntsman      : GeneralSequence = Skins::Sequence::Huntsman      [OriginalSequence]; break;
    case Skins::Knife::Karambit      : GeneralSequence = Skins::Sequence::Karambit      [OriginalSequence]; break;
    case Skins::Knife::Bayonet       : GeneralSequence = Skins::Sequence::Bayonet       [OriginalSequence]; break;
    case Skins::Knife::M9Bayonet     : GeneralSequence = Skins::Sequence::M9Bayonet     [OriginalSequence]; break;
    case Skins::Knife::Navaja        : GeneralSequence = Skins::Sequence::Navaja        [OriginalSequence]; break;
    case Skins::Knife::ShadowDaggers : GeneralSequence = Skins::Sequence::ShadowDaggers [OriginalSequence]; break;
    case Skins::Knife::Stiletto      : GeneralSequence = Skins::Sequence::Stiletto      [OriginalSequence]; break;
    case Skins::Knife::Talon         : GeneralSequence = Skins::Sequence::Talon         [OriginalSequence]; break;
    case Skins::Knife::Ursus         : GeneralSequence = Skins::Sequence::Ursus         [OriginalSequence]; break;
    case Skins::Knife::TDefault      : GeneralSequence = Skins::Sequence::TDefault      [OriginalSequence]; break;
    case Skins::Knife::CTDefault     : GeneralSequence = Skins::Sequence::CTDefault     [OriginalSequence]; break;
    }

    int SequenceGroup = -1;
    switch (NewKnife)
    {
    default:
        return OriginalSequence;
    case Skins::Knife::Nomad:
    case Skins::Knife::Skeleton:
    case Skins::Knife::Survival:
    case Skins::Knife::Paracord:
    case Skins::Knife::Ursus:
        SequenceGroup = 1;
        break;
    case Skins::Knife::Classic:
        SequenceGroup = 2;
        break;
    case Skins::Knife::Bowie:
        SequenceGroup = 3;
        break;
    case Skins::Knife::Butterfly:
        SequenceGroup = 4;
        break;
    case Skins::Knife::Falchion:
        SequenceGroup = 5;
        break;
    case Skins::Knife::Flip:
    case Skins::Knife::Gut:
    case Skins::Knife::Huntsman:
    case Skins::Knife::Karambit:
    case Skins::Knife::Bayonet:
    case Skins::Knife::M9Bayonet:
    case Skins::Knife::Navaja:
    case Skins::Knife::TDefault:
    case Skins::Knife::CTDefault:
        SequenceGroup = 6;
        break;
    case Skins::Knife::ShadowDaggers:
        SequenceGroup = 7;
        break;
    case Skins::Knife::Stiletto:
        SequenceGroup = 8;
        break;
    case Skins::Knife::Talon:
        SequenceGroup = 9;
        break;
    }

    int output = Skins::Sequence::GetSequenceFromGeneric(GeneralSequence, SequenceGroup);
    if (output < 0) return OriginalSequence;
    return output;
}

RecvVarProxyFn SkinChanger::oModelIndexProxy = nullptr;
RecvVarProxyFn SkinChanger::oSequenceProxy = nullptr;
void ModelIndexProxyHook(const CRecvProxyData* pData_const, void* pStruct, void* pOut)
{
    if (G::LocalPlayer && G::LocalPlayerAlive)
    {

    }
    SkinChanger::oModelIndexProxy(pData_const, pStruct, pOut);
}
void SequenceProxyHook(const CRecvProxyData* pData_const, void* pStruct, void* pOut)
{
    static int LastOriginalSequence = 0;
    static int LastAdjustedSequence = 0;

    CRecvProxyData* pData = const_cast<CRecvProxyData*>(pData_const);

    // there must be a viewmodel and we must be alive
    Entity* ViewModel = (Entity*)pStruct;
    if (ViewModel && G::LocalPlayer && G::LocalPlayerAlive)
    {
        // the viewmodel must belong to us (not a spectator)
        HANDLE ViewModelHandle = G::LocalPlayer->GetViewModel();
        Entity* LocalViewModel = I::entitylist->GetClientEntityFromHandle(ViewModelHandle);
        if (LocalViewModel == ViewModel)
        {
            // the viewmodel must be a knife
            HANDLE WeaponHandle = ViewModel->GetWeapon();
            Entity* Weapon = I::entitylist->GetClientEntityFromHandle(WeaponHandle);
            if (Weapon && Skins::KnifeFromId(Weapon->GetWeaponId()) != Skins::Knife::INVALID)
            {
                // if the original sequence hasn't changed
                if (LastOriginalSequence == pData->m_Value.m_Int)
                {
                    pData->m_Value.m_Int = LastAdjustedSequence;
                }
                else // otherwise, generate new value
                {
                    LastOriginalSequence = pData->m_Value.m_Int;

                    if (SkinChanger::OriginalKnife != SkinChanger::AppliedKnife) // only need to make a change if different
                    {
                        pData->m_Value.m_Int = TransformSequence(
                            pData->m_Value.m_Int,
                            Skins::KnifeFromId(SkinChanger::OriginalKnife),
                            Skins::KnifeFromId(SkinChanger::AppliedKnife)
                        );
                    }

                    LastAdjustedSequence = pData->m_Value.m_Int;
                }
            }
        }
    }
    SkinChanger::oSequenceProxy(pData_const, pStruct, pOut);
}

RecvVarProxyFn oTestProxy = nullptr;
void TestProxyHook(const CRecvProxyData* pData_const, void* pStruct, void* pOut)
{
    CRecvProxyData* pData = const_cast<CRecvProxyData*>(pData_const);

    Entity* Player = (Entity*)pStruct;
    L::Info("origin changed to", " - ");
    L::SameLine(std::to_string(pData_const->m_Value.m_Vector[0]).c_str(), ", ");
    L::SameLine(std::to_string(pData_const->m_Value.m_Vector[1]).c_str(), ", ");
    L::SameLine(std::to_string(pData_const->m_Value.m_Vector[2]).c_str());

    if (oTestProxy && GetKeyState('T') >= 0)
        oTestProxy(pData_const, pStruct, pOut);
}

void SkinChanger::Hook()
{
    for (ClientClass* pClass = I::client->GetAllClasses(); pClass; pClass = pClass->m_pNext)
    {
        if (!strcmp(pClass->m_pNetworkName, "CBaseViewModel"))
        {
            for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
            {
                RecvProp* pProp = &(pClass->m_pRecvTable->m_pProps[i]);
                if (!pProp) continue;
                
                if (!strcmp(pProp->m_pVarName, "m_nModelIndex"))
                {
                    oModelIndexProxy = (RecvVarProxyFn)pProp->m_ProxyFn;
                    pProp->m_ProxyFn = ModelIndexProxyHook;
                }
                else if (!strcmp(pProp->m_pVarName, "m_nSequence"))
                {
                    oSequenceProxy = pProp->m_ProxyFn;
                    pProp->m_ProxyFn = SequenceProxyHook;
                }
            }
        }
        else if (!strcmp(pClass->m_pNetworkName, "CEconWearable"))
        {
            CreateWearable = pClass->m_pCreateFn;
        }
    }

    //m_vecOrigin
    //{
    //    RecvProp* prop = N::FindRecvPropByPath({ "DT_CSPlayer", "DT_CSNonLocalPlayerExclusive", "m_vecOrigin" });
    //    oTestProxy = (RecvVarProxyFn)prop->m_ProxyFn;
    //    prop->m_ProxyFn = TestProxyHook;
    //}
}

void SkinChanger::UnHook()
{
    // first, clear all skins + knife
    for (size_t i = 0; i < Config::WeaponPaintKits.size(); i++)
        Config::WeaponPaintKits.at(i)->ClearSelection();
    Config::GetPaintKit("skinchanger-knife-paintkit")->ClearSelection();
    Config::GetState("skinchanger-knife-enable")->Set(false);
    if (G::LocalPlayer && G::LocalPlayerAlive && G::IsInGame)
    {
        RunFSN();
        GUI::ResetInventoryHud();
        Update();
    }

    for (ClientClass* pClass = I::client->GetAllClasses(); pClass; pClass = pClass->m_pNext)
    {
        if (!strcmp(pClass->m_pNetworkName, "CBaseViewModel"))
        {
            for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
            {
                RecvProp* pProp = &(pClass->m_pRecvTable->m_pProps[i]);
                if (!pProp) continue;

                if (!strcmp(pProp->m_pVarName, "m_nModelIndex"))
                {
                    pProp->m_ProxyFn = oModelIndexProxy;
                }
                else if (!strcmp(pProp->m_pVarName, "m_nSequence"))
                {
                    pProp->m_ProxyFn = oSequenceProxy;
                }
            }
        }
    }

    //m_vecOrigin
    //{
    //    RecvProp* prop = N::FindRecvPropByPath({ "DT_CSPlayer", "DT_CSNonLocalPlayerExclusive", "m_vecOrigin" });
    //    prop->m_ProxyFn = oTestProxy;
    //}
}

void SkinChanger::ForceSkin(Entity* Weapon, int PaintKit, float Wear)
{
    // only update if something has changed
    bool NeedsUpdate =
        *Weapon->GetItemIDHigh() == 0 ||
        *Weapon->GetFallbackPaintKit() != PaintKit ||
        *Weapon->GetFallbackWear() != Wear ||
        *Weapon->GetFallbackStatTrak() != -1 ||
        *Weapon->GetFallbackOriginalOwnerXuidLow() != 0 ||
        *Weapon->GetFallbackOriginalOwnerXuidHigh() != 0 ||
        strcmp(Weapon->GetCustomName(), "www.a4g4.com");
    if (!NeedsUpdate) return;

    *Weapon->GetFallbackStatTrak() = -1;
    *Weapon->GetFallbackPaintKit() = PaintKit;
    *Weapon->GetFallbackOriginalOwnerXuidLow() = 0;
    *Weapon->GetFallbackOriginalOwnerXuidHigh() = 0;
    *Weapon->GetFallbackWear() = Wear;
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

CreateClientClassFn SkinChanger::CreateWearable = nullptr;
HANDLE GenerateGlove(Entity** Output)
{
    int NextIndex = I::entitylist->GetHighestEntityIndex() + 1;
    int SerialNumber = rand() % 4096;
    HANDLE Handle = (HANDLE)(NextIndex | SerialNumber << 16);

    SkinChanger::CreateWearable(NextIndex, SerialNumber);
    *Output = I::entitylist->GetClientEntity(NextIndex);

    return Handle;
}

WeaponId SkinChanger::OriginalKnife = WeaponId::INVALID;
WeaponId SkinChanger::AppliedKnife = WeaponId::TDefaultKnife;
void SkinChanger::RunFSN()
{
    static auto EnableKnifeChanger = Config::GetState("skinchanger-knife-enable");
    static auto KnifeModel = Config::GetState("skinchanger-knife-model");
    static auto KnifeSkin = Config::GetPaintKit("skinchanger-knife-paintkit");
    static auto KnifeWear = Config::GetFloat("skinchanger-knife-wear");

    {
        HANDLE* Wearables = G::LocalPlayer->GetWearables();

        HANDLE hWearingGlove = Wearables[0];
        Entity* WearingGlove = I::entitylist->GetClientEntityFromHandle(hWearingGlove);

        static HANDLE hCreatedGlove = hWearingGlove;
        Entity* CreatedGlove = hWearingGlove == hCreatedGlove ? WearingGlove : I::entitylist->GetClientEntityFromHandle(hCreatedGlove);

        // if there's no valid glove, create one
        if (!WearingGlove && !CreatedGlove)
        {
            hCreatedGlove = GenerateGlove(&CreatedGlove);
        }

        // if not wearing valid glove, apply created one
        if (!WearingGlove)
        {
            hWearingGlove = hCreatedGlove;
            WearingGlove = CreatedGlove;
        }

        Wearables[0] = hWearingGlove;

        *WearingGlove->GetIndex() = -1;
        *WearingGlove->GetItemDefinitionIndex() = (int)WeaponId::SportGloves;
        WearingGlove->SetModelIndex(I::modelinfo->GetModelIndex("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"));
        ForceSkin(WearingGlove, 10048, 0.001f);
        WearingGlove->GetClientNetworkable()->PreDataUpdate(0);
    }

    for (int i = 0; i < I::entitylist->GetHighestEntityIndex(); i++)
    {
        Entity* Weapon = I::entitylist->GetClientEntity(i);
        if (!Weapon) continue;
        if (I::entitylist->GetClientEntityFromHandle(Weapon->GetOwner()) != G::LocalPlayer) continue;
        
        WeaponId id = (WeaponId)Weapon->GetWeaponId();
        int Index = -1;
        if ((Index = (int)Skins::WeaponFromId(id)) >= 0)
        {
            // this is a gun
            const Skins::PaintKit* PaintKit = Config::WeaponPaintKits.at(Index)->PaintKit;
            if (PaintKit && PaintKit->ID != 0)
                ForceSkin(Weapon, PaintKit->ID, 0.f);
            else
                ClearSkin(Weapon);
        }
        else if ((Index = (int)Skins::KnifeFromId(id)) >= 0)
        {
            WeaponId ConfigKnifeModel = (WeaponId)Skins::IdFromKnife((Skins::Knife)KnifeModel->Get());
            static WeaponId LastConfigKnifeModel = ConfigKnifeModel;

            if (OriginalKnife == WeaponId::INVALID)
                OriginalKnife = ConfigKnifeModel;

            // detect if this is the original knife skin
            // if the user has not changed their knife selection, but the currently set knife differs from the config
            // then this must be the original knife
            if (LastConfigKnifeModel == ConfigKnifeModel && Weapon->GetWeaponId() != ConfigKnifeModel)
            {
                OriginalKnife = (WeaponId)*Weapon->GetItemDefinitionIndex();
            }
            static int ModelIndex = I::modelinfo->GetModelIndex(Skins::GetKnifeModel(Skins::KnifeFromId(ConfigKnifeModel)));
            if (LastConfigKnifeModel != ConfigKnifeModel)
                ModelIndex = I::modelinfo->GetModelIndex(Skins::GetKnifeModel(Skins::KnifeFromId(ConfigKnifeModel)));
            LastConfigKnifeModel = ConfigKnifeModel;

            if (EnableKnifeChanger->Get())
            {
                SkinChanger::AppliedKnife = ConfigKnifeModel;
                if (*Weapon->GetItemDefinitionIndex() != (int)ConfigKnifeModel || *Weapon->GetModelIndex() != ModelIndex)
                {
                    *Weapon->GetItemDefinitionIndex() = (int)ConfigKnifeModel;
                    *Weapon->GetModelIndex() = ModelIndex;
                    GUI::ResetInventoryHud();
                    Update();
                }
                if (KnifeSkin->PaintKit && KnifeSkin->PaintKit->ID > 0)
                    ForceSkin(Weapon, KnifeSkin->PaintKit->ID, KnifeWear->Get());
                else
                    ClearSkin(Weapon);
            }
            else
            {
                SkinChanger::AppliedKnife = OriginalKnife;
                if (Weapon->GetWeaponId() != OriginalKnife)
                {
                    *Weapon->GetItemDefinitionIndex() = (int)OriginalKnife;
                    *Weapon->GetModelIndex() = I::modelinfo->GetModelIndex(Skins::GetKnifeModel(Skins::KnifeFromId(OriginalKnife)));
                    GUI::ResetInventoryHud();
                    Update();
                }
                ClearSkin(Weapon);
            }
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
            *ViewModel->GetAnimSequence() = TransformSequence(*ViewModel->GetAnimSequence(), Skins::KnifeFromId(OriginalKnife), Skins::KnifeFromId(AppliedKnife));
        }
    }
}


