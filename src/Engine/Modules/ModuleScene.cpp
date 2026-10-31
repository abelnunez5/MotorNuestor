#include "Modules/ModuleScene.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <memory>

#include <glm/glm.hpp>

#include "Importers/FBXLoader.h"
#include "Importers/TextureImporter.h"
#include "Core/Log.h"
#include "Core/Application.h"
#include "Modules/ModuleCamera.h"
#include "Modules/ModuleInput.h"
#include "Scene/MeshComponent.h"
#include "Scene/TextureComponent.h"
#include "Scene/Transform.h"

namespace {
    bool HasExtension(const std::string& path, const std::string& extension)
    {
        std::string found = std::filesystem::path(path).extension().string();
        std::transform(found.begin(), found.end(), found.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return found == extension;
    }

    bool IsTextureFile(const std::string& path)
    {
        return HasExtension(path, ".png") || HasExtension(path, ".dds");
    }

    // Las rutas de los FBX suelen venir rotas (absolutas de la máquina del artista), así que probamos varias.
    std::string ResolveTexturePath(const std::string& texturePath, const std::string& modelPath)
    {
        namespace fs = std::filesystem;
        const fs::path raw(texturePath);
        const fs::path modelDir = fs::path(modelPath).parent_path();
        const fs::path candidates[] = {
            raw,
            modelDir / raw,
            modelDir / raw.filename(),
            fs::path("Assets/Resources") / raw.filename()
        };
        for (const fs::path& candidate : candidates) {
            std::error_code ec;
            if (fs::exists(candidate, ec))
                return candidate.string();
        }
        return {};
    }

    // Bounds en mundo de un GameObject y todos sus descendientes
    void ExpandBounds(const GameObject& gameObject, AABB& bounds)
    {
        if (const MeshComponent* meshComponent = gameObject.GetComponent<MeshComponent>()) {
            const glm::mat4 world = gameObject.GetTransform().WorldMatrix();
            for (const Mesh::Vertex& vertex : meshComponent->GetMesh().GetVertices())
                bounds.Expand(glm::vec3(world * glm::vec4(vertex.position[0], vertex.position[1], vertex.position[2], 1.0f)));
        }
        for (const auto& child : gameObject.GetChildren())
            ExpandBounds(*child, bounds);
    }

    void ApplyTextureRecursive(GameObject& gameObject, const std::shared_ptr<Texture>& texture)
    {
        if (TextureComponent* component = gameObject.GetComponent<TextureComponent>())
            component->SetTexture(texture);
        for (const auto& child : gameObject.GetChildren())
            ApplyTextureRecursive(*child, texture);
    }
}

bool ModuleScene::Init()
{
    FBXLoader::Init();
    TextureImporter::Init();
    if (!LoadModel("Assets/Resources/BakerHouse.fbx"))
        LOG_WARNING("Default model not found, starting with an empty scene");
    return true;
}

UpdateStatus ModuleScene::Update()
{
    for (const std::string& file : app.GetInput().GetDroppedFiles()) {
        if (HasExtension(file, ".fbx"))
            LoadModel(file);
        else if (IsTextureFile(file))
            ApplyTexture(file);
        else
            LOG("Dropped file ignored (only .fbx, .png and .dds): %s", file.c_str());
    }
    return UpdateStatus::Continue;
}

void ModuleScene::CleanUp()
{
    // Hay que liberar los buffers y texturas antes de que ModuleWindow destruya el contexto GL.
    selected = nullptr;
    root.reset();
    TextureImporter::CleanUp();
    FBXLoader::CleanUp();
}

bool ModuleScene::LoadModel(const std::string& path)
{
    std::vector<Mesh> loaded = FBXLoader::Load(path);
    if (loaded.empty())
        return false;

    GameObject& model = root->AddChild(std::filesystem::u8path(path).stem().u8string());

    for (Mesh& mesh : loaded) {
        GameObject& child = model.AddChild(mesh.GetName());

        TextureComponent& texture = child.AddComponent<TextureComponent>();
        if (!mesh.GetTexturePath().empty()) {
            const std::string resolved = ResolveTexturePath(mesh.GetTexturePath(), path);
            if (!resolved.empty())
                texture.SetTexture(TextureImporter::Load(resolved));
            else
                LOG_WARNING("Texture '%s' of mesh '%s' not found", mesh.GetTexturePath().c_str(), mesh.GetName().c_str());
        }

        child.AddComponent<MeshComponent>(std::move(mesh));
    }

    selected = &model;

    AABB modelBounds;
    ExpandBounds(model, modelBounds);
    if (modelBounds.IsValid())
        app.GetCamera().FocusOn(modelBounds);
    return true;
}

void ModuleScene::ApplyTexture(const std::string& path)
{
    std::shared_ptr<Texture> texture = TextureImporter::Load(path);
    if (!texture)
        return;

    ApplyTextureRecursive(selected ? *selected : *root, texture);
}

bool ModuleScene::GetBounds(AABB& out) const
{
    out = AABB{};
    ExpandBounds(selected ? *selected : *root, out);
    return out.IsValid();
}