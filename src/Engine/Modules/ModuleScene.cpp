#include "Modules/ModuleScene.h"

#include <algorithm>
#include <cctype>
#include <filesystem>

#include "Importers/FBXLoader.h"
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
}

bool ModuleScene::Init()
{
    FBXLoader::Init();
    if (!LoadModel("Assets/Resources/BakerHouse.fbx"))
        LOG_WARNING("Default model not found, starting with an empty scene");
    return true;
}

UpdateStatus ModuleScene::Update()
{
    for (const std::string& file : app.GetInput().GetDroppedFiles()) {
        if (HasExtension(file, ".fbx"))
            LoadModel(file);
        else
            LOG("Dropped file ignored (only .fbx for now): %s", file.c_str());
    }
    return UpdateStatus::Continue;
}

void ModuleScene::CleanUp()
{
    // Hay que liberar los buffers antes de que ModuleWindow destruya el contexto GL.
    meshes.clear();
    FBXLoader::CleanUp();
}

bool ModuleScene::LoadModel(const std::string& path)
{
    std::vector<Mesh> loaded = FBXLoader::Load(path);
    if (loaded.empty())
        return false;

    for (Mesh& mesh : loaded){
		bounds.Expand(mesh.GetBounds());
        meshes.push_back(std::move(mesh));
    }

    app.GetCamera().FocusOn(bounds);        
    return true;
}

bool ModuleScene::GetBounds(AABB& out) const
{
	out = bounds;
	return bounds.IsValid();
}