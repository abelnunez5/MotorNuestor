#pragma once
#include <algorithm>
#include <utility>

#include "Resources/Mesh.h"
#include "Resources/NormalLines.h"
#include "Scene/Component.h"

class MeshComponent : public Component {
public:
    static constexpr Type StaticType = Type::Mesh;
    MeshComponent(GameObject& owner, Mesh mesh) : Component(owner, StaticType), mesh(std::move(mesh))
    {
        const AABB& bounds = this->mesh.GetBounds();
        if (bounds.IsValid())
            normalsLength = std::max(bounds.Radius() * 0.05f, 0.001f);  // Line 5% as long as the mesh
    }

    const Mesh& GetMesh() const { return mesh; }

    bool ShowVertexNormals() const { return showVertexNormals; }
    void SetShowVertexNormals(bool enabled) { showVertexNormals = enabled; }
    bool ShowFaceNormals() const { return showFaceNormals; }
    void SetShowFaceNormals(bool enabled) { showFaceNormals = enabled; }
    float GetNormalsLength() const { return normalsLength; }
    void SetNormalsLength(float length) { normalsLength = length; }

    bool IsDrawingNormals() const { return showVertexNormals || showFaceNormals; }
    void DrawNormals() const { normalLines.Draw(mesh, showVertexNormals, showFaceNormals, normalsLength); }

private:
    Mesh mesh;

    bool showVertexNormals = false;
    bool showFaceNormals = false;
    float normalsLength = 0.1f;
    mutable NormalLines normalLines;
};
