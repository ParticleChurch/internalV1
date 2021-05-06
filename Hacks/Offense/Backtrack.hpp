#pragma once

class Backtrack {
private:
    void GetClosestEntity(int& RecordUserID);
    void GetClosestTick(int RecordUserID, int& BestTickCount);
public:
    Tick TargetTick;
    void run();
};

extern Backtrack* backtrack;