#pragma once
#include <string>
#include <vector>
#include "Core/Module.h"
#include "Resources/Mesh.h"

class ModuleScene : public Module {
public:
    using Module::Module;

    bool Init() override;
    void CleanUp() override;

    bool LoadModel(const std::string& path);
    const std::vector<Mesh>& GetMeshes() const { return meshes; }

private:
    std::vector<Mesh> meshes;
};