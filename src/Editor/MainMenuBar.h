#pragma once

#include <memory>
#include <vector>

#include "Panel.h"

class Application;

// File / View / GameObject / Help
namespace MainMenuBar {

void Draw(Application& app, const std::vector<std::unique_ptr<Panel>>& panels, Panel& about);

}
