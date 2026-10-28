#pragma once

#include <SDL3/SDL.h>

#include "Core/Module.h"

// Owns the SDL window and the OpenGL context.
class ModuleWindow : public Module {
public:
    using Module::Module;

    bool Init() override;
    UpdateStatus PreUpdate() override;
    void CleanUp() override;

    void SwapBuffers() const;

    SDL_Window* GetSDLWindow() const { return window; }
    SDL_GLContext GetGLContext() const { return glContext; }

    // Size of the drawable area, in pixels.
    int Width() const { return width; }
    int Height() const { return height; }

    bool IsVSync() const { return vsync; }
    void SetVSync(bool enabled);

private:
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;

    int width = 0;
    int height = 0;
    bool vsync = true;
};
