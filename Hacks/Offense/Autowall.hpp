#pragma once

class Autowall
{
public:
	float GetDamageMultiplier(int HitGroup);
	bool IsArmored(int HitGroup, bool Helmet);
	std::add_pointer_t<bool __cdecl(float, float, float, float, float, float, trace_t&)> TraceToExit = reinterpret_cast<decltype(TraceToExit)>(FindPattern("client", "55 8B EC 83 EC 30 F3 0F 10 75"));
	float HandleBulletPenetration(SurfaceData* EnterSurfaceData, const trace_t& EnterTrace, const Vec& Direction, Vec& Result, float Penetration, float Damage);

	void TraceLine(Vec& start, Vec& end, unsigned int mask, Entity* ignore, trace_t* tr);
public:
	bool IsVisible(Vec Start, Vec End, Entity* Ent);
	bool IsVisible(Vec End, Entity* Ent);

	float Damage(const Vec& point, int hitbox, bool AllowFriendlyFire = false );
	float Damage(Vec start, const Vec& point, bool AllowFriendlyFire = false);
	bool CanHitFloatingPoint(const Vec& point, bool AllowFriendlyFire = false);
	bool CanHitFloatingPoint(Vec start, const Vec& point, bool AllowFriendlyFire = false);
	bool CanWallbang(float& dmg);
};

extern Autowall* autowall;



struct FireBulletData
{
	FireBulletData(const Vec& eyePos, Entity* entity) : src(eyePos), filter(entity)
	{
	}

	Vec          src;
	trace_t         enter_trace;
	Vec          direction;
	CTraceFilter   filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
};

// credits to 
class Autowall2
{
private:
	void TraceLine(Vec& start, Vec& end, unsigned int mask, Entity* ignore, trace_t* tr);
	bool VectortoVectorVisible(Vec src, Vec point);
	bool HandleBulletPenetration(WeaponData* wpn_data, FireBulletData& data, bool extracheck);
	bool TraceToExitalt(Vec& end, trace_t& tr, Vec start, Vec vEnd, trace_t* trace);
	bool SimulateFireBullet(Entity* local, Entity* weapon, FireBulletData& data);
	void UTIL_ClipTraceToPlayers(const Vec& vecAbsStart, const Vec& vecAbsEnd, unsigned int mask, CTraceFilter* filter, trace_t* tr);
	void ScaleDamage(int hitgroup, Entity* enemy, float weapon_armor_ratio, float& current_damage);
	float GetHitgroupDamageMult(int iHitGroup);
public:
	Vec aim;
	float Damage(const Vec& point);
	bool CanHitFloatingPoint(const Vec& point, const Vec& source);
	//bool CanWallbang(float& dmg);
};

extern Autowall2* autowall2;