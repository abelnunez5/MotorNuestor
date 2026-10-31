#pragma once
#include <utility>
#include "Resources/Mesh.h"
#include "Scene/Component.h"

class MeshComponent : public Component {
public:
    static constexpr Type StaticType = Type::Mesh;
    MeshComponent(GameObject& owner, Mesh mesh) : Component(owner, StaticType), mesh(std::move(mesh)) {}

    const Mesh& GetMesh() const { return mesh; }

private:
    Mesh mesh;
};
