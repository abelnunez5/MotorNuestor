#include "Modules/ModuleScene.h"
#include "Importers/FBXLoader.h"
#include "Core/Log.h"

bool ModuleScene::Init()
{
    FBXLoader::Init();
    if (!LoadModel("Assets/Resources/BakerHouse.fbx"))
        LOG_WARNING("Default model not found, starting with an empty scene");
    return true;
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

    for (Mesh& mesh : loaded)
        meshes.push_back(std::move(mesh));
    return true;
}