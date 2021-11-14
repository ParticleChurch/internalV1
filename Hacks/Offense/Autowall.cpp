#include "../../Include.hpp"

// Autowall
Autowall* autowall = new Autowall();

float Autowall::GetDamageMultiplier(int HitGroup)
{
    switch (HitGroup)
    {
    case HITGROUP_HEAD:
        return 4.f;
    case HITGROUP_STOMACH:
        return 1.25f;
    case HITGROUP_LEFTLEG:
    case HITGROUP_RIGHTLEG:
        return 0.75f;
    }
    return 1.0f;
}

bool Autowall::IsArmored(int HitGroup, bool Helmet)
{
    switch (HitGroup) {
    case HITGROUP_HEAD:
        return Helmet;
    case HITGROUP_CHEST:
    case HITGROUP_STOMACH:
    case HITGROUP_LEFTARM:
    case HITGROUP_RIGHTARM:
        return true;
    default:
        return false;
    }
}

float Autowall::HandleBulletPenetration(SurfaceData* EnterSurfaceData, const trace_t& EnterTrace, const Vec& Direction, Vec& Result, float Penetration, float Damage)
{
    Vec End;
    trace_t ExitTrace;
    __asm {
        mov ecx, End
        mov edx, EnterTrace
    }
    if (!this->TraceToExit(EnterTrace.Endpos.x, EnterTrace.Endpos.y, EnterTrace.Endpos.z, Direction.x, Direction.y, Direction.z, ExitTrace))
        return -1.0f;

    SurfaceData* ExitSurfaceData = I::physicssurfaceprops->getSurfaceData(ExitTrace.Surface.SurfaceProps);

    float DamageModifier = 0.16f;
    float PenetrationModifier = (EnterSurfaceData->game.flPenetrationModifier + ExitSurfaceData->game.flPenetrationModifier) / 2.0f;

    if (EnterSurfaceData->game.material == 71 || EnterSurfaceData->game.material == 89) {
        DamageModifier = 0.05f;
        PenetrationModifier = 3.0f;
    }
    else if (EnterTrace.Contents >> 3 & 1 || EnterTrace.Surface.Flags >> 7 & 1) {
        PenetrationModifier = 1.0f;
    }

    if (EnterSurfaceData->game.material == ExitSurfaceData->game.material) {
        if (ExitSurfaceData->game.material == 85 || ExitSurfaceData->game.material == 87)
            PenetrationModifier = 3.0f;
        else if (ExitSurfaceData->game.material == 76)
            PenetrationModifier = 2.0f;
    }

    Damage -= 11.25f / Penetration / PenetrationModifier + Damage * DamageModifier + (ExitTrace.Endpos - EnterTrace.Endpos).SquareLength() / 24.0f / PenetrationModifier;

    Result = ExitTrace.Endpos;
    return Damage;
}

void Autowall::TraceLine(Vec& start, Vec& end, unsigned int mask, Entity* ignore, trace_t* tr)
{
    Ray_t ray(start, end);
    CTraceFilter filter(ignore);
    I::enginetrace->TraceRay(ray, MASK_SHOT | CONTENTS_HITBOX, &filter, tr);
}

bool Autowall::IsVisible(Vec Start, Vec End, Entity* Ent)
{
    trace_t Trace;
    Ray_t Ray(Start, End);
    CTraceFilter Filter(G::LocalPlayer);
    I::enginetrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

    return (Trace.Entity == Ent || Trace.Fraction > 0.97f);
}

bool Autowall::IsVisible(Vec End, Entity* Ent)
{
    Vec Start = G::LocalPlayer->GetEyePos();

    trace_t Trace;
    Ray_t Ray(Start, End);
    CTraceFilter Filter(G::LocalPlayer);
    I::enginetrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

    return (Trace.Entity == Ent || Trace.Fraction > 0.97f);
}

