#include "PanelConfiguration.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <string>

#include "Core/Application.h"
#include "Core/EngineInfo.h"
#include "Core/SystemInfo.h"
#include "Core/Time.h"
#include "Importers/TextureImporter.h"
#include "Modules/ModuleCamera.h"
#include "Modules/ModuleInput.h"
#include "Modules/ModuleRenderer.h"
#include "Modules/ModuleWindow.h"

namespace {

std::string FormatBytes(uint64_t bytes)
{
    const char* units[] = { "B", "KB", "MB", "GB" };
    double value = static_cast<double>(bytes);
    int unit = 0;
    while (value >= 1024.0 && unit < 3) {
        value /= 1024.0;
        ++unit;
    }
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%.1f %s", value, units[unit]);
    return buffer;
}

float Average(const float* values, int count)
{
    float sum = 0.0f;
    for (int i = 0; i < count; ++i)
        sum += values[i];
    return count > 0 ? sum / static_cast<float>(count) : 0.0f;
}

float Maximum(const float* values, int count)
{
    return count > 0 ? *std::max_element(values, values + count) : 0.0f;
}

}

void PanelConfiguration::BeforeBegin()
{
    ImGui::SetNextWindowPos(ImVec2(980.0f, 40.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(360.0f, 560.0f), ImGuiCond_FirstUseEver);
}

void PanelConfiguration::DrawContent()
{
    DrawApplication();
    DrawWindow();
    DrawRenderer();
    DrawCamera();
    DrawInput();
    DrawTextures();
    DrawHardware();
    DrawSoftware();
    DrawMemory();
}

void PanelConfiguration::DrawApplication()
{
    if (!ImGui::CollapsingHeader("Application", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    ImGui::Text("%s v%s", EngineInfo::kName, EngineInfo::kVersion);
    ImGui::Text("Frame: %llu", static_cast<unsigned long long>(Time::FrameCount()));

    const float* fps = Time::FpsHistory();
    const float* frameTime = Time::FrameTimeHistory();
    const int count = Time::kHistorySize;
    const int offset = Time::HistoryOffset();

    char overlay[64];
    std::snprintf(overlay, sizeof(overlay), "FPS: %.1f (average %.1f)", Time::FPS(), Average(fps, count));
    ImGui::PlotLines("##fps", fps, count, offset, overlay, 0.0f, std::max(Maximum(fps, count) * 1.2f, 1.0f), ImVec2(0.0f, 70.0f));

    std::snprintf(overlay, sizeof(overlay), "Frame time: %.2f ms", Time::DeltaTime() * 1000.0f);
    ImGui::PlotLines("##frametime", frameTime, count, offset, overlay, 0.0f, std::max(Maximum(frameTime, count) * 1.2f, 1.0f), ImVec2(0.0f, 70.0f));
}

void PanelConfiguration::DrawWindow()
{
    if (!ImGui::CollapsingHeader("Window"))
        return;

    ModuleWindow& window = app.GetWindow();

    bool fullscreen = window.IsFullscreen();
    if (ImGui::Checkbox("Fullscreen", &fullscreen))
        window.SetFullscreen(fullscreen);

    bool resizable = window.IsResizable();
    if (ImGui::Checkbox("Resizable", &resizable))
        window.SetResizable(resizable);

    bool bordered = window.IsBordered();
    if (ImGui::Checkbox("Bordered", &bordered))
        window.SetBordered(bordered);

    bool vsync = window.IsVSync();
    if (ImGui::Checkbox("VSync", &vsync))
        window.SetVSync(vsync);

    int size[2] = {};
    window.GetSize(size[0], size[1]);
    ImGui::BeginDisabled(fullscreen);
    if (ImGui::DragInt2("Size", size, 1.0f, 320, 7680))
        window.SetSize(size[0], size[1]);
    ImGui::EndDisabled();

    ImGui::Text("Drawable area: %d x %d pixels", window.Width(), window.Height());
}

void PanelConfiguration::DrawRenderer()
{
    if (!ImGui::CollapsingHeader("Renderer"))
        return;

    ModuleRenderer::Settings& settings = app.GetRenderer().GetSettings();
    ImGui::Checkbox("Wireframe", &settings.wireframe);
    ImGui::Checkbox("Depth test", &settings.depthTest);
    ImGui::Checkbox("Cull faces", &settings.cullFace);
    ImGui::Checkbox("Lighting", &settings.lighting);
    ImGui::Checkbox("Textures", &settings.textures);
    ImGui::Checkbox("Grid", &settings.showGrid);
    ImGui::ColorEdit3("Background", settings.clearColor);
}

void PanelConfiguration::DrawCamera()
{
    if (!ImGui::CollapsingHeader("Camera"))
        return;

    ModuleCamera& camera = app.GetCamera();

    float fov = camera.GetFovY();
    if (ImGui::SliderFloat("Field of view", &fov, 30.0f, 120.0f, "%.0f deg"))
        camera.SetFovY(fov);

    float nearPlane = camera.GetNearPlane();
    if (ImGui::DragFloat("Near plane", &nearPlane, 0.01f, 0.01f, 10.0f, "%.2f"))
        camera.SetNearPlane(nearPlane);

    float farPlane = camera.GetFarPlane();
    if (ImGui::DragFloat("Far plane", &farPlane, 1.0f, 10.0f, 100000.0f, "%.0f"))
        camera.SetFarPlane(farPlane);

    float speed = camera.GetMoveSpeed();
    if (ImGui::DragFloat("Move speed", &speed, 0.1f, 0.1f, 1000.0f, "%.1f"))
        camera.SetMoveSpeed(speed);
    ImGui::SameLine();
    ImGui::TextDisabled("(re-fitted when focusing)");

    float sensitivity = camera.GetLookSensitivity();
    if (ImGui::SliderFloat("Look sensitivity", &sensitivity, 0.01f, 1.0f, "%.2f"))
        camera.SetLookSensitivity(sensitivity);
}

void PanelConfiguration::DrawInput()
{
    if (!ImGui::CollapsingHeader("Input"))
        return;

    const ModuleInput& input = app.GetInput();
    ImGui::Text("Mouse position: %.0f, %.0f", input.MouseX(), input.MouseY());
    ImGui::Text("Mouse movement: %.1f, %.1f", input.MouseDeltaX(), input.MouseDeltaY());
    ImGui::Text("Mouse wheel: %.1f", input.MouseWheel());
    ImGui::Text("Files dropped this frame: %zu", input.GetDroppedFiles().size());
}

void PanelConfiguration::DrawTextures()
{
    if (!ImGui::CollapsingHeader("Textures"))
        return;

    const auto textures = TextureImporter::GetLoaded();

    // 4 bytes per pixel, plus about a third more for the mipmaps
    uint64_t estimatedBytes = 0;
    for (const auto& texture : textures)
        estimatedBytes += static_cast<uint64_t>(texture->Width()) * texture->Height() * 4 * 4 / 3;

    ImGui::Text("Loaded: %zu (about %s of video memory)", textures.size(), FormatBytes(estimatedBytes).c_str());
    for (const auto& texture : textures)
        ImGui::BulletText("%s (%d x %d)", texture->Path().c_str(), texture->Width(), texture->Height());
}

void PanelConfiguration::DrawHardware()
{
    if (!ImGui::CollapsingHeader("Hardware"))
        return;

    const SystemInfo::Info& info = SystemInfo::Get();
    ImGui::Text("GPU: %s", info.gpuRenderer.c_str());
    ImGui::Text("Vendor: %s", info.gpuVendor.c_str());
    ImGui::Separator();
    ImGui::Text("CPU cores: %d", info.cpuCores);
    ImGui::Text("Cache line: %d bytes", info.cpuCacheLineBytes);
    ImGui::TextWrapped("Instructions: %s", info.cpuFeatures.c_str());
    ImGui::Text("System RAM: %.1f GB", static_cast<double>(info.systemRamMB) / 1024.0);
}

void PanelConfiguration::DrawSoftware()
{
    if (!ImGui::CollapsingHeader("Software versions"))
        return;

    const SystemInfo::Info& info = SystemInfo::Get();
    ImGui::Text("SDL: %s", info.sdlVersion.c_str());
    ImGui::Text("OpenGL: %s", info.glVersion.c_str());
    ImGui::Text("GLSL: %s", info.glslVersion.c_str());
    ImGui::Text("DevIL: %s", info.devilVersion.c_str());
    ImGui::Text("Assimp: %s", info.assimpVersion.c_str());
    ImGui::Text("glm: %s", info.glmVersion.c_str());
    ImGui::Text("Dear ImGui: %s", ImGui::GetVersion());
}

void PanelConfiguration::DrawMemory()
{
    if (!ImGui::CollapsingHeader("Memory"))
        return;

    const SystemInfo::MemoryUsage usage = SystemInfo::GetMemoryUsage();

    if (usage.processWorkingSet > 0)
        ImGui::Text("Process (RAM): %s, peak %s", FormatBytes(usage.processWorkingSet).c_str(), FormatBytes(usage.processPeak).c_str());
    else
        ImGui::TextDisabled("Process memory: not available on this platform");

    if (usage.gpuTotal > 0)
        ImGui::Text("GPU memory: %s free of %s", FormatBytes(usage.gpuAvailable).c_str(), FormatBytes(usage.gpuTotal).c_str());
    else if (usage.gpuAvailable > 0)
        ImGui::Text("GPU memory: %s free", FormatBytes(usage.gpuAvailable).c_str());
    else
        ImGui::TextDisabled("GPU memory: not reported by the driver");
}
