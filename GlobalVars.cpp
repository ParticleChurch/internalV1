#include "Include.hpp"

float GlobalVars::ServerTime(CUserCmd* cmd) noexcept
{
    static int tick;
    static CUserCmd* lastCmd;

    if (cmd) {
        if (G::LocalPlayer && (!lastCmd || lastCmd->hasbeenpredicted))
            tick = G::LocalPlayer->GetTickBase();
        else
            tick++;
        lastCmd = cmd;
    }
    return tick * m_intervalPerTick;
}