#pragma once
#include "Panel.h"

class Application;
class GameObject;
class ModuleScene;

class PanelHierarchy : public Panel {
public:
    explicit PanelHierarchy(Application& app) : Panel("Hierarchy"), app(app) {}

protected:
    void DrawContent() override;

private:
    void DrawNode(ModuleScene& scene, GameObject& gameObject);

    Application& app;
};
