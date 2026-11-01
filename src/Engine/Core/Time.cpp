#include "Core/Time.h"

#include <SDL3/SDL_timer.h>

namespace {

Uint64 lastTicksNS = 0;
float deltaTime = 0.0f;
float fps = 0.0f;
uint64_t frameCount = 0;

float fpsHistory[Time::kHistorySize] = {};
float frameTimeHistory[Time::kHistorySize] = {};
int historyOffset = 0;

}

void Time::Init()
{
    lastTicksNS = SDL_GetTicksNS();
    deltaTime = 0.0f;
    fps = 0.0f;
    frameCount = 0;

    for (int i = 0; i < kHistorySize; ++i)
        fpsHistory[i] = frameTimeHistory[i] = 0.0f;
    historyOffset = 0;
}

void Time::Tick()
{
    const Uint64 now = SDL_GetTicksNS();
    deltaTime = static_cast<float>(now - lastTicksNS) / 1.0e9f;
    lastTicksNS = now;

    fps = deltaTime > 0.0f ? 1.0f / deltaTime : 0.0f;
    ++frameCount;

    fpsHistory[historyOffset] = fps;
    frameTimeHistory[historyOffset] = deltaTime * 1000.0f;
    historyOffset = (historyOffset + 1) % kHistorySize;
}

float Time::DeltaTime() { return deltaTime; }
float Time::FPS() { return fps; }
uint64_t Time::FrameCount() { return frameCount; }
const float* Time::FpsHistory() { return fpsHistory; }
const float* Time::FrameTimeHistory() { return frameTimeHistory; }
int Time::HistoryOffset() { return historyOffset; }
