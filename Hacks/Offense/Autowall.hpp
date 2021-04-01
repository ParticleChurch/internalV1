#pragma once

class Autowall
{
private:
	float GetDamageMultiplier(int HitGroup);
	bool IsArmored(int HitGroup, bool Helmet);
	std::add_pointer_t<bool __cdecl(float, float, float, float, float, float, trace_t&)> TraceToExit = reinterpret_cast<decltype(TraceToExit)>(FindPattern("client", "55 8B EC 83 EC 30 F3 0F 10 75"));
	float HandleBulletPenetration(SurfaceData* EnterSurfaceData, const trace_t& EnterTrace, const Vec& Direction, Vec& Result, float Penetration, float Damage);

public:
	bool IsVisible(Vec Start, Vec End, Entity* Ent);
	bool IsVisible(Vec End, Entity* Ent);

	float Damage(const Vec& point, bool AllowFriendlyFire = false);
	bool CanHitFloatingPoint(const Vec& point, bool AllowFriendlyFire = false);
	bool CanWallbang(float& dmg);
};

extern Autowall* autowall;