#include "DockLayout.h"

#ifdef IMGUI_HAS_DOCK
#include <imgui_internal.h>
#endif

namespace {

#ifdef IMGUI_HAS_DOCK
void BuildDefaultLayout(ImGuiID dockspaceId, const ImVec2& size)
{
    ImGui::DockBuilderRemoveNode(dockspaceId);
    ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspaceId, size);

    ImGuiID center = dockspaceId;
    const ImGuiID bottom = ImGui::DockBuilderSplitNode(center, ImGuiDir_Down, 0.16f, nullptr, &center);
    const ImGuiID left = ImGui::DockBuilderSplitNode(center, ImGuiDir_Left, 0.11f, nullptr, &center);
    const ImGuiID right = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.18f, nullptr, &center);

    ImGui::DockBuilderDockWindow("Hierarchy", left);
    ImGui::DockBuilderDockWindow("Scene", center);
    ImGui::DockBuilderDockWindow("Inspector", right);
    ImGui::DockBuilderDockWindow("Configuration", right);
    ImGui::DockBuilderDockWindow("Console", bottom);

    ImGui::DockBuilderFinish(dockspaceId);
}
#endif

}

void DockLayout::Draw()
{
#ifdef IMGUI_HAS_DOCK
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    const bool noSavedLayout = ImGui::DockBuilderGetNode(kDockspaceId) == nullptr;

    ImGui::DockSpaceOverViewport(kDockspaceId, viewport, ImGuiDockNodeFlags_PassthruCentralNode);

    if (noSavedLayout)
        BuildDefaultLayout(kDockspaceId, viewport->WorkSize);
#endif
}
