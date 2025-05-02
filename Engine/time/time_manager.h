#ifndef TIME_H
#define TIME_H

#include <cstdint>

class Time {
public:
    Time();
    
    // Update time and calculate delta time (call each frame)
    void Update();
    
    // Get delta time in seconds (fixed-point representation)
    int32_t GetDeltaTime() const;
    
    // Get total elapsed time in seconds
    float GetElapsedTime() const;

private:
    // Fixed-point scale for PS1-style precision
    static constexpr int32_t FIXED_POINT_SCALE = 4096; // 12-bit fractional part
    
    double lastFrameTime;      // Last frame timestamp (in seconds, GLFW time)
    double currentFrameTime;   // Current frame timestamp
    int32_t deltaTime;         // Delta time in fixed-point
    float elapsedTime;         // Total elapsed time in seconds
    
    // Convert float seconds to fixed-point
    int32_t SecondsToFixed(float seconds) const;
};

#endif // TIME_H