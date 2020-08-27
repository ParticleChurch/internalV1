#pragma once
class Entity
{
public:
	float* pGetFlashMaxAlpha()
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
		return (float*)((DWORD)this + offset);
	}

	void SetEntitySpotted(BYTE val) {
		static DWORD offset = N::GetOffset("DT_BaseEntity", "m_bSpotted");
		if (offset == 0)
			offset = N::GetOffset("DT_BaseEntity", "m_bSpotted");
		*(BYTE*)((DWORD)this + offset) = val;
	}

	float GetInaccuracy()
	{
		typedef float(__thiscall* oGetInaccuracy)(void*);
		return GetVFunc<oGetInaccuracy>(this, 482)(this);
	}

	WeaponData* GetWeaponData() {
		typedef WeaponData* (__thiscall* ogetWeaponData)(void*);
		return GetVFunc<ogetWeaponData>(this, 460)(this);
	}

	WeaponId GetWeaponId() { //get active weapon entity then call
		static DWORD offset = N::GetOffset("DT_BaseCombatWeapon", "m_iItemDefinitionIndex");
		if (offset == 0)
			offset = N::GetOffset("DT_BaseCombatWeapon", "m_iItemDefinitionIndex");
		return *(WeaponId*)((DWORD)this + offset);
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
		return Vec(0,0,0);
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

	Vec GetEyeHeight()
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_vecViewOffset[0]");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_vecViewOffset[0]");

		return *(Vec*)((DWORD)this + offset);
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

	float GetNextAttack()
	{
		static DWORD offset = N::GetOffset("DT_BaseCombatCharacter", "m_flNextAttack");
		if (offset == 0)
			offset = N::GetOffset("DT_BaseCombatCharacter", "m_flNextAttack");
		return *(float*)((DWORD)this + offset);
	}

	float GetNextPrimaryAttack() {
		static DWORD offset = N::GetOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
		if (offset == 0)
			offset = N::GetOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
		return *(float*)((DWORD)this + offset);
	}

	float GetNextSecondaryAttack() {
		static DWORD offset = N::GetOffset("DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
		if (offset == 0)
			offset = N::GetOffset("DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
		return *(float*)((DWORD)this + offset);
	}

	float GetGrenadeThrowTime() {
		static DWORD offset = N::GetOffset("DT_BaseCSGrenade", "m_fThrowTime");
		if (offset == 0)
			offset = N::GetOffset("DT_BaseCSGrenade", "m_fThrowTime");
		return *(float*)((DWORD)this + offset);
	}

	float GetServerTime() {
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_nTickBase");
		if (offset == 0)
			offset = N::GetOffset("DT_BasePlayer", "m_nTickBase");
		return ((*(int*)((DWORD)this + offset)) * I::globalvars->m_intervalPerTick);
	}

	int GetTickBase() {
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_nTickBase");
		if (offset == 0)
			offset = N::GetOffset("DT_BasePlayer", "m_nTickBase");
		return *(int*)((DWORD)this + offset);
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
		return this->GetActiveWeapon()->GetNextPrimaryAttack() <= GetServerTime();
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

	float MaxAccurateSpeed() 
	{ 
		const WeaponData* WeaponData = this->GetActiveWeapon()->GetWeaponData();
		return (this->IsScoped() ? WeaponData->MaxSpeedAlt : WeaponData->MaxSpeed); //alt and regular might be flipped lol
	}

	float GetLBY() //LowerBodyYawTarget
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		return *(float*)((DWORD)this + offset);
	}

	Vec GetEyeAngles() //GetEyeAngles
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_angEyeAngles[0]");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_angEyeAngles[0]");
		Vec a = *(Vec*)((DWORD)this + offset);
		if (a.x > 89)
			a.x = 360 - a.x;
		return a;
	}

	void SetEyeAngles(QAngle angle)
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_angEyeAngles[0]");
		if (offset == 0)
			offset = N::GetOffset("DT_CSPlayer", "m_angEyeAngles[0]");
		*(QAngle*)((DWORD)this + offset) = angle;
	}

	float GetSimulationTime() 
	{
		static DWORD offset = N::GetOffset("DT_BaseAnimating", "m_flSimulationTime");
		if (offset == 0)
			offset = N::GetOffset("DT_BaseAnimating", "m_flSimulationTime");
		return *(float*)((DWORD)this + offset);
	}

	AnimState* GetAnimstate() noexcept
	{
		return *reinterpret_cast<AnimState**>(this + 0x3914);
	}

	float GetMaxDesyncAngle() noexcept
	{
		const auto animState = GetAnimstate();

		if (!animState)
			return 0.0f;

		float yawModifier = (animState->stopToFullRunningFraction * -0.3f - 0.2f) * std::clamp(animState->footSpeed, 0.0f, 1.0f) + 1.0f;

		if (animState->duckAmount > 0.0f)
			yawModifier += (animState->duckAmount * std::clamp(animState->footSpeed2, 0.0f, 1.0f) * (0.5f - yawModifier));

		return animState->velocitySubtractY * yawModifier;
	}

	bool LBYUpdated()
	{
		static int LastLBY = -1;
		int CurLBY = (int)GetLBY();
		if (CurLBY != LastLBY)
		{
			LastLBY = CurLBY;
			return true;
		}
		return false;
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

	bool IsDormant()
	{
		typedef bool(__thiscall* oIsDormant)(void*);
		return GetVFunc<oIsDormant>(this + 8, 9)(this + 8);
	}

	int Index() {
		typedef int(__thiscall* oGetIndex)(void*);
		return GetVFunc<oGetIndex>(this + 8, 10)(this + 8);
	}

	Model* GetModel()
	{
		typedef Model* (__thiscall* oGetModel)(void*);
		return GetVFunc<oGetModel>(this + 4, 8)(this + 4);
	}

	mstudiobbox_t* GetHitBoxSet(int Hitbox)
	{
		studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(GetModel());
		if (StudioModel)
			return StudioModel->GetHitboxSet(0)->GetHitbox(Hitbox);
		else
			return NULL;
	}

	//big old cube formed by a1, b1, c1, d1, a2, b2, c2, d2...
	void GetHitboxEnds(int Hitbox, Vec& a1, Vec& b1, Vec& c1, Vec& d1, Vec& a2, Vec& b2, Vec& c2, Vec& d2)
	{
		Matrix3x4 BoneMatrix[MAXSTUDIOBONES];
		if (SetupBones(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
			studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(GetModel());
			if (StudioModel)
			{
				mstudiobbox_t* hitbox = StudioModel->GetHitboxSet(0)->GetHitbox(Hitbox);
				if (hitbox) {
					Vec min = hitbox->bbmin.Transform(BoneMatrix[hitbox->bone]);
					Vec max = hitbox->bbmax.Transform(BoneMatrix[hitbox->bone]);
					
					//bottom
					a1.x = min.x - hitbox->m_flRadius;
					b1.x = min.x - hitbox->m_flRadius;
					c1.x = min.x + hitbox->m_flRadius;
					d1.x = min.x + hitbox->m_flRadius;

					a1.y = min.y - hitbox->m_flRadius;
					b1.y = min.y + hitbox->m_flRadius;
					c1.y = min.y - hitbox->m_flRadius;
					d1.y = min.y + hitbox->m_flRadius;

					a1.z = min.z - hitbox->m_flRadius;
					b1.z = min.z - hitbox->m_flRadius;
					c1.z = min.z - hitbox->m_flRadius;
					d1.z = min.z - hitbox->m_flRadius;

					//top
					a2.x = max.x - hitbox->m_flRadius;
					b2.x = max.x - hitbox->m_flRadius;
					c2.x = max.x + hitbox->m_flRadius;
					d2.x = max.x + hitbox->m_flRadius;

					a2.y = max.y - hitbox->m_flRadius;
					b2.y = max.y + hitbox->m_flRadius;
					c2.y = max.y - hitbox->m_flRadius;
					d2.y = max.y + hitbox->m_flRadius;

					a2.z = max.z - hitbox->m_flRadius;
					b2.z = max.z - hitbox->m_flRadius;
					c2.z = max.z - hitbox->m_flRadius;
					d2.z = max.z - hitbox->m_flRadius;
				}
			}
		}
	}

	Vec GetBone(int Hitbox)
	{
		Matrix3x4 BoneMatrix[MAXSTUDIOBONES];
		if (SetupBones(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
			studiohdr_t* StudioModel = I::modelinfo->GetStudioModel(GetModel());
			if (StudioModel)
			{
				mstudiobbox_t* hitbox = StudioModel->GetHitboxSet(0)->GetHitbox(Hitbox);
				if (hitbox) {
					Vec min = hitbox->bbmin.Transform(BoneMatrix[hitbox->bone]);
					Vec max = hitbox->bbmax.Transform(BoneMatrix[hitbox->bone]);
					return (max + min) / 2.f;
				}
			}
		}
		return Vec();
	}

	Vec GetLeft(Vec C, float radius, Entity* ent)
	{
		Vec P = ent->GetEyePos();	//player
		float gamma = (M_PI / 2.f) + atan2f(C.y - P.y, C.x - P.x);
		float distance = sqrtf(pow((C.x - P.x), 2) + pow((C.y - P.y), 2));
		float beta = asin(radius / distance);

		return Vec(C.x + radius * cosf(gamma + beta), C.y + radius * sinf(gamma + beta), C.z);
	}

	Vec GetRight(Vec C, float radius, Entity* ent)
	{
		Vec P = ent->GetEyePos();	//player
		float gamma = (M_PI / 2.f) + atan2f(C.y - P.y, C.x - P.x);
		float distance = sqrtf(pow((C.x - P.x), 2) + pow((C.y - P.y), 2));
		float beta = asin(radius / distance);

		return Vec(C.x - radius * cosf(gamma - beta), C.y - radius * sinf(gamma - beta), C.z);
	}

};