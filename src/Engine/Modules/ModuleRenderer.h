#pragma once

#include "Core/Module.h"
#include "Resources/Framebuffer.h"

class ModuleRenderer : public Module {
public:

    struct Settings {
        bool wireframe = false;
        bool depthTest = true;
        bool cullFace = true;
        bool lighting = true;
        bool textures = true;
        bool showGrid = true;
        float clearColor[3] = { 0.1f, 0.1f, 0.1f };
    };

    using Module::Module;

    bool Init() override;
    UpdateStatus PreUpdate() override;   // limpia pantalla
    UpdateStatus PostUpdate() override;  // dibuja la escena
    void CleanUp() override;

    Settings& GetSettings() { return settings; }
    const Settings& GetSettings() const { return settings; }

    void SetViewportSize(int width, int height) { sceneBuffer.Resize(width, height); }
    unsigned int GetSceneTexture() const { return sceneBuffer.ColorTexture(); }

private:
    void DrawGrid() const;

    Settings settings;
    Framebuffer sceneBuffer;
};
