#pragma once
#include <glm/glm.hpp>
#include "Scene/Component.h"

class Transform : public Component {
public:
    static constexpr Type StaticType = Type::Transform;
    explicit Transform(GameObject& owner) : Component(owner, StaticType) {}

    const glm::vec3& GetPosition() const { return position; }
    const glm::vec3& GetRotation() const { return rotation; }   // Euler, en grados
    const glm::vec3& GetScale() const { return scale; }

    void SetPosition(const glm::vec3& value) { position = value; }
    void SetRotation(const glm::vec3& value) { rotation = value; }
    void SetScale(const glm::vec3& value) { scale = value; }

    glm::mat4 LocalMatrix() const;
    glm::mat4 WorldMatrix() const;   // incluye a los padres

private:
    glm::vec3 position{ 0.0f };
    glm::vec3 rotation{ 0.0f };
    glm::vec3 scale{ 1.0f };
};

