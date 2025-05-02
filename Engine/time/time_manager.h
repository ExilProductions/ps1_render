#ifndef TIME_H
#define TIME_H

#include <cstdint>

class Time
{
public:
    Time();

    void Update();

    int32_t GetDeltaTime() const;

    float GetElapsedTime() const;

private:
    static constexpr int32_t FIXED_POINT_SCALE = 4096;

    double lastFrameTime;
    double currentFrameTime;
    int32_t deltaTime;
    float elapsedTime;

    int32_t SecondsToFixed(float seconds) const;
};

#endif