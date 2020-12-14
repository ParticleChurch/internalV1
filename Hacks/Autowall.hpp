#pragma once

class Autowall
{
private:
	float GetDamageMultiplier(int HitGroup);
	bool IsArmored(int HitGroup, bool Helmet);
	std::add_pointer_t<bool __cdecl(float, float, float, float, float, float, trace_t&)> TraceToExit = reinterpret_cast<decltype(TraceToExit)>(G::TraceToExitPattern);
	float HandleBulletPenetration(SurfaceData* EnterSurfaceData, const trace_t& EnterTrace, const Vec& Direction, Vec& Result, float Penetration, float Damage);
public:
	bool CanScan(Entity* Ent, const Vec& Destination, const WeaponData* WeaponData, int MinDamage, bool AllowFriendlyFire);
	bool CanScanBacktrack(Entity* Ent, const Vec& Destination, const WeaponData* WeaponData, int MinDamage, bool AllowFriendlyFire, int Hitgroup);

	bool IsVisible(Vec Start, Vec End, Entity* Ent);
	bool IsVisible(Vec End, Entity*); //FROM LOCAL PLAYER HEAD POS

	float GetDamage(Entity* Ent, const Vec& Destination, bool AllowFriendlyFire, bool& visible);
};

extern Autowall* autowall;