float Autowall::Damage(const Vec& point, int hitbox, bool AllowFriendlyFire)
{
    if (!G::LocalPlayer) return false;

    if (!G::LocalPlayerWeaponData) return false;

    float Damage = G::LocalPlayerWeaponData->Damage;

    Vec Start{ G::LocalPlayer->GetEyePos() };
    Vec Direction{ Start - point };
    Direction /= Direction.VecLength();

    // Then check if autowallable
    int hitsLeft = 4;
    while (Damage >= 1.0f && hitsLeft > 0) {
        trace_t Trace;
        CTraceFilter Filter(G::LocalPlayer);
        Ray_t Ray(Start, point);
        I::enginetrace->TraceRay(Ray, 0x4600400B, &Filter, &Trace);

        if (!AllowFriendlyFire && Trace.Entity && Trace.Entity->IsPlayer() && (G::LocalPlayer->GetTeam() == Trace.Entity->GetTeam()))
            return 0.f;

        if (Trace.Fraction == 1.0f)
            break;

        if (Trace.Entity 
            && Trace.Entity->IsPlayer()
            && Trace.Hitgroup >= HITGROUP_GENERIC && Trace.Hitgroup <= HITGROUP_RIGHTLEG
            && Trace.Hitgroup == aimbot->GetHitGroup(hitbox)
            && Trace.Entity->GetTeam() != G::LocalPlayerTeam) {

            Damage = GetDamageMultiplier(Trace.Hitgroup) * Damage * powf(G::LocalPlayerWeaponData->RangeModifier, Trace.Fraction * G::LocalPlayerWeaponData->Range / 500.0f);

            float ArmorRatio = G::LocalPlayerWeaponData->ArmorRatio / 2.0f;
            if (IsArmored(Trace.Hitgroup, Trace.Entity->HasHelmet()))
                Damage -= (Trace.Entity->ArmorVal() < Damage * ArmorRatio / 2.0f ? Trace.Entity->ArmorVal() * 4.0f : Damage) * (1.0f - ArmorRatio);

            return Damage;
        }
        // if its some other hitbox, say you can do TINY damage to the targeted hitbox
        else if (Trace.Entity
            && Trace.Entity->IsPlayer()
            && Trace.Hitgroup >= HITGROUP_GENERIC && Trace.Hitgroup <= HITGROUP_RIGHTLEG
            && Trace.Hitgroup != aimbot->GetHitGroup(hitbox)
            && Trace.Entity->GetTeam() != G::LocalPlayerTeam)
        {
            Damage = GetDamageMultiplier(Trace.Hitgroup) * Damage * powf(G::LocalPlayerWeaponData->RangeModifier, Trace.Fraction * G::LocalPlayerWeaponData->Range / 500.0f);

            float ArmorRatio = G::LocalPlayerWeaponData->ArmorRatio / 2.0f;
            if (IsArmored(Trace.Hitgroup, Trace.Entity->HasHelmet()))
                Damage -= (Trace.Entity->ArmorVal() < Damage * ArmorRatio / 2.0f ? Trace.Entity->ArmorVal() * 4.0f : Damage) * (1.0f - ArmorRatio);

            // if we are aiming for head, we DEF dont wanna shoot unless we ABSOLUTELY see head
            return aimbot->GetHitGroup(hitbox) == HITGROUP_HEAD ? Damage / 2 : Damage;
        }

        const auto SurfaceData = I::physicssurfaceprops->getSurfaceData(Trace.Surface.SurfaceProps);

        if (SurfaceData->game.flPenetrationModifier < 0.1f)
            break;

        Damage = HandleBulletPenetration(SurfaceData, Trace, Direction, Start, G::LocalPlayerWeaponData->Penetration, Damage);
        hitsLeft--;
    }
    return 0.f;
}

