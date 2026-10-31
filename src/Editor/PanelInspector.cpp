#include "PanelInspector.h"

#include <cstdint>

#include "Core/Application.h"
#include "Modules/ModuleScene.h"

void PanelInspector::DrawContent()
{
    ModuleScene& scene = app.GetScene();
    const int selected = scene.GetSelected();
    if (selected < 0 || static_cast<size_t>(selected) >= scene.GetMeshes().size()) {
        ImGui::TextDisabled("No object selected");
        return;
    }

    ImGui::TextUnformatted(scene.GetMeshes()[selected].GetName().c_str());
    ImGui::Separator();

    // Temporal, falta Transform y Mesh
    DrawTextureSection(scene.GetTexture(selected));
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
        
        ImGui::Image(static_cast<ImTextureID>(static_cast<std::intptr_t>(preview->Id())),
            ImVec2(128.0f, 128.0f), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
    }

    // Temporal, (editor.cpp) registrar este panel en Editor.cpp y añadirlo al menú View (MainMenuBar).
// Temporal, (GameObjects): añadir aquí las secciones Transform y Mesh (visualizar normales).
}