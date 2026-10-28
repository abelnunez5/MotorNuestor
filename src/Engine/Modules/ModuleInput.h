#pragma once

#include <array>
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include <SDL3/SDL.h>

#include "Core/Module.h"

enum class KeyState { Idle, Down, Held, Up };

// Polls SDL events once per frame (PreUpdate) and exposes keyboard, mouse and file-drop state to the rest of the engine.
class ModuleInput : public Module {
public:
    using EventListener = std::function<void(const SDL_Event&)>;
    using Module::Module;

    UpdateStatus PreUpdate() override;

    // Called for every raw SDL event (the editor uses it to feed ImGui).
    void AddEventListener(EventListener listener) { listeners.push_back(std::move(listener)); }

    KeyState GetKey(SDL_Scancode key) const { return keyboard[key]; }
    KeyState GetMouseButton(int button) const { return mouseButtons[button]; }

    float MouseX() const { return mouseX; }
    float MouseY() const { return mouseY; }
    float MouseDeltaX() const { return mouseDeltaX; }
    float MouseDeltaY() const { return mouseDeltaY; }
    float MouseWheel() const { return mouseWheel; }

    // Files dropped on the window during this frame.
    const std::vector<std::string>& GetDroppedFiles() const { return droppedFiles; }

private:
    std::array<KeyState, static_cast<size_t>(SDL_SCANCODE_COUNT)> keyboard{};
    std::array<KeyState, 6> mouseButtons{};

    float mouseX = 0.0f;
    float mouseY = 0.0f;
    float mouseDeltaX = 0.0f;
    float mouseDeltaY = 0.0f;
    float mouseWheel = 0.0f;

    std::vector<std::string> droppedFiles;
    std::vector<EventListener> listeners;
};
