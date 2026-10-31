#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "Core/Module.h"

class ModuleWindow;
class ModuleInput;
class ModuleRenderer;
class ModuleCamera;
class ModuleScene;

class Application {
public:
    Application();
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    // Registers a module. Registration order = Init/Update order.
    // Reverse order is used for CleanUp. The editor will be added this way from main.cpp so the engine never depends on it.

    template <typename T, typename... Args>
    T& AddModule(Args&&... args)
    {
        auto module = std::make_unique<T>(*this, std::forward<Args>(args)...);
        T& reference = *module;
        modules.push_back(std::move(module));
        return reference;
    }

    bool Init();
    UpdateStatus Update();   // runs one frame
    void CleanUp();

    void RequestQuit() { quitRequested = true; }

    ModuleWindow& GetWindow() const { return *window; }
    ModuleInput& GetInput() const { return *input; }
    ModuleRenderer& GetRenderer() const { return *renderer; }
    ModuleCamera& GetCamera() const { return *camera; }
    ModuleScene& GetScene() const { return *scene; }

private:
    UpdateStatus RunPhase(UpdateStatus(Module::* phase)());

    std::vector<std::unique_ptr<Module>> modules;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleRenderer* renderer = nullptr;
    ModuleCamera* camera = nullptr;
    ModuleScene* scene = nullptr;

    bool quitRequested = false;
    bool cleanedUp = false;
};
