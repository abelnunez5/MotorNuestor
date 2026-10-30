#pragma once
#pragma once
#include "Core/Module.h"

class ModuleRenderer : public Module {
public:
    using Module::Module;

    bool Init() override;
    UpdateStatus PreUpdate() override;   // limpia pantalla
    UpdateStatus PostUpdate() override;  // dibuja la escena
};