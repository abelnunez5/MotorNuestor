#pragma once
#include "Panel.h"

class Application;
class TextureComponent;

class PanelInspector : public Panel {
public:
    explicit PanelInspector(Application& app) : Panel("Inspector"), app(app) {}

protected:
    void DrawContent() override;

private:
    void DrawTextureSection(TextureComponent& component);

    Application& app;
};