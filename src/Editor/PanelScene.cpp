#include "PanelScene.h"

#include <cstdint>

#include "Core/Application.h"
#include "Modules/ModuleCamera.h"
#include "Modules/ModuleRenderer.h"

void PanelScene::BeforeBegin()
{
	ImGui::SetNextWindowPos(ImVec2(10.0f, 40.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(960.0f, 540.0f), ImGuiCond_FirstUseEver);
}

void PanelScene::DrawContent()
{
    const ImVec2 available = ImGui::GetContentRegionAvail();
    const ImVec2 scale = ImGui::GetIO().DisplayFramebufferScale;
 
    const int pixelsX = static_cast<int>(available.x * scale.x);
    const int pixelsY = static_cast<int>(available.y * scale.y);
    if (pixelsX <= 0 || pixelsY <= 0) return;

    ModuleRenderer& renderer = app.GetRenderer();
    renderer.SetViewportSize(pixelsX, pixelsY);

    const ImTextureID texture = (ImTextureID)(intptr_t)renderer.GetSceneTexture();
    ImGui::Image(texture, available, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

    const bool hovered = ImGui::IsItemHovered() && !ImGui::GetIO().WantTextInput;
    app.GetCamera().SetViewportHovered(hovered);
}