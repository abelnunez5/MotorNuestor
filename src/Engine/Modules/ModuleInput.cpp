#include "Modules/ModuleInput.h"

#include <algorithm>
#include <filesystem>
#include <cctype>

#include "Core/Application.h"
#include "Core/Log.h"
#include "Modules/ModuleScene.h"

namespace {

KeyState Advance(KeyState previous, bool pressed)
{
    const bool wasPressed = previous == KeyState::Down || previous == KeyState::Held;
    if (pressed)
        return wasPressed ? KeyState::Held : KeyState::Down;
    return wasPressed ? KeyState::Up : KeyState::Idle;
}

}

UpdateStatus ModuleInput::PreUpdate()
{
    // Per-frame data
    droppedFiles.clear();
    mouseWheel = 0.0f;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        for (const EventListener& listener : listeners)
            listener(event);

        switch (event.type) {
        case SDL_EVENT_QUIT:
            app.RequestQuit();
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            mouseWheel += event.wheel.y;
            break;
        case SDL_EVENT_DROP_FILE:

            droppedFiles.emplace_back(event.drop.data);
            break;
        }
    }

    int numKeys = 0;
    const bool* keys = SDL_GetKeyboardState(&numKeys);
    const size_t count = std::min(keyboard.size(), static_cast<size_t>(numKeys));
    for (size_t i = 0; i < count; ++i)
        keyboard[i] = Advance(keyboard[i], keys[i]);

    const SDL_MouseButtonFlags buttons = SDL_GetMouseState(&mouseX, &mouseY);
    SDL_GetRelativeMouseState(&mouseDeltaX, &mouseDeltaY);
    for (int button = 1; button < static_cast<int>(mouseButtons.size()); ++button)
        mouseButtons[button] = Advance(mouseButtons[button], (buttons & SDL_BUTTON_MASK(button)) != 0);

    return UpdateStatus::Continue;
}
