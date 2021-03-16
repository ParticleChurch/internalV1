#include "../../Include.hpp"

Autowall* autowall = new Autowall();

float Autowall::GetDamageMultiplier(int HitGroup)
{
    switch (HitGroup) {
    case HITGROUP_HEAD:
        return 4.0f;
    case HITGROUP_STOMACH:
        return 1.25f;
    case HITGROUP_LEFTLEG:
    case HITGROUP_RIGHTLEG:
        return 0.75f;
    default:
        return 1.0f;
    }
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
    float PenetrationModifier = (EnterSurfaceData->PenetrationModifier + ExitSurfaceData->PenetrationModifier) / 2.0f;

    if (EnterSurfaceData->Material == 71 || EnterSurfaceData->Material == 89) {
        DamageModifier = 0.05f;
        PenetrationModifier = 3.0f;
    }
    else if (EnterTrace.Contents >> 3 & 1 || EnterTrace.Surface.Flags >> 7 & 1) {
        PenetrationModifier = 1.0f;
    }

    if (EnterSurfaceData->Material == ExitSurfaceData->Material) {
        if (ExitSurfaceData->Material == 85 || ExitSurfaceData->Material == 87)
            PenetrationModifier = 3.0f;
        else if (ExitSurfaceData->Material == 76)
            PenetrationModifier = 2.0f;
    }

    Damage -= 11.25f / Penetration / PenetrationModifier + Damage * DamageModifier + (ExitTrace.Endpos - EnterTrace.Endpos).SquareLength() / 24.0f / PenetrationModifier;

    Result = ExitTrace.Endpos;
    return Damage;
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

float Autowall::GetDamage(Entity* Ent, const Vec& Destination, bool AllowFriendlyFire)
{
    if (!G::LocalPlayer) return 0;

    Entity* weapon = G::LocalPlayer->GetActiveWeapon();
    if (!weapon) return 0;

    WeaponData* WeaponData = weapon->GetWeaponData();
    if (!WeaponData) return 0;

    float Damage = WeaponData->Damage;

    Vec Start{ G::LocalPlayer->GetEyePos() };
    Vec Dest{ Vec(Destination) };
    Vec Direction{ Start - Dest };
    Direction /= Direction.VecLength();

    int hitsLeft = 4;

    while (Damage >= 1.0f && hitsLeft > 0) {
        trace_t Trace;
        CTraceFilter Filter(G::LocalPlayer);
        Ray_t Ray(Start, Destination);
        I::enginetrace->TraceRay(Ray, 0x4600400B, &Filter, &Trace);

        if (!AllowFriendlyFire && Trace.Entity && Trace.Entity->IsPlayer() && (G::LocalPlayer->GetTeam() == Trace.Entity->GetTeam()))
            return 0;

        if (Trace.Fraction == 1.0f)
            break;

        bool PossibleDam = GetDamageMultiplier(HITGROUP_HEAD) * Damage * powf(WeaponData->RangeModifier, Trace.Fraction * WeaponData->Range / 500.0f) >= 1;
        if (!PossibleDam) // if we can't do damage anyway, why bother xD
            return 0;

        if (Trace.Entity == Ent && Trace.Hitgroup > HITGROUP_GENERIC && Trace.Hitgroup <= HITGROUP_RIGHTLEG) {
            Damage = GetDamageMultiplier(Trace.Hitgroup) * Damage * powf(WeaponData->RangeModifier, Trace.Fraction * WeaponData->Range / 500.0f);

            float ArmorRatio = WeaponData->ArmorRatio / 2.0f;
            if (IsArmored(Trace.Hitgroup, Trace.Entity->HasHelmet()))
                Damage -= (Trace.Entity->ArmorVal() < Damage * ArmorRatio / 2.0f ? Trace.Entity->ArmorVal() * 4.0f : Damage) * (1.0f - ArmorRatio);
            return Damage;
        }

        const auto SurfaceData = I::physicssurfaceprops->getSurfaceData(Trace.Surface.SurfaceProps);

        if (SurfaceData->PenetrationModifier < 0.1f)
            break;


        Damage = HandleBulletPenetration(SurfaceData, Trace, Direction, Start, WeaponData->Penetration, Damage);
        hitsLeft--;
    }
    return 0;
}

float Autowall::GetDamageVis(Entity* Ent, const Vec& Destination, bool AllowFriendlyFire)
{
    if (!G::LocalPlayer) return 0;

    Entity* weapon = G::LocalPlayer->GetActiveWeapon();
    if (!weapon) return 0;

    WeaponData* WeaponData = weapon->GetWeaponData();
    if (!WeaponData) return 0;

    float Damage = WeaponData->Damage;

    Vec Start{ G::LocalPlayer->GetEyePos() };
    Vec Dest{ Vec(Destination) };
    Vec Direction{ Start - Dest };
    Direction /= Direction.VecLength();

    if (Damage < 1.0f)
        return 0.f;

    trace_t Trace;
    CTraceFilter Filter(G::LocalPlayer);
    Ray_t Ray(Start, Destination);
    I::enginetrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

    if (!AllowFriendlyFire && Trace.Entity && Trace.Entity->IsPlayer() && (G::LocalPlayer->GetTeam() == Trace.Entity->GetTeam()))
        return 0.f;

    if (Trace.Fraction == 1.0f)
        return 0.f;

    if (Trace.Entity == Ent && Trace.Hitgroup > HITGROUP_GENERIC && Trace.Hitgroup <= HITGROUP_RIGHTLEG) {
        Damage = GetDamageMultiplier(Trace.Hitgroup) * Damage * powf(WeaponData->RangeModifier, Trace.Fraction * WeaponData->Range / 500.0f);

        float ArmorRatio = WeaponData->ArmorRatio / 2.0f;
        if (IsArmored(Trace.Hitgroup, Trace.Entity->HasHelmet()))
            Damage -= (Trace.Entity->ArmorVal() < Damage * ArmorRatio / 2.0f ? Trace.Entity->ArmorVal() * 4.0f : Damage) * (1.0f - ArmorRatio);
        return Damage;
    }
    return 0.f;
}

float Autowall::GetDamage(Entity* From, Entity* To, const Vec& Destination)
{
    if (!G::LocalPlayer)
        return 0;

    Entity* Weapon = From->GetActiveWeapon();
    if (!Weapon) return 0;

    WeaponData* WeaponData = Weapon->GetWeaponData();
    if (!WeaponData) return 0;

    float Damage = WeaponData->Damage;

    Vec Start{ From->GetEyePos() };
    Vec Dest{ Vec(Destination) };
    Vec Direction{ Start - Dest };
    Direction /= Direction.VecLength();

    int hitsLeft = 4;

    while (Damage >= 1.0f && hitsLeft > 0) {
        trace_t Trace;
        CTraceFilter Filter(From);
        Ray_t Ray(Start, Destination);
        I::enginetrace->TraceRay(Ray, 0x4600400B, &Filter, &Trace);

        if (Trace.Fraction == 1.0f || (Trace.Entity == To && Trace.Hitgroup > HITGROUP_GENERIC && Trace.Hitgroup <= HITGROUP_RIGHTLEG)) {
            Damage = GetDamageMultiplier(HITGROUP_HEAD) * Damage * powf(WeaponData->RangeModifier, Trace.Fraction * WeaponData->Range / 500.0f);

            float ArmorRatio = WeaponData->ArmorRatio / 2.0f;
            if (IsArmored(HITGROUP_HEAD, To->HasHelmet()))
                Damage -= (To->ArmorVal() < Damage * ArmorRatio / 2.0f ? To->ArmorVal() * 4.0f : Damage) * (1.0f - ArmorRatio);

            return Damage;
        }

        const auto SurfaceData = I::physicssurfaceprops->getSurfaceData(Trace.Surface.SurfaceProps);

        if (SurfaceData->PenetrationModifier < 0.1f)
            break;

        Damage = HandleBulletPenetration(SurfaceData, Trace, Direction, Start, WeaponData->Penetration, Damage);
        hitsLeft--;
    }
    return 0;
}

bool Autowall::GetDamage(Vec From, const Vec& Destination, Entity* Ent)
{
    if (!G::LocalPlayer)
        return false;

    float Damage = G::LocalPlayerWeaponData->Damage;

    Vec Start{ From };
    Vec Dest{ Vec(Destination) };
    Vec Direction{ Start - Dest };
    Direction /= Direction.VecLength();

    int hitsLeft = 4;

    while (Damage >= 1.0f && hitsLeft > 0) {
        trace_t Trace;
        CTraceFilter Filter(G::LocalPlayer);
        Ray_t Ray(Start, Destination);
        I::enginetrace->TraceRay(Ray, 0x4600400B, &Filter, &Trace);

        if (Trace.Entity && Trace.Entity->IsPlayer() && (G::LocalPlayer->GetTeam() == Trace.Entity->GetTeam()))
            return false;

        if (Trace.Fraction == 1.0f)
            break;

        if (Trace.Entity == Ent && Trace.Hitgroup > HITGROUP_GENERIC && Trace.Hitgroup <= HITGROUP_RIGHTLEG) {
            Damage = GetDamageMultiplier(Trace.Hitgroup) * Damage * powf(G::LocalPlayerWeaponData->RangeModifier, Trace.Fraction * G::LocalPlayerWeaponData->Range / 500.0f);

            float ArmorRatio = G::LocalPlayerWeaponData->ArmorRatio / 2.0f;
            if (IsArmored(Trace.Hitgroup, Trace.Entity->HasHelmet()))
                Damage -= (Trace.Entity->ArmorVal() < Damage * ArmorRatio / 2.0f ? Trace.Entity->ArmorVal() * 4.0f : Damage) * (1.0f - ArmorRatio);

            return Damage >= 1;
        }

        const auto SurfaceData = I::physicssurfaceprops->getSurfaceData(Trace.Surface.SurfaceProps);

        if (SurfaceData->PenetrationModifier < 0.1f)
            break;


        Damage = HandleBulletPenetration(SurfaceData, Trace, Direction, Start, G::LocalPlayerWeaponData->Penetration, Damage);
        hitsLeft--;
    }
    return false;
}

bool Autowall::CanScan(Entity* Ent, const Vec& Destination, const WeaponData* WeaponData, int MinDamage, bool AllowFriendlyFire)
{
    if (!G::LocalPlayer)
        return false;

    float Damage = WeaponData->Damage;

    Vec Start{ G::LocalPlayer->GetEyePos() };
    Vec Dest{ Vec(Destination) };
    Vec Direction{ Start - Dest };
    Direction /= Direction.VecLength();

    int hitsLeft = 4;

    while (Damage >= 1.0f && hitsLeft > 0) {
        trace_t Trace;
        CTraceFilter Filter(G::LocalPlayer);
        Ray_t Ray(Start, Destination);
        I::enginetrace->TraceRay(Ray, 0x4600400B, &Filter, &Trace);

        if (!AllowFriendlyFire && Trace.Entity && Trace.Entity->IsPlayer() && (G::LocalPlayer->GetTeam() == Trace.Entity->GetTeam()))
            return false;

        if (Trace.Fraction == 1.0f)
            break;

        if (Trace.Entity == Ent && Trace.Hitgroup > HITGROUP_GENERIC && Trace.Hitgroup <= HITGROUP_RIGHTLEG) {
            Damage = GetDamageMultiplier(Trace.Hitgroup) * Damage * powf(WeaponData->RangeModifier, Trace.Fraction * WeaponData->Range / 500.0f);

            float ArmorRatio = WeaponData->ArmorRatio / 2.0f;
            if (IsArmored(Trace.Hitgroup, Trace.Entity->HasHelmet()))
                Damage -= (Trace.Entity->ArmorVal() < Damage * ArmorRatio / 2.0f ? Trace.Entity->ArmorVal() * 4.0f : Damage) * (1.0f - ArmorRatio);

            return Damage >= MinDamage;
        }

        const auto SurfaceData = I::physicssurfaceprops->getSurfaceData(Trace.Surface.SurfaceProps);

        if (SurfaceData->PenetrationModifier < 0.1f)
            break;


        Damage = HandleBulletPenetration(SurfaceData, Trace, Direction, Start, WeaponData->Penetration, Damage);
        hitsLeft--;
    }
    return false;
}

bool Autowall::CanScanBacktrack(Entity* Ent, const Vec& Destination, const WeaponData* WeaponData, int MinDamage, bool AllowFriendlyFire, int Hitgroup)
{
    if (!G::LocalPlayer)
        return false;

    float Damage = WeaponData->Damage;

    Vec Start{ G::LocalPlayer->GetEyePos() };
    Vec Dest{ Vec(Destination) };
    Vec Direction{ Start - Dest };
    Direction /= Direction.VecLength();

    int hitsLeft = 4;

    while (Damage >= 1.0f && hitsLeft > 0) {
        trace_t Trace;
        CTraceFilter Filter(G::LocalPlayer);
        Ray_t Ray(Start, Destination);
        I::enginetrace->TraceRay(Ray, 0x4600400B, &Filter, &Trace);

        if (!AllowFriendlyFire && Trace.Entity && Trace.Entity->IsPlayer() && (G::LocalPlayer->GetTeam() == Trace.Entity->GetTeam()))
            return false;

        if (Trace.Fraction == 1.0f || (Trace.Entity == Ent && Trace.Hitgroup > HITGROUP_GENERIC&& Trace.Hitgroup <= HITGROUP_RIGHTLEG)) {
            Damage = GetDamageMultiplier(Hitgroup) * Damage * powf(WeaponData->RangeModifier, Trace.Fraction * WeaponData->Range / 500.0f);

            float ArmorRatio = WeaponData->ArmorRatio / 2.0f;
            if (IsArmored(Hitgroup, Ent->HasHelmet()))
                Damage -= (Ent->ArmorVal() < Damage * ArmorRatio / 2.0f ? Ent->ArmorVal() * 4.0f : Damage) * (1.0f - ArmorRatio);

            return Damage >= MinDamage;
        }

        const auto SurfaceData = I::physicssurfaceprops->getSurfaceData(Trace.Surface.SurfaceProps);

        if (SurfaceData->PenetrationModifier < 0.1f)
            break;


        Damage = HandleBulletPenetration(SurfaceData, Trace, Direction, Start, WeaponData->Penetration, Damage);
        hitsLeft--;
    }
    return false;
}