float Autowall::Damage(Vec start, const Vec& point, bool AllowFriendlyFire)
{
    if (!G::LocalPlayer) return false;

    if (!G::LocalPlayerWeaponData) return false;

    float Damage = G::LocalPlayerWeaponData->Damage;

    Vec Start{ start };
    Vec Direction{ Start - point };
    Direction /= Direction.VecLength();

    // Then check if autowallable
    int hitsLeft = 4;
    while (Damage >= 1.0f && hitsLeft > 0) {
        trace_t Trace;
        CTraceFilter Filter(G::LocalPlayer);
        Ray_t Ray(Start, point);
        I::enginetrace->TraceRay(Ray, 0x4600400B, &Filter, &Trace);

        if (!AllowFriendlyFire && Trace.Entity && Trace.Entity->IsPlayer() && (G::LocalPlayer->GetTeam() == Trace.Entity->GetTeam()))
            return false;

        if (Trace.Fraction == 1.0f)
            break;

        if (Trace.Hitgroup > HITGROUP_GENERIC && Trace.Hitgroup <= HITGROUP_RIGHTLEG) {
            Damage = GetDamageMultiplier(Trace.Hitgroup) * Damage * powf(G::LocalPlayerWeaponData->RangeModifier, Trace.Fraction * G::LocalPlayerWeaponData->Range / 500.0f);

            float ArmorRatio = G::LocalPlayerWeaponData->ArmorRatio / 2.0f;
            if (IsArmored(Trace.Hitgroup, Trace.Entity->HasHelmet()))
                Damage -= (Trace.Entity->ArmorVal() < Damage * ArmorRatio / 2.0f ? Trace.Entity->ArmorVal() * 4.0f : Damage) * (1.0f - ArmorRatio);

            return Damage;
        }

        const auto SurfaceData = I::physicssurfaceprops->getSurfaceData(Trace.Surface.SurfaceProps);

        if (SurfaceData->game.flPenetrationModifier < 0.1f)
            break;

        Damage = HandleBulletPenetration(SurfaceData, Trace, Direction, Start, G::LocalPlayerWeaponData->Penetration, Damage);
        hitsLeft--;
    }
    return 0.f;
}

//https://github.com/sstokic-tgm/Gladiatorcheatz-v2/blob/master/features/LagCompensation.cpp 
// need to fight with FinishLagCompensation function to see how to rage backtrack
// STILL IN PROGRESS!
bool Autowall::CanHitFloatingPoint(const Vec& point, bool AllowFriendlyFire)
{
    if (!G::LocalPlayer) return false;

    if (!G::LocalPlayerWeaponData) return false;

    float Damage = G::LocalPlayerWeaponData->Damage;

    Vec Start{ G::LocalPlayer->GetEyePos() };
    Vec Direction{ Start - point };
    Direction /= Direction.VecLength();

    // Then check if is autowallable
    int hitsLeft = 4;
    while (Damage >= 1.0f && hitsLeft > 0) {
        trace_t Trace;
        CTraceFilter Filter(G::LocalPlayer);
        Ray_t Ray(Start, point);
        //MASK_SHOT + 0x0000000B
        I::enginetrace->TraceRay(Ray, 0x4600400B, &Filter, &Trace);

        if (!AllowFriendlyFire && Trace.Entity && Trace.Entity->IsPlayer() && (G::LocalPlayer->GetTeam() == Trace.Entity->GetTeam()))
            return false;


        // We have reached end of trace and therefore can hit it
        L::Debug("Checking trace fraction 1");
        if (Trace.Fraction > 0.97f && (!Trace.Entity || Trace.Entity->GetTeam() != G::LocalPlayerTeam))
        {
            // if reached end of ray, and the trace entity isn't on our team/doesnt exist...
            return true;
        }
            

        // We've hit an enemy hitbox, and therefore can't hit point?
        // as long as trace entity's team isn't on our team
        if (Trace.Hitgroup > HITGROUP_GENERIC && Trace.Hitgroup <= HITGROUP_RIGHTLEG && Trace.Entity->GetTeam() != G::LocalPlayerTeam) {
            Damage = GetDamageMultiplier(Trace.Hitgroup) * Damage * powf(G::LocalPlayerWeaponData->RangeModifier, Trace.Fraction * G::LocalPlayerWeaponData->Range / 500.0f);

            float ArmorRatio = G::LocalPlayerWeaponData->ArmorRatio / 2.0f;
            if (IsArmored(Trace.Hitgroup, Trace.Entity->HasHelmet()))
                Damage -= (Trace.Entity->ArmorVal() < Damage * ArmorRatio / 2.0f ? Trace.Entity->ArmorVal() * 4.0f : Damage) * (1.0f - ArmorRatio);

            return Damage >= 1;
        }

        const auto SurfaceData = I::physicssurfaceprops->getSurfaceData(Trace.Surface.SurfaceProps);

        if (SurfaceData->game.flPenetrationModifier < 0.1f)
            break;

        Damage = HandleBulletPenetration(SurfaceData, Trace, Direction, Start, G::LocalPlayerWeaponData->Penetration, Damage);
        hitsLeft--;
    }
    return false;
}

