#include "PanelAbout.h"

#include <SDL3/SDL.h>

#include "Core/EngineInfo.h"

void PanelAbout::DrawContent()
{
    ImGui::Text("%s  v%s", EngineInfo::kName, EngineInfo::kVersion);
    ImGui::Separator();

    ImGui::TextUnformatted("Team");
    for (const char* member : EngineInfo::kTeam)
        ImGui::BulletText("%s", member);

    ImGui::Separator();
    ImGui::TextUnformatted("Libraries");
    for (const char* library : EngineInfo::kLibraries)
        ImGui::BulletText("%s", library);

    ImGui::Separator();
    ImGui::Text("License: %s", EngineInfo::kLicense);

    if (ImGui::Button("Open repository"))
        SDL_OpenURL(EngineInfo::kRepoUrl);
}
