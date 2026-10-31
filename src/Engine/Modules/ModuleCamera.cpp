#include "Modules/ModuleCamera.h"

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

#include "Core/Application.h"
#include "Core/Time.h"
#include "Modules/ModuleInput.h"
#include "Modules/ModuleScene.h"
#include "Modules/ModuleWindow.h"

namespace {

    constexpr float kMaxPitch = 89.0f;
    constexpr float kZoomFactor = 0.1f;
    constexpr float kMinDistance = 0.1f;
    const glm::vec3 kWorldUp(0.0f, 1.0f, 0.0f);

    bool IsPressed(KeyState state)
    {
        return state == KeyState::Down || state == KeyState::Held;
    }

}

UpdateStatus ModuleCamera::Update()
{
    ModuleInput& input = app.GetInput();

    const bool altHeld = IsPressed(input.GetKey(SDL_SCANCODE_LALT)) || IsPressed(input.GetKey(SDL_SCANCODE_RALT));
    const KeyState rightMouse = input.GetMouseButton(SDL_BUTTON_RIGHT);
    const KeyState leftMouse = input.GetMouseButton(SDL_BUTTON_LEFT);

    if (drag == Drag::None && viewportHovered) {
        if (rightMouse == KeyState::Down)
            BeginDrag(Drag::Fly);
        else if (leftMouse == KeyState::Down && altHeld)
            BeginDrag(Drag::Orbit);
    }

    switch (drag) {
    case Drag::Fly:
        if (IsPressed(rightMouse))
            UpdateFly(input);
        else
            EndDrag();
        break;

    case Drag::Orbit:
        if (IsPressed(leftMouse))
            UpdateOrbit(input);
        else
            EndDrag();
        break;

    case Drag::None:
        if (viewportHovered) {
            UpdateZoom(input.MouseWheel());
            if (input.GetKey(SDL_SCANCODE_F) == KeyState::Down)
                FocusOnScene();
        }
        break;
    }

    viewportHovered = false;
    return UpdateStatus::Continue;
}

void ModuleCamera::CleanUp()
{
    if (drag != Drag::None)
        EndDrag();
}

glm::mat4 ModuleCamera::ViewMatrix() const
{
    return glm::lookAt(position, position + Forward(), kWorldUp);
}

glm::mat4 ModuleCamera::ProjectionMatrix(float aspect) const
{
    return glm::perspective(glm::radians(fovY), aspect, nearPlane, farPlane);
}

void ModuleCamera::FocusOn(const AABB& bounds)
{
    if (!bounds.IsValid())
        return;

    const glm::vec3 center = bounds.Center();
    const float radius = std::max(bounds.Radius(), 0.01f);

    distance = radius / std::sin(glm::radians(fovY) * 0.5f) * 1.1f;
    position = center - Forward() * distance;
    pivot = center;

    moveSpeed = std::clamp(radius, 1.0f, 1000.0f);
    farPlane = std::max(1000.0f, (distance + radius) * 4.0f);
}

glm::vec3 ModuleCamera::Forward() const
{
    const float y = glm::radians(yaw);
    const float p = glm::radians(pitch);
    return glm::vec3(std::cos(p) * std::sin(y), std::sin(p), -std::cos(p) * std::cos(y));
}

// Right after entering relative mode SDL can report a big jump, so a couple of frames are skipped.
glm::vec2 ModuleCamera::ConsumeMouseDelta(const ModuleInput& input)
{
    if (ignoreDeltaFrames > 0) {
        --ignoreDeltaFrames;
        return glm::vec2(0.0f);
    }
    return glm::vec2(input.MouseDeltaX(), input.MouseDeltaY());
}

void ModuleCamera::BeginDrag(Drag mode)
{
    drag = mode;
    ignoreDeltaFrames = 2;

    if (mode == Drag::Orbit)
        pivot = position + Forward() * distance;

    SDL_SetWindowRelativeMouseMode(app.GetWindow().GetSDLWindow(), true);
}

void ModuleCamera::EndDrag()
{
    drag = Drag::None;
    SDL_SetWindowRelativeMouseMode(app.GetWindow().GetSDLWindow(), false);
}

void ModuleCamera::UpdateFly(const ModuleInput& input)
{
    const glm::vec2 delta = ConsumeMouseDelta(input);
    yaw += delta.x * lookSensitivity;
    pitch = std::clamp(pitch - delta.y * lookSensitivity, -kMaxPitch, kMaxPitch);

    const glm::vec3 forward = Forward();
    const glm::vec3 right = glm::normalize(glm::cross(forward, kWorldUp));

    glm::vec3 direction(0.0f);
    if (IsPressed(input.GetKey(SDL_SCANCODE_W))) direction += forward;
    if (IsPressed(input.GetKey(SDL_SCANCODE_S))) direction -= forward;
    if (IsPressed(input.GetKey(SDL_SCANCODE_D))) direction += right;
    if (IsPressed(input.GetKey(SDL_SCANCODE_A))) direction -= right;
    if (IsPressed(input.GetKey(SDL_SCANCODE_E))) direction += kWorldUp;
    if (IsPressed(input.GetKey(SDL_SCANCODE_Q))) direction -= kWorldUp;

    if (glm::length(direction) > 0.0f) {
        const bool shift = IsPressed(input.GetKey(SDL_SCANCODE_LSHIFT)) || IsPressed(input.GetKey(SDL_SCANCODE_RSHIFT));
        const float speed = moveSpeed * (shift ? 2.0f : 1.0f);
        position += glm::normalize(direction) * speed * Time::DeltaTime();
    }
}

void ModuleCamera::UpdateOrbit(const ModuleInput& input)
{
    const glm::vec2 delta = ConsumeMouseDelta(input);
    yaw += delta.x * lookSensitivity;
    pitch = std::clamp(pitch - delta.y * lookSensitivity, -kMaxPitch, kMaxPitch);

    position = pivot - Forward() * distance;
}

void ModuleCamera::UpdateZoom(float wheel)
{
    if (wheel == 0.0f)
        return;

    const float newDistance = std::max(kMinDistance, distance - wheel * distance * kZoomFactor);
    position += Forward() * (distance - newDistance);
    distance = newDistance;
}

//Temporal, como aun no tenemos GameObjects, la F hace focus en todo lo cargado. Luego usaremos el bounds del GameObject 
void ModuleCamera::FocusOnScene()
{
    AABB bounds;
    if (app.GetScene().GetBounds(bounds))
        FocusOn(bounds);
}