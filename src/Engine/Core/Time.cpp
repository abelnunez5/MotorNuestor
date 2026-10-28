#include "Core/Time.h"

#include <SDL3/SDL_timer.h>

namespace {

Uint64 lastTicksNS = 0;
float deltaTime = 0.0f;
float fps = 0.0f;
uint64_t frameCount = 0;

}

void Time::Init()
{
    lastTicksNS = SDL_GetTicksNS();
    deltaTime = 0.0f;
    fps = 0.0f;
    frameCount = 0;
}

void Time::Tick()
{
    const Uint64 now = SDL_GetTicksNS();
    deltaTime = static_cast<float>(now - lastTicksNS) / 1.0e9f;
    lastTicksNS = now;

    fps = deltaTime > 0.0f ? 1.0f / deltaTime : 0.0f;
    ++frameCount;
}

float Time::DeltaTime() { return deltaTime; }
float Time::FPS() { return fps; }
uint64_t Time::FrameCount() { return frameCount; }
