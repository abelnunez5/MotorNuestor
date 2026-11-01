#include "PanelInspector.h"

#include <cstdint>

#include <glm/glm.hpp>

#include "Core/Application.h"
#include "Modules/ModuleScene.h"
#include "Scene/GameObject.h"
#include "Scene/MeshComponent.h"
#include "Scene/TextureComponent.h"
#include "Scene/Transform.h"

void PanelInspector::DrawContent()
{
    GameObject* selected = app.GetScene().GetSelected();
    if (!selected) {
        ImGui::TextDisabled("No object selected");
        return;
    }

    ImGui::TextUnformatted(selected->GetName().c_str());
    ImGui::Separator();

    DrawTransformSection(selected->GetTransform());

    if (MeshComponent* meshComponent = selected->GetComponent<MeshComponent>())
        DrawMeshSection(*meshComponent);

    if (TextureComponent* textureComponent = selected->GetComponent<TextureComponent>())
        DrawTextureSection(*textureComponent);
}

void PanelInspector::DrawTransformSection(Transform& transform)
{
    if (!ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    glm::vec3 position = transform.GetPosition();
    if (ImGui::DragFloat3("Position", &position.x, 0.05f))
        transform.SetPosition(position);

    glm::vec3 rotation = transform.GetRotation();
    if (ImGui::DragFloat3("Rotation", &rotation.x, 0.5f))
        transform.SetRotation(rotation);

    glm::vec3 scale = transform.GetScale();
    if (ImGui::DragFloat3("Scale", &scale.x, 0.01f))
        transform.SetScale(scale);
}

void PanelInspector::DrawMeshSection(MeshComponent& meshComponent)
{
    if (!ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    const Mesh& mesh = meshComponent.GetMesh();
    ImGui::Text("Name: %s", mesh.GetName().c_str());
    ImGui::Text("Vertices: %zu", mesh.GetVertices().size());
    ImGui::Text("Triangles: %zu", mesh.GetIndices().size() / 3);

    const AABB& bounds = mesh.GetBounds();
    if (bounds.IsValid()) {
        const glm::vec3 size = bounds.Size();
        ImGui::Text("Bounds: %.2f x %.2f x %.2f", size.x, size.y, size.z);
    }

    ImGui::Separator();

    bool vertexNormals = meshComponent.ShowVertexNormals();
    if (ImGui::Checkbox("Show vertex normals", &vertexNormals))
        meshComponent.SetShowVertexNormals(vertexNormals);

    bool faceNormals = meshComponent.ShowFaceNormals();
    if (ImGui::Checkbox("Show face normals", &faceNormals))
        meshComponent.SetShowFaceNormals(faceNormals);

    if (meshComponent.IsDrawingNormals()) {
        float length = meshComponent.GetNormalsLength();
        if (ImGui::DragFloat("Normals length", &length, 0.005f, 0.001f, 100.0f, "%.3f"))
            meshComponent.SetNormalsLength(length);
    }
}

void PanelInspector::DrawTextureSection(TextureComponent& component)
{
    if (!ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    if (const std::shared_ptr<Texture>& texture = component.GetTexture()) {
        ImGui::TextWrapped("Path: %s", texture->Path().c_str());
        ImGui::Text("Size: %d x %d", texture->Width(), texture->Height());
    }
    else {
        ImGui::TextDisabled("No texture assigned");
    }

    bool checker = component.IsShowingChecker();
    if (ImGui::Checkbox("Show checker texture", &checker))
        component.SetShowChecker(checker);

    if (const Texture* preview = component.GetActive()) {
        // uv invertidas: la textura se guardó con origen abajo-izquierda
        ImGui::Image(static_cast<ImTextureID>(static_cast<std::intptr_t>(preview->Id())),
            ImVec2(128.0f, 128.0f), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
    }
}