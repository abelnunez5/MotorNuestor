#include "Modules/ModuleScene.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <memory>

#include "Importers/FBXLoader.h"
#include "Importers/TextureImporter.h"
#include "Core/Log.h"
#include "Core/Application.h"
#include "Modules/ModuleCamera.h"
#include "Modules/ModuleInput.h"

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
 
    textures.clear();
    TextureImporter::CleanUp();
    FBXLoader::CleanUp();
}

bool ModuleScene::LoadModel(const std::string& path)
{
    std::vector<Mesh> loaded = FBXLoader::Load(path);
    if (loaded.empty())
        return false;

   
    for (Mesh& mesh : loaded) {
        bounds.Expand(mesh.GetBounds());

        TextureComponent texture;
        if (!mesh.GetTexturePath().empty()) {
            const std::string resolved = ResolveTexturePath(mesh.GetTexturePath(), path);
            if (!resolved.empty())
                texture.SetTexture(TextureImporter::Load(resolved));
            else
                LOG_WARNING("Texture '%s' of mesh '%s' not found", mesh.GetTexturePath().c_str(), mesh.GetName().c_str());
        }
        textures.push_back(std::move(texture));
        meshes.push_back(std::move(mesh));
    }



    app.GetCamera().FocusOn(bounds);
    return true;
}

void ModuleScene::ApplyTexture(const std::string& path)
{
    std::shared_ptr<Texture> texture = TextureImporter::Load(path);
    if (!texture)
        return;

    if (selected >= 0 && static_cast<size_t>(selected) < textures.size()) {
        textures[selected].SetTexture(std::move(texture));
    }
    else {
        for (TextureComponent& component : textures)
            component.SetTexture(texture);
    }
}

bool ModuleScene::GetBounds(AABB& out) const
{
    out = bounds;
    return bounds.IsValid();
}