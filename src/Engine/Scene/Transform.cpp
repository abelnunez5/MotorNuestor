#include "Scene/Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Scene/GameObject.h"

glm::mat4 Transform::LocalMatrix() const
{
    const glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
    const glm::mat4 rotationMatrix = glm::mat4_cast(glm::quat(glm::radians(rotation)));
    const glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);
    return translation * rotationMatrix * scaling;
}

glm::mat4 Transform::WorldMatrix() const
{
    const GameObject* parent = GetOwner().GetParent();
    return parent ? parent->GetTransform().WorldMatrix() * LocalMatrix() : LocalMatrix();
}