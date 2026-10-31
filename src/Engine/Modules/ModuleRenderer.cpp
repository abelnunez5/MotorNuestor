#include <glad/glad.h>
#include "Modules/ModuleRenderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "Modules/ModuleScene.h"
#include "Modules/ModuleWindow.h"
#include "Modules/ModuleCamera.h"
#include "Core/Application.h"
#include "Scene/GameObject.h"
#include "Scene/MeshComponent.h"
#include "Scene/TextureComponent.h"
#include "Scene/Transform.h"

namespace {
    // El transform local de cada nodo se apila sobre el del padre
    void DrawGameObject(const GameObject& gameObject)
    {
        const glm::mat4 local = gameObject.GetTransform().LocalMatrix();
        glPushMatrix();
        glMultMatrixf(glm::value_ptr(local));

        if (const MeshComponent* mesh = gameObject.GetComponent<MeshComponent>()) {
            const TextureComponent* texture = gameObject.GetComponent<TextureComponent>();
            if (texture)
                texture->Bind();
            mesh->GetMesh().Draw();
            if (texture)
                TextureComponent::Unbind();
        }

        for (const auto& child : gameObject.GetChildren())
            DrawGameObject(*child);

        glPopMatrix();
    }
}

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
    if (!sceneBuffer.IsValid())
        return UpdateStatus::Continue;

    const float aspect = static_cast<float>(sceneBuffer.Width()) / static_cast<float>(sceneBuffer.Height());
    const ModuleCamera& camera = app.GetCamera();

    sceneBuffer.Bind();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(camera.ProjectionMatrix(aspect)));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(camera.ViewMatrix()));

    DrawGrid();
    DrawGameObject(app.GetScene().GetRoot());

    sceneBuffer.Unbind();

    const ModuleWindow& window = app.GetWindow();
    glViewport(0, 0, window.Width(), window.Height());

    return UpdateStatus::Continue;
}

void ModuleRenderer::CleanUp()
{
    sceneBuffer.Release();
}

void ModuleRenderer::DrawGrid() const
{
    constexpr int kHalfSize = 50;
    constexpr float kExtent = static_cast<float>(kHalfSize);

    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);

    glColor3f(0.35f, 0.35f, 0.35f);
    for (int i = -kHalfSize; i <= kHalfSize; ++i) {
        if (i == 0)
            continue;
        const float offset = static_cast<float>(i);
        glVertex3f(offset, 0.0f, -kExtent);
        glVertex3f(offset, 0.0f, kExtent);
        glVertex3f(-kExtent, 0.0f, offset);
        glVertex3f(kExtent, 0.0f, offset);
    }

    // World axes --> X red, Z blue
    glColor3f(0.8f, 0.2f, 0.2f);
    glVertex3f(-kExtent, 0.0f, 0.0f);
    glVertex3f(kExtent, 0.0f, 0.0f);
    glColor3f(0.2f, 0.4f, 0.9f);
    glVertex3f(0.0f, 0.0f, -kExtent);
    glVertex3f(0.0f, 0.0f, kExtent);

    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_LIGHTING);
}