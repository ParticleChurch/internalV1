#include "../../Include.hpp"

SkinChanger* skinchanger = new SkinChanger();

void SkinChanger::ForceSkin(Entity* weap, int skinid)
{
    int* idhigh = weap->GetIDHigh();
    if (idhigh == nullptr) // nullptr check this shit, sometimes its annoying
        return;

    *idhigh = 1; // the reason we set this is so the game is forced to use the fallback values

    int* paintkit = weap->GetPaintKit();
    if (!paintkit) return;

    *paintkit = skinid; // we'll set this in our FrameStage hook

    float* wear = weap->GetFallBackWear();
    if (!wear) return;

    *wear = 0.0000000001f; // lower = better
}

void SkinChanger::run(int stage)
{
	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
        for (int i = 0; i < I::entitylist->GetHighestEntityIndex(); i++) // iterate through everything
        {
            Entity* weapon = (Entity*)I::entitylist->GetClientEntity(i); // define your weapon
            if (!weapon) continue;

            int owner = weapon->GetOwner();
            if (!I::entitylist->GetClientEntityFromHandle((HANDLE)owner)) continue;

            if (I::entitylist->GetClientEntityFromHandle((HANDLE)owner) != G::LocalPlayer) continue;


            ForceSkin(weapon, 180);

            //if (!cl::entlist->GetClientEntityFromHandle(*(int*)((DWORD)weapon, offsets.m_hOwnerEntity))) // null check handle, dont check against NULL/0 since a null handle will be 0xFFFFFFFF
            //    continue;
            //if (cl::entlist->GetClientEntityFromHandle(*(int*)((DWORD)weapon, offsets.m_hOwnerEntity)) != localent) // weapon handle isnt yours
            //    continue;
        }
	}
}
