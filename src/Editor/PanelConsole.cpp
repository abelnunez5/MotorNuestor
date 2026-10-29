#include "PanelConsole.h"

#include <deque>

#include "Core/Log.h"

namespace {

ImVec4 ColorFor(Log::Level level)
{
    switch (level) {
    case Log::Level::Warning: return ImVec4(1.0f, 0.8f, 0.2f, 1.0f);
    case Log::Level::Error:   return ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
    default:                  return ImVec4(0.85f, 0.85f, 0.85f, 1.0f);
    }
}

}

void PanelConsole::DrawContent()
{
    if (ImGui::Button("Clear"))
        Log::Clear();
    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &autoScroll);
    ImGui::Separator();

    ImGui::BeginChild("ConsoleScroll", ImVec2(0.0f, 0.0f), ImGuiChildFlags_None,
                      ImGuiWindowFlags_HorizontalScrollbar);

    const std::deque<Log::Entry>& entries = Log::Entries();
    ImGuiListClipper clipper;
    clipper.Begin(static_cast<int>(entries.size()));
    while (clipper.Step()) {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
            const Log::Entry& entry = entries[static_cast<size_t>(i)];
            ImGui::PushStyleColor(ImGuiCol_Text, ColorFor(entry.level));
            ImGui::TextUnformatted(entry.text.c_str());
            ImGui::PopStyleColor();
        }
    }

    if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
}
