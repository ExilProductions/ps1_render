#include "time_manager.h"
#include <GLFW/glfw3.h>

Time::Time()
    : lastFrameTime(glfwGetTime()), currentFrameTime(lastFrameTime), deltaTime(0), elapsedTime(0.0f)
{
}

void Time::Update()
{
    currentFrameTime = glfwGetTime();
    float deltaTimeSeconds = static_cast<float>(currentFrameTime - lastFrameTime);

    deltaTime = SecondsToFixed(deltaTimeSeconds);
    elapsedTime += deltaTimeSeconds;

    lastFrameTime = currentFrameTime;
}

int32_t Time::GetDeltaTime() const
{
    return deltaTime;
}

float Time::GetElapsedTime() const
{
    return elapsedTime;
}

int32_t Time::SecondsToFixed(float seconds) const
{
    return static_cast<int32_t>(seconds * FIXED_POINT_SCALE);
}