#include "../../Include.hpp"

AnimFix* animfix = new AnimFix();

void AnimFix::UpdateReal()
{
    if (!G::LocalPlayer)
        return;

    AnimState* animstate = G::LocalPlayer->GetAnimstate();

    if (!animstate)
        return;

    if (G::LocalPlayer->GetHealth() <= 0 || !I::input->m_fCameraInThirdPerson)
    {
        G::LocalPlayer->UpdateClientSideAnimation();
        return;
    }

    static AnimationLayer backup_layers[15];
    if (!backup_layers)
        std::memcpy(backup_layers, G::LocalPlayer->GetAnimOverlays(), (sizeof(AnimationLayer) * 15));

    static float proper_abs = animstate->m_flGoalFeetYaw;
    static std::array<float, 24> sent_pose_params = G::LocalPlayer->m_flPoseParameter();

    if (G::LocalPlayer->GetAnimstate()->m_iLastClientSideAnimationUpdateFramecount == I::globalvars->m_frameCount)
        G::LocalPlayer->GetAnimstate()->m_iLastClientSideAnimationUpdateFramecount -= 1;

    static int old_tick = 0;
    if (I::globalvars->m_tickCount != old_tick) // Only update animations each tick, though we are doing this each frame.
    {
        old_tick = I::globalvars->m_tickCount;
        std::memcpy(backup_layers, G::LocalPlayer->GetAnimOverlays(), (sizeof(AnimationLayer) * 15));
        *G::LocalPlayer->ClientAnimations() = true; //just does stuff like set m_bClientSideAnimation and m_iLastAnimUpdateFrameCount
        G::LocalPlayer->UpdateAnimationState(G::LocalPlayer->GetAnimstate(), ViewAng);
        if (SendPacket)
        {
            proper_abs = animstate->m_flGoalFeetYaw;
            sent_pose_params = G::LocalPlayer->m_flPoseParameter();
        }
    }
    *G::LocalPlayer->ClientAnimations() = false;
    G::LocalPlayer->SetAbsAngles(Vec(0, proper_abs, 0));
    G::LocalPlayer->GetAnimstate()->m_flUnknownFraction = 0.f; // Lol.
    std::memcpy(G::LocalPlayer->GetAnimOverlays(), backup_layers, (sizeof(AnimationLayer) * 15));
    G::LocalPlayer->m_flPoseParameter() = sent_pose_params;

}


bool AnimFix::GenerateMatrix(Entity* pEntity, Matrix3x4* pBoneToWorldOut, int boneMask, float currentTime)
{
	if (!pEntity || !pEntity->IsPlayer() || !pEntity->IsAlive())
		return false;

	studiohdr_t* pStudioHdr = I::modelinfo->GetStudioModel(pEntity->GetModel());
	if (!pStudioHdr) {
		return false;
	}

	// get ptr to bone accessor.
	CBoneAccessor* accessor = &pEntity->m_BoneAccessor();
	if (!accessor)
		return false;

	// store origial output matrix.
	// likely cachedbonedata.
	Matrix3x4* backup_matrix = accessor->m_pBones;
	if (!backup_matrix)
		return false;

	Vec vecAbsOrigin = pEntity->GetAbsOrigin();
	QAngle vecAbsAngles = pEntity->GetAbsAngles();

	Vec vecBackupAbsOrigin = pEntity->GetAbsOrigin();
	QAngle vecBackupAbsAngles = pEntity->GetAbsAngles();

	Matrix3x4 parentTransform;
	AngleMatrix(vecAbsAngles, vecAbsOrigin, parentTransform);

	pEntity->GetEffects() |= 8;

	pEntity->SetAbsOrigin(vecAbsOrigin);
	pEntity->SetAbsAngles(vecAbsAngles);

	Vec pos[128];
	__declspec(align(16)) quaternion_t     q[128];

	//m_bEnablePVS = true; //IDK LKJSDF:LKJSD:LFKJ:SDKFJ:LSKDJF:LKSDJF:LKSJD:FLKJS:LDKFJJ:l

	uint32_t fBackupOcclusionFlags = pEntity->OcclusionFlags();

	pEntity->OcclusionFlags() |= 0xA; // skipp call to accumulatelayers in standardblendingrules

	pEntity->StandardBlendingRules(pStudioHdr, pos, q, currentTime, boneMask);

	pEntity->OcclusionFlags() = fBackupOcclusionFlags; // standardblendingrules was called now restore niggaaaa

	
	accessor->m_pBones = pBoneToWorldOut;

	uint8_t computed[0x100];
	std::memset(computed, 0, 0x100);

	pEntity->BuildTransformations(pStudioHdr, pos, q, parentTransform, boneMask, computed);

	accessor->m_pBones = backup_matrix;

	pEntity->SetAbsOrigin(vecBackupAbsOrigin);
	pEntity->SetAbsAngles(vecBackupAbsAngles);

	//m_bEnablePVS = false; IDK :LAKJSF:LKJSD:LFKJS:LDKJF:LSKDJF:LSKDJF:LKSJDF:LKJ

	return true;
}

