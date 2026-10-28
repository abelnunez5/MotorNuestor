#pragma once

#include <cstdint>

// Real-time clock
namespace Time {

void Init();
void Tick();            // call once per frame, at the start

float DeltaTime();      // seconds of the last frame
float FPS();            // instantaneous frames per second
uint64_t FrameCount();  // frames since Init()

}
