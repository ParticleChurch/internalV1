#pragma once
enum MoveType {
	NOCLIP = 8,
	LADDER = 9
};

class Entity
{
public:
	void SetEntitySpotted(BYTE val) {
		static DWORD offset = N::GetOffset("DT_BaseEntity", "m_bSpotted");
		if (offset == 0)
			offset = N::GetOffset("DT_BaseEntity", "m_bSpotted");
		*(BYTE*)((DWORD)this + offset) = val;
	}

	WeaponData* GetWeaponData() {
		typedef WeaponData* (__thiscall* ogetWeaponData)(void*);
		return GetVFunc<ogetWeaponData>(this, 460)(this);
	}

	void* self() {
		return this;
	}

	int GetFlags()
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_fFlags");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_fFlags");
		return *(int*)((DWORD)this + offset);
	}

	int GetTeam()
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_iTeamNum");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_iTeamNum");
		return *(int*)((DWORD)this + offset);
	}

	Vec GetVecVelocity() {
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_vecVelocity[0]");
		if (offset == 0)
			offset = N::GetOffset("DT_BasePlayer", "m_vecVelocity[0]");
		return *(Vec*)((DWORD)this + offset);
	}

	int GetMoveType() {
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_nRenderMode") + 1;
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_nRenderMode") + 1;
		return *(int*)((DWORD)this + offset);
	}

	bool SetupBones(Matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime = 0)
	{
		__asm
		{
			mov edi, this
			lea ecx, dword ptr ds : [edi + 0x4]
			mov edx, dword ptr ds : [ecx]
			push currentTime
			push boneMask
			push nMaxBones
			push pBoneToWorldOut
			call dword ptr ds : [edx + 0x34]
		}
	}

	Vec GetBonePos(int boneID) {
		Matrix3x4 boneMatrixes[128];
		if (SetupBones(boneMatrixes, 128, 0x100, 0))
			return Vec(boneMatrixes[boneID][0][3], boneMatrixes[boneID][1][3], boneMatrixes[boneID][2][3]);
	}

	Vec GetVecOrigin()
	{
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_vecOrigin");
		if (offset == 0)
			offset = N::GetOffset("DT_BasePlayer", "m_vecOrigin");
		return *(Vec*)((DWORD)this + offset);
	}

	Vec GetEyePos() {
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_vecViewOffset[0]");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_vecViewOffset[0]");
		return (GetVecOrigin() + *(Vec*)((DWORD)this + offset));
	}

	int GetHealth() {
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_iHealth");
		if (offset == 0)
			offset = N::GetOffset("DT_BasePlayer", "m_iHealth");
		return *(int*)((DWORD)this + offset);
	}

	int GetAmmo() {
		static DWORD offset = N::GetOffset("DT_BaseCombatWeapon", "m_iClip1");
		if (offset == 0)
			offset = N::GetOffset("DT_BaseCombatWeapon", "m_iClip1");
		if (!this || !this->GetActiveWeapon())
			return false;
		return *(int*)(this->GetActiveWeapon() + offset);
	}

	bool GetGunGameImmunity() {
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_bGunGameImmunity");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_bGunGameImmunity");
		return *(int*)((DWORD)this + offset);
	}

	int GetLifeState() {
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_lifeState");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_lifeState");
		return *(int*)((DWORD)this + offset);
	}

	float NextPrimaryAttack() {
		static DWORD offset = N::GetOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
		if (offset == 0)
			offset = N::GetOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
		return *(float*)((DWORD)this + offset);
	}

	float GetSimulationTime() {
		static DWORD offset = N::GetOffset("CBaseEntity", "m_flSimulationTime");
		if (offset == 0)
			offset = N::GetOffset("CBaseEntity", "m_flSimulationTime");
		return *(float*)((DWORD)this + offset);
	}

	float GetServerTime() {
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_nTickBase");
		if (offset == 0)
			offset = N::GetOffset("DT_BasePlayer", "m_nTickBase");
		return ((*(int*)((DWORD)this + offset)) * I::globalvars->m_intervalPerTick);
	}

	Entity* GetActiveWeapon() {
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_hActiveWeapon");
		if (offset == 0)
			offset = N::GetOffset("DT_BasePlayer", "m_hActiveWeapon");
		static HANDLE weaponHandle;
		weaponHandle = *(HANDLE*)((DWORD)this + offset);
		return I::entitylist->GetClientEntityFromHandle(weaponHandle);
	}

	bool CanShoot() {
		if (!this || !this->GetActiveWeapon())
			return false;
		return this->GetActiveWeapon()->NextPrimaryAttack() <= GetServerTime();
	}

	Vec GetAimPunchAngle() {
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_aimPunchAngle");
		if (offset == 0)
			offset = N::GetOffset("DT_BasePlayer", "m_aimPunchAngle");
		return *(Vec*)((DWORD)this + offset);
	}

	int GetShotsFired() {
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_iShotsFired");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_iShotsFired");
		return *(int*)((DWORD)this + offset);
	}

	float GetFlashMaxAlpha()
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
		return *(float*)((DWORD)this + offset);
	}

	bool HasHelmet() {
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_bHasHelmet");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_bHasHelmet");
		return *(float*)((DWORD)this + offset);
	}

	int ArmorVal() {
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_ArmorValue");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_ArmorValue");
		return *(int*)((DWORD)this + offset);
	}

	bool IsScoped() {
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_bIsScoped");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_bIsScoped");
		return *(bool*)((DWORD)this + offset);
	}

	float MaxAccurateSpeed() {
		const WeaponData* weaponData = this->GetActiveWeapon()->GetWeaponData();
		return (this->IsScoped() ? weaponData->maxSpeedAlt : weaponData->maxSpeed) / 3;
	}

	bool IsAlive()
	{
		typedef bool(__thiscall* oAlive)(void*);
		return GetVFunc<oAlive>(this, 155)(this);
	}

	bool IsPlayer()
	{
		typedef bool(__thiscall* oPlayer)(void*);
		return GetVFunc<oPlayer>(this, 157)(this);
	}

	ClientClass* GetClientClass()
	{
		typedef ClientClass* (__thiscall* oGetClientClass)(void*);
		return GetVFunc<oGetClientClass>(this, 2)(this);
	}

	bool IsDormant() {
		return *(bool*)((DWORD)this + (DWORD)0xED);
	}

	int index() {
		typedef int(__thiscall* oGetIndex)(void*);
		return GetVFunc<oGetIndex>(this, 10)(this + 8);
	}
};