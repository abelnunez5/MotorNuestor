#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Scene/Component.h"

class Transform;

class GameObject {
public:
    explicit GameObject(std::string name, GameObject* parent = nullptr);
    ~GameObject() = default;

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    const std::string& GetName() const { return name; }
    void SetName(std::string newName) { name = std::move(newName); }

    GameObject* GetParent() const { return parent; }
    const std::vector<std::unique_ptr<GameObject>>& GetChildren() const { return children; }
    GameObject& AddChild(std::string childName);

    // Todo GameObject tiene Transform desde que nace
    Transform& GetTransform() { return *transform; }
    const Transform& GetTransform() const { return *transform; }

    template<typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
        auto component = std::make_unique<T>(*this, std::forward<Args>(args)...);
        T& reference = *component;
        components.push_back(std::move(component));
        return reference;
    }

    template<typename T>
    T* GetComponent()
    {
        for (const auto& component : components)
            if (component->GetType() == T::StaticType)
                return static_cast<T*>(component.get());
        return nullptr;
    }

    template<typename T>
    const T* GetComponent() const
    {
        for (const auto& component : components)
            if (component->GetType() == T::StaticType)
                return static_cast<const T*>(component.get());
        return nullptr;
    }

private:
    std::string name;
    GameObject* parent = nullptr;
    std::vector<std::unique_ptr<GameObject>> children;
    std::vector<std::unique_ptr<Component>> components;
    Transform* transform = nullptr;
};
