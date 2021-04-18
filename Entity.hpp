#pragma once

class Collideable {
public:
	virtual void* pad() = 0;
	virtual const Vec& obbMins() = 0;
	virtual const Vec& obbMaxs() = 0;
	int get_solid()
	{
		typedef int(__thiscall* oget_solid)(void*);
		return GetVFunc<oget_solid>(this, 11)(this);
	}
};

using CBaseHandle = uint32_t;
class Entity
{
public:
	//2 should be GetRefEHandle
	CBaseHandle& GetRefEHandle()
	{
		typedef CBaseHandle& (__thiscall* oGetRefEHandle)(void*);
		return GetVFunc<oGetRefEHandle>(this, 2)(this);
	}

	// FUCKING ANIMATIONS
	void CreateState(AnimState* state)
	{
		static auto CreateAnimState = reinterpret_cast<void(__thiscall*)(AnimState*, Entity*)>(FindPattern("client.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46"));
		if (!CreateAnimState)
			return;

		CreateAnimState(state, this);
	}

	void ResetAnimationState(AnimState* state)
	{
		if (!state)
			return;

		static auto ResetAnimState = reinterpret_cast<void(__thiscall*)(AnimState*)>(FindPattern("client.dll", "56 6A 01 68 ? ? ? ? 8B F1"));
		if (!ResetAnimState)
			return;

		ResetAnimState(state);
	}
	// Thirdperson crap
	int* GetObserverTarget()
	{
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_hObserverTarget");
		return (int*)((DWORD)this + offset);
	}

	int* GetObserverMode()
	{
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_iObserverMode");
		return (int*)((DWORD)this + offset);
	}

	const Matrix3x4& GetTransform()
	{
		typedef const Matrix3x4& (__thiscall* oGetTransform)(void*);
		return GetVFunc<oGetTransform>(this + sizeof(uintptr_t), 3)(this + sizeof(uintptr_t));
	}

	Collideable* GetCollideable() 
	{
		typedef Collideable* (__thiscall* oGetCollideable)(void*);
		return GetVFunc<oGetCollideable>(this, 3)(this);
	}

