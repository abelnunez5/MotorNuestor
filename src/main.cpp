#include <SDL3/SDL_main.h>

#include "Core/Application.h"
#include "Core/Log.h"

int main(int /*argc*/, char* /*argv*/[])
{
    Application app;

    if (!app.Init()) {
        LOG_ERROR("Application failed to initialize");
        return -1;
    }

    UpdateStatus status = UpdateStatus::Continue;
    while (status == UpdateStatus::Continue)
        status = app.Update();

    return status == UpdateStatus::Error ? -1 : 0;
}
