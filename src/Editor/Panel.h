#pragma once

#include <string>
#include <utility>

#include <imgui.h>

// Base class for every editor window
class Panel {
public:
    explicit Panel(std::string name, bool visible = true)
        : name(std::move(name)), visible(visible) {}
    virtual ~Panel() = default;

    void Render()
    {
        if (!visible)
            return;

		BeforeBegin();

        if (ImGui::Begin(name.c_str(), &visible, WindowFlags()))
            DrawContent();
        ImGui::End();
    }

    const std::string& Name() const { return name; }
    bool& Visible() { return visible; }

protected:
    virtual void DrawContent() = 0;
    virtual ImGuiWindowFlags WindowFlags() const { return ImGuiWindowFlags_None; }

	virtual void BeforeBegin() {}

private:
    std::string name;
    bool visible;
};
