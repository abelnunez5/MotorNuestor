#pragma once

#include "Panel.h"

class Application;

class PanelConfiguration : public Panel {
public:
    explicit PanelConfiguration(Application& app) : Panel("Configuration"), app(app) {}

protected:
    void BeforeBegin() override;
    void DrawContent() override;

private:
    void DrawApplication();
    void DrawWindow();
    void DrawRenderer();
    void DrawCamera();
    void DrawInput();
    void DrawTextures();
    void DrawHardware();
    void DrawSoftware();
    void DrawMemory();

    Application& app;
};
