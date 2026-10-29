#pragma once

#include "Panel.h"

// Shows everything written with LOG / LOG_WARNING / LOG_ERROR.
class PanelConsole : public Panel {
public:
    PanelConsole() : Panel("Console") {}

protected:
    void DrawContent() override;

private:
    bool autoScroll = true;
};
