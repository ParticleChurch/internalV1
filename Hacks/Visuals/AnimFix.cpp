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
