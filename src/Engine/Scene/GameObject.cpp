#include "Scene/GameObject.h"
#include "Scene/Transform.h"

GameObject::GameObject(std::string name, GameObject* parent)
    : name(std::move(name)), parent(parent)
{
    transform = &AddComponent<Transform>();
}

GameObject& GameObject::AddChild(std::string childName)
{
    children.push_back(std::make_unique<GameObject>(std::move(childName), this));
    return *children.back();
}