bool Autowall::CanHitFloatingPoint(Vec start, const Vec& point, bool AllowFriendlyFire)
{
    if (!G::LocalPlayer) return false;

    if (!G::LocalPlayerWeaponData) return false;

    float Damage = G::LocalPlayerWeaponData->Damage;

    Vec Start{ start };
    Vec Direction{ Start - point };
    Direction /= Direction.VecLength();

    // Then check if is autowallable
    int hitsLeft = 3; // orig 4, changed to 3 because we dont need that accuracy
    while (Damage >= 1.0f && hitsLeft > 0) {
        trace_t Trace;
        CTraceFilter Filter(G::LocalPlayer);
        Ray_t Ray(Start, point);
        //MASK_SHOT + 0x0000000B
        I::enginetrace->TraceRay(Ray, 0x4600400B, &Filter, &Trace);

        if (!AllowFriendlyFire && Trace.Entity && Trace.Entity->IsPlayer() && (G::LocalPlayer->GetTeam() == Trace.Entity->GetTeam()))
            return false;


        // We have reached end of trace and therefore can hit it
        L::Debug("Checking trace fraction 1");
        if (Trace.Fraction > 0.97f && (!Trace.Entity || Trace.Entity->GetTeam() != G::LocalPlayerTeam))
        {
            // if reached end of ray, and the trace entity isn't on our team/doesnt exist...
            return true;
        }


        // We've hit an enemy hitbox, and therefore can't hit point?
        // as long as trace entity's team isn't on our team
        if (Trace.Hitgroup > HITGROUP_GENERIC && Trace.Hitgroup <= HITGROUP_RIGHTLEG && Trace.Entity->GetTeam() != G::LocalPlayerTeam) {
            Damage = GetDamageMultiplier(Trace.Hitgroup) * Damage * powf(G::LocalPlayerWeaponData->RangeModifier, Trace.Fraction * G::LocalPlayerWeaponData->Range / 500.0f);

            float ArmorRatio = G::LocalPlayerWeaponData->ArmorRatio / 2.0f;
            if (IsArmored(Trace.Hitgroup, Trace.Entity->HasHelmet()))
                Damage -= (Trace.Entity->ArmorVal() < Damage * ArmorRatio / 2.0f ? Trace.Entity->ArmorVal() * 4.0f : Damage) * (1.0f - ArmorRatio);

            return Damage >= 1;
        }

        const auto SurfaceData = I::physicssurfaceprops->getSurfaceData(Trace.Surface.SurfaceProps);

        if (SurfaceData->game.flPenetrationModifier < 0.1f)
            break;

        Damage = HandleBulletPenetration(SurfaceData, Trace, Direction, Start, G::LocalPlayerWeaponData->Penetration, Damage);
        hitsLeft--;
    }
    return false;
}

bool Autowall::CanWallbang(float& dmg)
{
    if (!G::LocalPlayer) return false;

    if (!G::LocalPlayer->IsAlive()) return false;

    Vec EyeAng;
    I::engine->GetViewAngles(EyeAng);

    Vec dst, forward;

    AngleVectors(EyeAng, &forward);
    dst = G::LocalPlayer->GetEyePos() + (forward * 8196.f); //idk what this number is lmao

    // Do full autowall for wallbang
    if (Damage(dst, false) > dmg)
        return true;

    return false;
}

// AUTOWALL2_________________________________________________
Autowall2* autowall2 = new Autowall2();

void Autowall2::TraceLine(Vec& start, Vec& end, unsigned int mask, Entity* ignore, trace_t* tr)
{
    Ray_t ray(start, end);
    CTraceFilter filter(ignore);
    I::enginetrace->TraceRay(ray, mask, &filter, tr);
}

