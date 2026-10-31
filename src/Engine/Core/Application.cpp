#include "Core/Application.h"

#include <typeinfo>

#include "Core/Log.h"
#include "Core/Time.h"
#include "Modules/ModuleCamera.h"
#include "Modules/ModuleInput.h"
#include "Modules/ModuleWindow.h"
#include "Modules/ModuleRenderer.h"
#include "Modules/ModuleScene.h"

Application::Application()
{
    // Window must be first: it creates the SDL video system and the GL context.
    window = &AddModule<ModuleWindow>();
    input = &AddModule<ModuleInput>();
    renderer = &AddModule<ModuleRenderer>();
    camera = &AddModule<ModuleCamera>();
    scene = &AddModule<ModuleScene>();
}

Application::~Application()
{
    CleanUp();
}

bool Application::Init()
{
    LOG("Application init");

    for (const auto& module : modules) {
        if (!module->Init()) {
            LOG_ERROR("Init failed in %s", typeid(*module).name());
            return false;
        }
    }

    Time::Init();
    LOG("Application started");
    return true;
}

UpdateStatus Application::Update()
{
    Time::Tick();

    UpdateStatus status = RunPhase(&Module::PreUpdate);
    if (status == UpdateStatus::Continue)
        status = RunPhase(&Module::Update);
    if (status == UpdateStatus::Continue)
        status = RunPhase(&Module::PostUpdate);
    if (status != UpdateStatus::Continue)
        return status;

    // The swap is done here, after every PostUpdate, so modules added later can still render into this frame.
    window->SwapBuffers();

    return quitRequested ? UpdateStatus::Stop : UpdateStatus::Continue;
}

void Application::CleanUp()
{
    if (cleanedUp)
        return;
    cleanedUp = true;

    LOG("Application clean up");
    for (auto it = modules.rbegin(); it != modules.rend(); ++it)
        (*it)->CleanUp();
}

UpdateStatus Application::RunPhase(UpdateStatus (Module::*phase)())
{
    for (const auto& module : modules) {
        const UpdateStatus status = ((*module).*phase)();
        if (status != UpdateStatus::Continue)
            return status;
    }
    return UpdateStatus::Continue;
}
