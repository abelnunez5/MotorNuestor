#pragma once
#include "Panel.h"

class Application;
class MeshComponent;
class TextureComponent;
class Transform;

class PanelInspector : public Panel {
public:
    explicit PanelInspector(Application& app) : Panel("Inspector"), app(app) {}

protected:
    void DrawContent() override;

private:
    void DrawTransformSection(Transform& transform);
    void DrawMeshSection(const MeshComponent& meshComponent);
    void DrawTextureSection(TextureComponent& component);

    Application& app;
};