#pragma once

#include <glm/glm.hpp>

#include "Core/Module.h"
#include "Math/AABB.h"

class ModuleInput;

class ModuleCamera : public Module {
public:
    using Module::Module;

    UpdateStatus Update() override;
    void CleanUp() override;

    glm::mat4 ViewMatrix() const;
    glm::mat4 ProjectionMatrix(float aspect) const;

    void SetViewportHovered(bool hovered) { viewportHovered = hovered; }

    void FocusOn(const AABB& bounds);

    // Settings
    float GetFovY() const { return fovY; }
    void SetFovY(float degrees) { fovY = degrees; }
    float GetMoveSpeed() const { return moveSpeed; }
    void SetMoveSpeed(float unitsPerSecond) { moveSpeed = unitsPerSecond; }
    float GetLookSensitivity() const { return lookSensitivity; }
    void SetLookSensitivity(float degreesPerPixel) { lookSensitivity = degreesPerPixel; }

private:
    enum class Drag { None, Fly, Orbit };

    glm::vec3 Forward() const;
    glm::vec2 ConsumeMouseDelta(const ModuleInput& input);

    void BeginDrag(Drag mode);
    void EndDrag();
    void UpdateFly(const ModuleInput& input);
    void UpdateOrbit(const ModuleInput& input);
    void UpdateZoom(float wheel);
    void FocusOnScene();

    glm::vec3 position{ 0.0f, 3.0f, 10.0f };
    float yaw = 0.0f;
    float pitch = -15.0f;
    float distance = 10.0f;   // distance from the camera to the orbit pivot
    glm::vec3 pivot{ 0.0f };  // only while orbiting

    // Lens
    float fovY = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;

    // Settings
    float moveSpeed = 5.0f;
    float lookSensitivity = 0.15f;

    // Interaction
    Drag drag = Drag::None;
    bool viewportHovered = false;
    int ignoreDeltaFrames = 0;
};