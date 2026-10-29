#pragma once

#include <memory>
#include <vector>

#include "Core/Module.h"
#include "Panel.h"

class Editor : public Module {
public:
    using Module::Module;

    bool Init() override;
    UpdateStatus PreUpdate() override;   // starts the ImGui frame
    UpdateStatus Update() override;      // builds the UI
    UpdateStatus PostUpdate() override;  // draws the UI
    void CleanUp() override;

private:
    std::vector<std::unique_ptr<Panel>> panels;
    Panel* aboutPanel = nullptr;

    bool sdlBackendReady = false;
    bool openglBackendReady = false;
};
