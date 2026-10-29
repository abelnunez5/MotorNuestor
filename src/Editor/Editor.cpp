#include "Editor.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include "Core/Application.h"
#include "Core/Log.h"
#include "MainMenuBar.h"
#include "Modules/ModuleInput.h"
#include "Modules/ModuleWindow.h"
#include "PanelAbout.h"
#include "PanelConsole.h"

bool Editor::Init()
{
    LOG("Initializing editor (Dear ImGui %s)", IMGUI_VERSION);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
#ifdef IMGUI_HAS_DOCK
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif

    ImGui::StyleColorsDark();

    ModuleWindow& window = app.GetWindow();

    if (!ImGui_ImplSDL3_InitForOpenGL(window.GetSDLWindow(), window.GetGLContext())) {
        LOG_ERROR("ImGui SDL3 backend failed to initialize");
        return false;
    }
    sdlBackendReady = true;

    if (!ImGui_ImplOpenGL3_Init("#version 130")) {
        LOG_ERROR("ImGui OpenGL3 backend failed to initialize");
        return false;
    }
    openglBackendReady = true;

    app.GetInput().AddEventListener([](const SDL_Event& event) {
        ImGui_ImplSDL3_ProcessEvent(&event);
    });

    panels.push_back(std::make_unique<PanelConsole>());

    auto about = std::make_unique<PanelAbout>();
    aboutPanel = about.get();
    panels.push_back(std::move(about));

    LOG("Editor ready");
    return true;
}

UpdateStatus Editor::PreUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    return UpdateStatus::Continue;
}

UpdateStatus Editor::Update()
{
    MainMenuBar::Draw(app, panels, *aboutPanel);

#ifdef IMGUI_HAS_DOCK
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
#endif

    for (const auto& panel : panels)
        panel->Render();

    return UpdateStatus::Continue;
}

UpdateStatus Editor::PostUpdate()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return UpdateStatus::Continue;
}

void Editor::CleanUp()
{
    panels.clear();
    aboutPanel = nullptr;

    if (openglBackendReady) {
        ImGui_ImplOpenGL3_Shutdown();
        openglBackendReady = false;
    }
    if (sdlBackendReady) {
        ImGui_ImplSDL3_Shutdown();
        sdlBackendReady = false;
    }
    if (ImGui::GetCurrentContext())
        ImGui::DestroyContext();
}
