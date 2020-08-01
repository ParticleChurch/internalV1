#pragma once

struct Tick {
    int TickCount;
    Matrix3x4 Matrix[128];
    Vec Bone(int i) {
        return Vec(Matrix[i][0][3],
            Matrix[i][1][3],
            Matrix[i][2][3]);
    }
    void operator = (Tick const& Obj) {
        TickCount = Obj.TickCount;
        for (int i = 0; i < 128; i++) {
            Matrix[i] = Obj.Matrix[i];
        }
    }
};

class Backtrack {
public:
    std::deque<Tick> records[65];
    void update(int stage);
    void run();
};

extern Backtrack* backtrack; 