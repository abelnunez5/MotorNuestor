#include <glad/glad.h>
#include "Modules/ModuleRenderer.h"
#include "Modules/ModuleScene.h"
#include "Modules/ModuleWindow.h"
#include "Core/Application.h"
#include <cmath>

bool ModuleRenderer::Init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    return true;
}

UpdateStatus ModuleRenderer::PreUpdate()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    return UpdateStatus::Continue;
}

UpdateStatus ModuleRenderer::PostUpdate()
{
    // PROVISIONAL: sustituir por view/projection de la cámara cuando exista.
    const ModuleWindow& window = app.GetWindow();
    const float aspect = static_cast<float>(window.Width()) / static_cast<float>(window.Height());
    const float nearPlane = 0.1f, farPlane = 1000.0f;
    const float top = nearPlane * std::tan(60.0f * 0.5f * 3.14159265f / 180.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-top * aspect, top * aspect, -top, top, nearPlane, farPlane);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, -1.0f, -6.0f); // aleja la cámara; ajusta según la escala del modelo

    for (const Mesh& mesh : app.GetScene().GetMeshes())
        mesh.Draw();

    return UpdateStatus::Continue;
}