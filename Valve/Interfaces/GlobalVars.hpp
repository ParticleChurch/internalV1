#pragma once
class GlobalVars {
public:
    float   m_realTime;
    int     m_frameCount;
    float   m_absFrameTime;
    float   unknown;
    float   m_curTime;
    float   m_frameTime;
    int     m_maxClients;
    int     m_tickCount;
    float   m_intervalPerTick;
    float   m_interpAmount;

    float ServerTime(CUserCmd* = nullptr) noexcept;
};