#include "PanelHierarchy.h"

#include "Core/Application.h"
#include "Modules/ModuleScene.h"
#include "Scene/GameObject.h"

void PanelHierarchy::DrawContent()
{
    ModuleScene& scene = app.GetScene();
    for (const auto& child : scene.GetRoot().GetChildren())
        DrawNode(scene, *child);

    // Clic en espacio vacío = deseleccionar
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered())
        scene.Select(nullptr);
}

void PanelHierarchy::DrawNode(ModuleScene& scene, GameObject& gameObject)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
        | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
    if (gameObject.GetChildren().empty())
        flags |= ImGuiTreeNodeFlags_Leaf;
    if (scene.GetSelected() == &gameObject)
        flags |= ImGuiTreeNodeFlags_Selected;

    const bool open = ImGui::TreeNodeEx(&gameObject, flags, "%s", gameObject.GetName().c_str());
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        scene.Select(&gameObject);

    if (open) {
        for (const auto& child : gameObject.GetChildren())
            DrawNode(scene, *child);
        ImGui::TreePop();
    }
}