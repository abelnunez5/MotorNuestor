#include "MainMenuBar.h"

#include <string>

#include <SDL3/SDL.h>

#include "Core/Application.h"
#include "Core/EngineInfo.h"
#include "Core/Log.h"

namespace {

    void OpenUrl(const std::string& url)
    {
        if (!SDL_OpenURL(url.c_str()))
        LOG_WARNING("Could not open %s: %s", url.c_str(), SDL_GetError());
    }
}

void MainMenuBar::Draw(Application& app, const std::vector<std::unique_ptr<Panel>>& panels, Panel& about)
{
    if (!ImGui::BeginMainMenuBar())
        return;

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Exit"))
            app.RequestQuit();
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
        for (const auto& panel : panels)
            ImGui::MenuItem(panel->Name().c_str(), nullptr, &panel->Visible());
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help")) {
        const std::string repo = EngineInfo::kRepoUrl;

        if (ImGui::MenuItem("Documentation"))
            OpenUrl(repo + "/docs");
        if (ImGui::MenuItem("Report a bug"))
            OpenUrl(repo + "/issues");
        if (ImGui::MenuItem("Download latest"))
            OpenUrl(repo + "/releases");

        ImGui::Separator();
        ImGui::MenuItem("About", nullptr, &about.Visible());
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}
