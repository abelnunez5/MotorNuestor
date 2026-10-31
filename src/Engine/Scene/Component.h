#pragma once

class GameObject;

class Component {
public:
    enum class Type { Transform, Mesh, Texture };

    virtual ~Component() = default;
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;

    Type GetType() const { return type; }
    GameObject& GetOwner() const { return owner; }

protected:
    Component(GameObject& owner, Type type) : owner(owner), type(type) {}

private:
    GameObject& owner;
    Type type;
};