bool Autowall2::VectortoVectorVisible(Vec src, Vec point)
{
    trace_t Trace;
    TraceLine(src, point, MASK_SOLID, G::LocalPlayer, &Trace);

    if (Trace.Fraction == 1.0f)
    {
        return true;
    }

    return false;
}

bool Autowall2::HandleBulletPenetration(WeaponData* wpn_data, FireBulletData& data, bool extracheck)
{
    SurfaceData* enter_surface_data = I::physicssurfaceprops->getSurfaceData(data.enter_trace.Surface.SurfaceProps);
    int enter_material = enter_surface_data->game.material;
    float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
    data.trace_length += data.enter_trace.Fraction * data.trace_length_remaining;
    data.current_damage *= pow(wpn_data->RangeModifier, (data.trace_length * 0.002));
    if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
        data.penetrate_count = 0;
    if (data.penetrate_count <= 0)
        return false;
    static Vec dummy;
    static trace_t trace_exit;
    if (!TraceToExitalt(dummy, data.enter_trace, data.enter_trace.Endpos, data.direction, &trace_exit))
        return false;
    SurfaceData* exit_surface_data = I::physicssurfaceprops->getSurfaceData(trace_exit.Surface.SurfaceProps);
    int exit_material = exit_surface_data->game.material;
    float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
    float final_damage_modifier = 0.16f;
    float combined_penetration_modifier = 0.0f;
    if (((data.enter_trace.Contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
    {
        combined_penetration_modifier = 3.0f;
        final_damage_modifier = 0.05f;
    }
    else
        combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
    if (enter_material == exit_material)
    {
        if (exit_material == 87 || exit_material == 85)combined_penetration_modifier = 3.0f;
        else if (exit_material == 76)combined_penetration_modifier = 2.0f;
    }
    float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
    float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->Penetration) * 1.25f);
    float thickness = (trace_exit.Endpos - data.enter_trace.Endpos).VecLength();
    if (extracheck)
        if (!VectortoVectorVisible(trace_exit.Endpos, aim))
            return false;
    thickness *= thickness;
    thickness *= v34;
    thickness /= 24.0f;
    float lost_damage = fmaxf(0.0f, v35 + thickness);
    if (lost_damage > data.current_damage)
        return false;
    if (lost_damage >= 0.0f)
        data.current_damage -= lost_damage;
    if (data.current_damage < 1.0f)
        return false;
    data.src = trace_exit.Endpos;
    data.penetrate_count--;

    return true;
}

bool Autowall2::TraceToExitalt(Vec& end, trace_t& tr, Vec start, Vec vEnd, trace_t* trace)
{
    typedef bool(__fastcall* TraceToExitFn)(Vec&, trace_t&, float, float, float, float, float, float, trace_t*);
    static TraceToExitFn TraceToExit = (TraceToExitFn)FindPattern("client.dll", "55 8B EC 83 EC 30 F3 0F 10 75");

    if (!TraceToExit)
        return false;

    //return TraceToExit(end, tr, start.x, start.y, start.z, vEnd.x, vEnd.y, vEnd.z, trace);
    __asm
    {
        push trace
        push vEnd.z
        push vEnd.y
        push vEnd.x
        push start.z
        push start.y
        push start.x
        mov edx, tr
        mov ecx, end
        call TraceToExit
        add esp, 0x1C
    }
}

bool Autowall2::SimulateFireBullet(Entity* local, Entity* weapon, FireBulletData& data)
{
    data.penetrate_count = 4; // Max Amount Of Penitration
    data.trace_length = 0.0f; // wow what a meme
    auto* wpn_data = weapon->GetWeaponData(); // Get Weapon Info
    data.current_damage = (float)wpn_data->Damage;// Set Damage Memes
    while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
    {
        data.trace_length_remaining = wpn_data->Range - data.trace_length;
        Vec End_Point = data.src + data.direction * data.trace_length_remaining;
        TraceLine(data.src, End_Point, 0x4600400B, local, &data.enter_trace);
        UTIL_ClipTraceToPlayers(data.src, End_Point * 40.f, 0x4600400B, &data.filter, &data.enter_trace);
        if (data.enter_trace.Fraction == 1.0f)
            break;
        if ((data.enter_trace.Hitgroup <= 7) && (data.enter_trace.Hitgroup > 0) && (local->GetTeam() != data.enter_trace.Entity->GetTeam()))
        {
            data.trace_length += data.enter_trace.Fraction * data.trace_length_remaining;
            data.current_damage *= pow(wpn_data->RangeModifier, data.trace_length * 0.002);
            ScaleDamage(data.enter_trace.Hitgroup, data.enter_trace.Entity, wpn_data->ArmorRatio, data.current_damage);
            return true;
        }
        if (!HandleBulletPenetration(wpn_data, data, false))
            break;
    }
    return false;
}

void Autowall2::UTIL_ClipTraceToPlayers(const Vec& vecAbsStart, const Vec& vecAbsEnd, unsigned int mask, CTraceFilter* filter, trace_t* tr)
{
    static DWORD dwAddress = FindPattern("client.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10");

    if (!dwAddress)
        return;

    _asm
    {
        MOV		EAX, filter
        LEA		ECX, tr
        PUSH	ECX
        PUSH	EAX
        PUSH	mask
        LEA		EDX, vecAbsEnd
        LEA		ECX, vecAbsStart
        CALL	dwAddress
        ADD		ESP, 0xC
    }
}

void Autowall2::ScaleDamage(int hitgroup, Entity* enemy, float weapon_armor_ratio, float& current_damage)
{
    current_damage *= GetHitgroupDamageMult(hitgroup);

    if (enemy->ArmorVal() > 0.0f && hitgroup < HITGROUP_LEFTLEG)
    {
        if (hitgroup == HITGROUP_HEAD && !enemy->HasHelmet())
            return;

        float armorscaled = (weapon_armor_ratio * 0.5f) * current_damage;
        if ((current_damage - armorscaled) * 0.5f > enemy->ArmorVal())
            armorscaled = current_damage - (enemy->ArmorVal() * 2.0f);
        current_damage = armorscaled;
    }
}

float Autowall2::GetHitgroupDamageMult(int iHitGroup)
{
    switch (iHitGroup)
    {
    case HITGROUP_HEAD:
        return 4.f;
    case HITGROUP_STOMACH:
        return 1.25f;
    case HITGROUP_LEFTLEG:
    case HITGROUP_RIGHTLEG:
        return 0.75f;
    }

    return 1.0f;
}

float Autowall2::Damage(const Vec& point)
{
    auto data = FireBulletData(G::LocalPlayer->GetEyePos(), G::LocalPlayer);

    Vec angles;
    angles = aimbot->CalculateAngle(data.src, point);
    AngleVectors(angles, &data.direction);
    data.direction /= data.direction.VecLength();

    if (SimulateFireBullet(G::LocalPlayer, G::LocalPlayer->GetActiveWeapon(), data))
        return data.current_damage;

    return 0.f;
}

bool Autowall2::CanHitFloatingPoint(const Vec& point, const Vec& source)
{
    aim = point;

    if (!G::LocalPlayer)
        return false;

    FireBulletData data = FireBulletData(source, G::LocalPlayer);

    Vec angles = aimbot->CalculateAngle(data.src, point);
    AngleVectors(angles, &data.direction);
    data.direction /= data.direction.VecLength();

    Entity* pWeapon = G::LocalPlayerWeapon;

    if (!pWeapon)
        return false;

    data.penetrate_count = 1;
    data.trace_length = 0.0f;

    WeaponData* weaponData = G::LocalPlayerWeaponData;

    if (!weaponData)
        return false;

    data.current_damage = (float)weaponData->Damage;

    data.trace_length_remaining = weaponData->Range - data.trace_length;

    Vec end = data.src + data.direction * data.trace_length_remaining;

    TraceLine(data.src, end, MASK_SHOT | CONTENTS_HITBOX, G::LocalPlayer, &data.enter_trace);

    if (VectortoVectorVisible(data.src, point))
    {
        return true;
    }
    static bool extra_check = true;
    if (HandleBulletPenetration(weaponData, data, extra_check))
    {
        return true;
    }

    return false;
}
