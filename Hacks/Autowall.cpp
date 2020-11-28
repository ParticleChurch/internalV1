#include "../Include.hpp"

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

float Autowall::GetDamage(Entity* Ent, const Vec& Destination, bool AllowFriendlyFire, bool& visible)
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

        if (Trace.Entity == Ent && Trace.Hitgroup > HITGROUP_GENERIC && Trace.Hitgroup <= HITGROUP_RIGHTLEG) {
            Damage = GetDamageMultiplier(Trace.Hitgroup) * Damage * powf(WeaponData->RangeModifier, Trace.Fraction * WeaponData->Range / 500.0f);

            float ArmorRatio = WeaponData->ArmorRatio / 2.0f;
            if (IsArmored(Trace.Hitgroup, Trace.Entity->HasHelmet()))
                Damage -= (Trace.Entity->ArmorVal() < Damage * ArmorRatio / 2.0f ? Trace.Entity->ArmorVal() * 4.0f : Damage) * (1.0f - ArmorRatio);
            if (hitsLeft == 4)
                visible = true;
            else
                visible = false;
            return Damage;
        }

        const auto SurfaceData = I::physicssurfaceprops->getSurfaceData(Trace.Surface.SurfaceProps);

        if (SurfaceData->PenetrationModifier < 0.1f)
            break;


        Damage = HandleBulletPenetration(SurfaceData, Trace, Direction, Start, WeaponData->Penetration, Damage);
        hitsLeft--;
    }
    visible = false;
    return 0;
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
