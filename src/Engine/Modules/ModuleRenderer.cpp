#include <glad/glad.h>
#include "Modules/ModuleRenderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "Core/Application.h"
#include "Modules/ModuleCamera.h"
#include "Modules/ModuleScene.h"
#include "Modules/ModuleWindow.h"
#include "Scene/GameObject.h"
#include "Scene/MeshComponent.h"
#include "Scene/TextureComponent.h"
#include "Scene/Transform.h"

namespace {

    void SetEnabled(GLenum capability, bool enabled)
    {
        if (enabled)
            glEnable(capability);
        else
            glDisable(capability);
    }

    // El transform local de cada nodo se apila sobre el del padre
    void DrawGameObject(const GameObject& gameObject, const ModuleRenderer::Settings& settings)
    {
        const glm::mat4 local = gameObject.GetTransform().LocalMatrix();
        glPushMatrix();
        glMultMatrixf(glm::value_ptr(local));

        if (const MeshComponent* meshComponent = gameObject.GetComponent<MeshComponent>()) {
            const TextureComponent* texture = settings.textures ? gameObject.GetComponent<TextureComponent>() : nullptr;
            if (texture)
                texture->Bind();
            meshComponent->GetMesh().Draw();
            if (texture)
                TextureComponent::Unbind();

            if (meshComponent->IsDrawingNormals())
                meshComponent->DrawNormals();
        }

        for (const auto& child : gameObject.GetChildren())
            DrawGameObject(*child, settings);

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
    glClearColor(settings.clearColor[0], settings.clearColor[1], settings.clearColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Options from the Configuration window
    SetEnabled(GL_DEPTH_TEST, settings.depthTest);
    SetEnabled(GL_CULL_FACE, settings.cullFace);
    SetEnabled(GL_LIGHTING, settings.lighting);
    glPolygonMode(GL_FRONT_AND_BACK, settings.wireframe ? GL_LINE : GL_FILL);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(camera.ProjectionMatrix(aspect)));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(camera.ViewMatrix()));

    if (settings.showGrid)
        DrawGrid();

    glColor3f(1.0f, 1.0f, 1.0f);
    DrawGameObject(app.GetScene().GetRoot(), settings);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

    const bool wasLit = glIsEnabled(GL_LIGHTING) == GL_TRUE;
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
    if (wasLit)
        glEnable(GL_LIGHTING);
}
