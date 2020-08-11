#pragma once

struct Tick {
    int Index;
    float SimulationTime;
    Matrix3x4 Matrix[256];

    //Resolving info
    QAngle angle;
    bool Shooting = false;
    bool InAir = false;

    Tick() {};
    Tick(int Index, float SimulationTime, Matrix3x4 Matrix[128]) {
        this->Index = Index;
        this->SimulationTime = SimulationTime;
        for (int i = 0; i < 128; i++) {
            this->Matrix[i] = Matrix[i];
        }
    }
    Vec Bone(int i) {
        return Vec(Matrix[i][0][3],
            Matrix[i][1][3],
            Matrix[i][2][3]);
    }
    void operator = (Tick const& Obj) {
        SimulationTime = Obj.SimulationTime;
        Index = Obj.Index;
        for (int i = 0; i < 128; i++) {
            Matrix[i] = Obj.Matrix[i];
        }
    }
    bool operator == (Tick const& Obj)
    {
        return (SimulationTime == Obj.SimulationTime) &&
            (Index == Obj.Index) &&
            (Shooting == Obj.Shooting);
    }
    bool operator != (Tick const& Obj)
    {
        return (SimulationTime != Obj.SimulationTime) || (Index != Obj.Index) || (Shooting != Obj.Shooting);
    }
};

class Backtrack {
private:
    ConVar* UpdateRate;
    ConVar* MaxUpdateRate;
    ConVar* Interp;
    ConVar* InterpRatio;
    ConVar* MinInterpRatio;
    ConVar* MaxInterpRatio;
    ConVar* MaxUnlag; 
   
public:
    std::deque<Tick> Records[65];
    void update();
    void run();

    int TimeToTicks(float time) noexcept
    {
        return static_cast<int>(0.5f + time / I::globalvars->m_intervalPerTick);
    }
    bool Valid(float SimulationTime)
    {
        auto network = I::engine->GetNetChannelInfo();
        if (!network)
            return false;

        auto delta = std::clamp(network->GetLatency(0) + network->GetLatency(1) + GetLerp(), 0.f, MaxUnlag->GetFloat()) - (I::globalvars->ServerTime() - SimulationTime);
        return std::fabsf(delta) <= 0.2f;
    }
    float GetLerp()
    {
        auto ratio = std::clamp(InterpRatio->GetFloat(), MinInterpRatio->GetFloat(), MaxInterpRatio->GetFloat());
        return max(Interp->GetFloat(), (ratio / ((MaxUpdateRate) ? MaxUpdateRate->GetFloat() : UpdateRate->GetFloat())));
    }

    void Init();
};

extern Backtrack* backtrack; 