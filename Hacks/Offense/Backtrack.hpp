#pragma once

class Backtrack {
private:
    bool ValidPlayer(Player player);
    void GetClosestEntity(int& RecordUserID);
    void GetClosestTick(int RecordUserID, int& BestTickCount);
public:
    Player::Tick TragetTick;
    void update(int CurStage);
    void run();
};

extern Backtrack* backtrack; 