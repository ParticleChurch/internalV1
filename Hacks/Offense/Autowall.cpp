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
            return false;

        if (Trace.Fraction == 1.0f)
            break;

        if (Trace.Hitgroup > HITGROUP_GENERIC && Trace.Hitgroup <= HITGROUP_RIGHTLEG 
            && Trace.hitbox == hitbox
            && Trace.Entity->GetTeam() != G::LocalPlayerTeam) {
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
        I::enginetrace->TraceRay(Ray, 0x4600400B, &Filter, &Trace);

        if (!AllowFriendlyFire && Trace.Entity && Trace.Entity->IsPlayer() && (G::LocalPlayer->GetTeam() == Trace.Entity->GetTeam()))
            return false;

        // We have reached end of trace and therefore can hit it
        L::Verbose("Checking trace fraction 1");
        if (Trace.Fraction > 0.97f && (!Trace.Entity || Trace.Entity->GetTeam() != G::LocalPlayerTeam))
        {
            H::console.push_back("trace fraction 1");
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

    // if it is autowallable
    int hitsLeft = 4;
    while (Damage >= 1.0f && hitsLeft > 0) {
        trace_t Trace;
        CTraceFilter Filter(G::LocalPlayer);
        Ray_t Ray(Start, point);
        I::enginetrace->TraceRay(Ray, 0x4600400B, &Filter, &Trace);

        if (!AllowFriendlyFire && Trace.Entity && Trace.Entity->IsPlayer() && (G::LocalPlayer->GetTeam() == Trace.Entity->GetTeam()))
            return false;

        // We have reached end of trace and therefore can hit it
        if (Trace.Fraction == 1.0f)
            return false;

        // We've hit an enemy hitbox, and therefore can't hit point? can hit? idk... prob should should ig
        if (Trace.Hitgroup > HITGROUP_GENERIC && Trace.Hitgroup <= HITGROUP_RIGHTLEG) {
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
