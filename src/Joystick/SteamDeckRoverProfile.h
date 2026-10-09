#pragma once

// Analog Steam Deck rover profile: controller values must be normalized to [-1,1]
// for sticks and [0,1] for triggers. No Qt/SDL dependencies: unit-testable.
#include <algorithm>
#include <cmath>

namespace SteamDeckRoverProfile {
constexpr float TriggerDeadzone = 0.025f;
constexpr float StickDeadzone = 0.08f;
constexpr float GimbalYawMaxDegPerSec = 45.0f;
constexpr float GimbalPitchMaxDegPerSec = 30.0f;

inline float clamp(float value, float low, float high)
{
    // Treat invalid input as neutral.
    return std::isfinite(value) ? std::clamp(value, low, high) : 0.0f;
}

inline float trigger(float value)
{
    value = clamp(value, 0.0f, 1.0f);
    return value < TriggerDeadzone ? 0.0f : value;
}

inline float stick(float value)
{
    value = clamp(value, -1.0f, 1.0f);
    return std::abs(value) < StickDeadzone ? 0.0f : value;
}

inline float throttle(float rightTrigger, float leftTrigger)
{
    // Right trigger forward, left trigger reverse; simultaneous triggers cancel.
    return clamp(trigger(rightTrigger) - trigger(leftTrigger), -1.0f, 1.0f);
}

inline float steering(float leftX) { return stick(leftX); }
inline float gimbalYawRate(float rightX) { return stick(rightX) * GimbalYawMaxDegPerSec; }
inline float gimbalPitchRate(float rightY)
{
    // SDL uses negative Y for stick-up; positive pitch rate points up.
    return -stick(rightY) * GimbalPitchMaxDegPerSec;
}
} // namespace SteamDeckRoverProfile
