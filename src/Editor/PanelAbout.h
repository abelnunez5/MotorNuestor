#pragma once

#include "Panel.h"

// Engine name, version, team, libraries and license. Hidden by default.
class PanelAbout : public Panel {
public:
    PanelAbout() : Panel("About", false) {}

protected:
    void DrawContent() override;
    ImGuiWindowFlags WindowFlags() const override { return ImGuiWindowFlags_AlwaysAutoResize; }
};