// THIS CRASHES, IDK Y... ILL THINK ABOUT IT LATER
void AnimFix::UpdateFakeAnim()
{
	if (!G::cmd || !(G::LocalPlayer && G::LocalPlayerAlive))
		return;

	if (!I::clientstate)
		return;

	bool allocate = (FakeAnimState == nullptr);
	bool change = (!allocate) && (G::LocalPlayer->GetRefEHandle() != EntHandle);
	bool reset = (!allocate && !change) && (G::LocalPlayer->m_flSpawnTime() != LocalSpawnTime);

	// player changed, free old animation state.
	if (change && FakeAnimStateAllocated)
		I::memalloc->Free(FakeAnimStateAllocated);

	// need to reset? (on respawn)
	if (reset) {
		// reset animation state.
		G::LocalPlayer->ResetAnimationState(FakeAnimState);

		// note new spawn time.
		LocalSpawnTime = G::LocalPlayer->m_flSpawnTime();
	}

	// need to allocate or create new due to player change.
	if (allocate || change) {
		// only works with games heap alloc.
		FakeAnimStateAllocated = (AnimState*)I::memalloc->Alloc(sizeof(AnimState));

		if (FakeAnimStateAllocated != nullptr)
			G::LocalPlayer->CreateState(FakeAnimStateAllocated);

		// used to detect if we need to recreate / reset.
		EntHandle = G::LocalPlayer->GetRefEHandle();
		LocalSpawnTime = G::LocalPlayer->m_flSpawnTime();

		// note anim state for future use.
		FakeAnimState = FakeAnimStateAllocated;
	}
	else {
		// make sure our state isn't null.
		if (!FakeAnimState)
			return;

		// allow re-animating in the same frame.
		FakeAnimState->m_iLastClientSideAnimationUpdateFramecount = 0.f;

		// update anim update delta as server would.
		FakeAnimState->m_flAnimUpdateDelta = fmaxf(I::globalvars->m_curTime - FakeAnimState->m_flLastClientSideAnimationUpdateTime, 0.0);

		// update fake animations per tick.
		if (UpdateFake) {
			// update fake animations when we send commands.
			if (!I::clientstate->m_choked_commands) {

				// update fake animation state.
				G::LocalPlayer->UpdateAnimationState(FakeAnimState, ViewAng); // COULD ALSO BE FAKE ANGLE IDK BRUGH

				std::memcpy(FakeLayers, G::LocalPlayer->GetAnimOverlays(), sizeof(AnimationLayer) * 13);
				FakePoses = G::LocalPlayer->m_flPoseParameter();
			}

			// remove model sway.
			FakeLayers[12].m_flWeight = 0.f;

			// make sure to only animate once per tick.
			UpdateFake = false;
		}

		// note - alpha; 
		// you don't need to set animation layers/poses to the fake ones here 
		// since they will already be fake, as UpdateAnimationState called 
		// UpdateClientsideAnimations - And since this is being called before
		// we update local player angles/layers, it will get overwritten by them
		// in the end.

		// Backup
		std::array< float, 24 > RealPoses = G::LocalPlayer->m_flPoseParameter();
		AnimationLayer RealLayers[13];
		std::memcpy(RealLayers, G::LocalPlayer->GetAnimOverlays(), sizeof(AnimationLayer) * 13);

		// replace current animation layers and poses with the fake ones.
		G::LocalPlayer->m_flPoseParameter() = FakePoses;
		std::memcpy(G::LocalPlayer->GetAnimOverlays(), FakeLayers, sizeof(AnimationLayer) * 13);

		// replace the model rotation and build a matrix with our fake data.
		float PrevAng = G::LocalPlayer->GetAbsAngles().y;
		G::LocalPlayer->SetAbsAngles(QAngle(0.f, FakeAnimState->m_flGoalFeetYaw, 0.f));

		// generate a fake matrix.
		GenerateMatrix(G::LocalPlayer, FakeMatrix, 0x7FF00, G::LocalPlayer->GetSimulationTime());

		// fix interpolated model.
		for (auto& i : FakeMatrix) {
			i.c[0][3] -= G::LocalPlayer->GetRenderOrigin().x;
			i.c[1][3] -= G::LocalPlayer->GetRenderOrigin().y;
			i.c[2][3] -= G::LocalPlayer->GetRenderOrigin().z;
		}

		// revert our layers and poses back.
		std::memcpy(G::LocalPlayer->GetAnimOverlays(), RealLayers, sizeof(AnimationLayer) * 13);
		G::LocalPlayer->m_flPoseParameter() = FakePoses;

		// replace the model rotation with the proper rotation.
		G::LocalPlayer->SetAbsAngles(QAngle(0.f, PrevAng, 0.f));
	}
}

void AnimFix::UpdateVal(CUserCmd* cmd, bool& sendPacket)
{
    if (!G::LocalPlayer || !G::LocalPlayer->IsAlive())
        return;
    SendPacket = sendPacket;
    if (SendPacket)
    {
        FakeAng = cmd->viewangles;
    }

    ViewAng = cmd->viewangles;
}