	float* pGetFlashMaxAlpha()
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
		return (float*)((DWORD)this + offset);
	}

	void SetEntitySpotted(BYTE val) {
		static DWORD offset = N::GetOffset("DT_BaseEntity", "m_bSpotted");
		*(BYTE*)((DWORD)this + offset) = val;
	}

	float GetSpread()
	{
		typedef float(__thiscall* oGetSpread)(void*);
		return GetVFunc<oGetSpread>(this, 452)(this);
	}

	float GetInaccuracy()
	{
		typedef float(__thiscall* oGetInaccuracy)(void*);
		return GetVFunc<oGetInaccuracy>(this, 482)(this);
	}

	float_t m_flSpawnTime()
	{
		return *(float_t*)((uintptr_t)this + 0xA360);
	}

	WeaponData* GetWeaponData() {
		typedef WeaponData* (__thiscall* ogetWeaponData)(void*);
		return GetVFunc<ogetWeaponData>(this, 460)(this);
	}

	WeaponId GetWeaponId() { //get active weapon entity then call
		static DWORD offset = N::GetOffset("DT_BaseCombatWeapon", "m_iItemDefinitionIndex");
		return *(WeaponId*)((DWORD)this + offset);
	}

	HANDLE GetOwner()
	{
		static DWORD offset = N::GetOffset("DT_BaseEntity", "m_hOwnerEntity");
		return *(HANDLE*)((DWORD)this + offset);
	}

	int GetFlags()
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_fFlags");
		return *(int*)((DWORD)this + offset);
	}

	int GetTeam()
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_iTeamNum");
		return *(int*)((DWORD)this + offset);
	}

	Vec GetVecVelocity() {
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_vecVelocity[0]");
		return *(Vec*)((DWORD)this + offset);
	}

	int GetMoveType() {
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_nRenderMode") + 1;
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
		return *(Vec*)((DWORD)this + offset);
	}

	Vec GetEyePos() {
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_vecViewOffset[0]");
		return (GetVecOrigin() + *(Vec*)((DWORD)this + offset));
	}

	Vec GetEyeHeight()
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_vecViewOffset[0]");
		return *(Vec*)((DWORD)this + offset);
	}

	int GetHealth() {
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_iHealth");
		return *(int*)((DWORD)this + offset);
	}

	int GetAmmo() {
		static DWORD offset = N::GetOffset("DT_BaseCombatWeapon", "m_iClip1");
		Entity* weap = this->GetActiveWeapon();
		if (!weap)
			return 0;
		return *(int*)(weap + offset);
	}

	bool GetGunGameImmunity() {
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_bGunGameImmunity");
		return *(int*)((DWORD)this + offset);
	}

	char GetLifeState() {
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_lifeState");
		return *(char*)((DWORD)this + offset);
	}

	float GetNextSecondaryAttack() {
		static DWORD offset = N::GetOffset("DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
		return *(float*)((DWORD)this + offset);
	}

	float GetGrenadeThrowTime() {
		static DWORD offset = N::GetOffset("DT_BaseCSGrenade", "m_fThrowTime");
		return *(float*)((DWORD)this + offset);
	}

	float GetServerTime() {
		return GetTickBase() * I::globalvars->m_intervalPerTick;
	}

	int GetTickBase() {
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_nTickBase");
		return *(int*)((DWORD)this + offset);
	}

	Entity* GetActiveWeapon() {
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_hActiveWeapon");
		return I::entitylist->GetClientEntityFromHandle(*(HANDLE*)((DWORD)this + offset));
	}

	float GetLastShotTime() {
		static DWORD offset = N::GetOffset("DT_WeaponCSBase", "m_fLastShotTime");
		return *(float*)((DWORD)this + offset);
	}

	int* GetViewModelID()
	{
		static DWORD offset = N::GetOffset("DT_BaseCombatWeapon", "m_iViewModelIndex");
		return (int*)((DWORD)this + offset);
	}
	

	//SKINCHANGER VALUES ______________________________________________________________________________
	// When changed, can be used to make a weapon appear as something else, 
	// most commonly used to change the default knives.

	// an array
	unsigned int* GetWeapons()
	{
		static DWORD offset = N::GetOffset("DT_BaseCombatCharacter", "m_hMyWeapons");
		return (unsigned int*)((DWORD)this + offset);
	}

	int* GetModelIndex()
	{
		static DWORD offset = N::GetOffset("DT_BaseEntity", "m_nModelIndex");
		return (int*)((DWORD)this + offset);
	}

	HANDLE GetWeapon()
	{
		static DWORD offset = N::GetOffset("DT_BaseViewModel", "m_hWeapon");
		return *(HANDLE*)((DWORD)this + offset);
	}

	HANDLE GetViewModel() {
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_hViewModel[0]");
		return *(HANDLE*)((DWORD)this + offset);
	}

	int* GetItemDefinitionIndex() {
		static DWORD offset = N::GetOffset("DT_BaseAttributableItem", "m_iItemDefinitionIndex");
		return (int*)((DWORD)this + offset);
	}

	char* GetCustomName() {
		static DWORD offset = N::GetOffset("DT_BaseAttributableItem", "m_szCustomName");
		return (char*)((DWORD)this + offset);
	}

	// When set to a non-zero value, fallback values will be used.
	int* GetItemIDHigh() {
		static DWORD offset = N::GetOffset("DT_BaseAttributableItem", "m_iItemIDHigh");
		return (int*)((DWORD)this + offset);
	}

	// Defines the quality of this weapon. Qualities 4 and 11 are the same as 0.
	// Knives always use quality 3 which makes the star appear.
	int* GetEntityQuality() {
		static DWORD offset = N::GetOffset("DT_BaseAttributableItem", "m_iEntityQuality");
		return (int*)((DWORD)this + offset);
	}

	// This and m_OriginalOwnerXuidHigh contain the XUIDs of the player who originally purchased this weapon.
	int* GetOriginalOwnerXuidLow()
	{
		static DWORD offset = N::GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
		return (int*)((DWORD)this + offset);
	}

	// This and m_OriginalOwnerXuidLow contain the XUIDs of the player who originally purchased this weapon.
	int* GetOriginalOwnerXuidHigh()
	{
		static DWORD offset = N::GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
		return (int*)((DWORD)this + offset);
	}
	/*
	The amount of recorded kills on the weapons StatTrak counter. When set to -1 the StatTrak counter will not appear.
	Setting this to 0 or above will automatically enable the StatTrak counter.
	Note: The kill counter will not increment automatically.
	If you are having issues with the StatTrak counter appearing as USER ERROR, set m_iAccountID to your XUID Low value
	*/
	int* GetFallbackStatTrak()
	{
		static DWORD offset = N::GetOffset("DT_BaseAttributableItem", "m_nFallbackStatTrak");
		return (int*)((DWORD)this + offset);
	}

	int* GetFallbackOriginalOwnerXuidLow()
	{
		static DWORD offset = N::GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
		return (int*)((DWORD)this + offset);
	}

	int* GetFallbackOriginalOwnerXuidHigh()
	{
		static DWORD offset = N::GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
		return (int*)((DWORD)this + offset);
	}

	// The ID of the paint kit to apply. All valid IDs can be found in the csgo/scripts/items/items_game.txt file.
	int* GetFallbackPaintKit()
	{
		static DWORD offset = N::GetOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit");
		return (int*)((DWORD)this + offset);
	}

	// Float value ranging from 0 to 1 used to determine how worn the paint on the weapon is.
	float* GetFallbackWear()
	{
		static DWORD offset = N::GetOffset("DT_BaseAttributableItem", "m_flFallbackWear");
		return (float*)((DWORD)this + offset);
	}

	// Used to randomize the pattern on the weapon for supported paint kits such as Fade, Crimson Web, Case Hardened, etc.
	int* GetFallbackSeed()
	{
		static DWORD offset = N::GetOffset("DT_BaseAttributableItem", "m_nFallbackSeed");
		return (int*)((DWORD)this + offset);
	}

	float* GetAnimTime()
	{
		static DWORD offset = N::GetOffset("DT_BaseEntity", "m_flAnimTime");
		return (float*)((DWORD)this + offset);
	}

	int* GetAnimSequence()
	{
		static DWORD offset = N::GetOffset("DT_BaseViewModel", "m_nSequence");
		return (int*)((DWORD)this + offset);
	}

	// END SKINCHANGER VALUES ______________________________________________________________________________

	float GetNextAttack()
	{
		static DWORD offset = N::GetOffset("DT_BaseCombatCharacter", "m_flNextAttack");
		return *(float*)((DWORD)this + offset);
	}

	float GetNextPrimaryAttack() {
		static DWORD offset = N::GetOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
		return *(float*)((DWORD)this + offset);
	}

	//basically return false if still getting weapon out, otherwise true
	bool CanShoot2() {
		if (!this->GetActiveWeapon())
			return false;
		return this->GetNextAttack() <= GetServerTime();
	}

	bool CanShoot() {
		Entity* weap = this->GetActiveWeapon();
		if (!weap)
			return false;
		float ServerTime = this->GetServerTime();
		return this->GetNextAttack() <= ServerTime && weap->GetNextPrimaryAttack() <= ServerTime;
	}

	Vec GetAimPunchAngle() {
		static DWORD offset = N::GetOffset("DT_BasePlayer", "m_aimPunchAngle");
		return *(Vec*)((DWORD)this + offset);
	}

	int GetShotsFired() {
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_iShotsFired");
		return *(int*)((DWORD)this + offset);
	}

	float GetFireReadyTime()
	{
		static DWORD offset = N::GetOffset("DT_WeaponCSBase", "m_flPostponeFireReadyTime");
		return *(float*)((DWORD)this + offset);
	}

	float GetFlashMaxAlpha()
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
		return *(float*)((DWORD)this + offset);
	}

	bool HasHelmet() {
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_bHasHelmet");
		return *(float*)((DWORD)this + offset);
	}

	int ArmorVal() {
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_ArmorValue");
		return *(int*)((DWORD)this + offset);
	}

	bool IsScoped() {
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_bIsScoped");
		if((bool*)((DWORD)this + offset))
			return *(bool*)((DWORD)this + offset);
		return false;
	}

	int ScopeLevel() {
		static DWORD offset = N::GetOffset("DT_WeaponCSBaseGun", "m_zoomLevel");
		return *(int*)((DWORD)this + offset);
	}

	float MaxAccurateSpeed() 
	{ 
		Entity* weap = this->GetActiveWeapon();
		if (!weap) return 0.f;
		const WeaponData* WeaponData = weap->GetWeaponData();
		return (this->IsScoped() ? WeaponData->MaxSpeedAlt : WeaponData->MaxSpeed); //alt and regular might be flipped lol
	}

	float GetLBY() //LowerBodyYawTarget
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		return *(float*)((DWORD)this + offset);
	}

	Vec GetEyeAngles() //GetEyeAngles
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_angEyeAngles[0]");
		Vec a = *(Vec*)((DWORD)this + offset);
		if (a.x > 89)
			a.x = 360 - a.x;
		return a;
	}

	Vec* PGetEyeAngles() //GetEyeAngles
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_angEyeAngles[0]");
		Vec* a = (Vec*)((DWORD)this + offset);
		a->Normalize();
		return a;
	}

	void SetEyeAngles(QAngle angle)
	{
		static DWORD offset = N::GetOffset("DT_CSPlayer", "m_angEyeAngles[0]");
		*(QAngle*)((DWORD)this + offset) = angle;
	}

	Vec& GetAbsAngles()
	{
		typedef Vec& (__thiscall* OriginalFn)(void*);
		static OriginalFn f = GetVFunc<OriginalFn>(this, 11);
		return f(this);
	}

	float GetSimulationTime() 
	{
		static DWORD offset = N::GetOffset("DT_BaseAnimating", "m_flSimulationTime");
		return *(float*)((DWORD)this + offset);
	}

	AnimState* GetAnimstate() noexcept
	{
		return *reinterpret_cast<AnimState**>(this + 0x3914);
	}

	AnimState2* GetAnimstate2() noexcept
	{
		return *reinterpret_cast<AnimState2**>(this + 0x3914);
	}

	float GetMaxDesyncAngle() noexcept
	{
		const auto animState = GetAnimstate2();

		if (!animState)
			return 0.0f;

		float yawModifier = (animState->stopToFullRunningFraction * -0.3f - 0.2f) * std::clamp(animState->footSpeed, 0.0f, 1.0f) + 1.0f;

		if (animState->duckAmount > 0.0f)
			yawModifier += (animState->duckAmount * std::clamp(animState->footSpeed2, 0.0f, 1.0f) * (0.5f - yawModifier));

		return fabsf(animState->velocitySubtractY * yawModifier);
	}

	std::array< float, 24 >& m_flPoseParameter()
	{
		static DWORD offset = N::GetOffset("DT_BaseAnimating", "m_flPoseParameter");
		return *reinterpret_cast<std::array<float, 24>*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	AnimationLayer* GetAnimOverlays()
	{
		return *(AnimationLayer**)((DWORD)this + 0x2990);
	}

	AnimationLayer* GetAnimOverlay(int i)
	{
		if (i < 15)
			return &GetAnimOverlays()[i];
		return nullptr;
	}

	bool* ClientAnimations()
	{
		static DWORD offset = N::GetOffset("DT_BaseAnimating", "m_bClientSideAnimation");
		return (bool*)(DWORD(this) + offset);
	}

	void UpdateAnimationState(AnimState* state, Vec angle)
	{
		static auto update_anim_state = FindPattern("client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24");

		if (!update_anim_state)
			return;

		__asm
		{
			push 0
		}

		__asm
		{
			mov ecx, state

			movss xmm1, dword ptr[angle + 4]
			movss xmm2, dword ptr[angle]

			call update_anim_state
		}
	}

	void UpdateClientSideAnimation()
	{
		typedef void(__thiscall* oUpdateClientSideAnimation)(void*);
		static auto func = GetVFunc<oUpdateClientSideAnimation>(this, 223);
		func(this);
	}

	void SetAbsAngles(Vec angle)
	{
		using SetAbsAnglesFn = void(__thiscall*)(void*, const Vec& Angles);
		static SetAbsAnglesFn SetAbsAngles = (SetAbsAnglesFn)FindPattern("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8");
		SetAbsAngles(this, angle);
	}

	bool IsAlive()
	{
		return this->GetHealth() > 0 && this->GetLifeState() == 0;
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
			return nullptr;
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