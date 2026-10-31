#pragma once

#include "Core/Module.h"
#include "Resources/Framebuffer.h"

class ModuleRenderer : public Module {
public:
    using Module::Module;

    bool Init() override;
    UpdateStatus PreUpdate() override;   // limpia pantalla
    UpdateStatus PostUpdate() override;  // dibuja la escena
	void CleanUp() override;

    void SetViewportSize(int width, int height) { sceneBuffer.Resize(width, height); }
	unsigned int GetSceneTexture() const { return sceneBuffer.ColorTexture(); }

private:
	void DrawGrid() const;

	Framebuffer sceneBuffer;
};