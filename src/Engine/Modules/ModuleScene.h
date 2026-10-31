#pragma once
#include <string>
#include <vector>
#include "Core/Module.h"
#include "Math/AABB.h"
#include "Resources/Mesh.h"

class ModuleScene : public Module {
public:
    using Module::Module;

    bool Init() override;
    UpdateStatus Update() override;
    void CleanUp() override;

    bool LoadModel(const std::string& path);
    const std::vector<Mesh>& GetMeshes() const { return meshes; }

	// Temporal, cuando estén los GameObjects estos darán los bounds
    bool GetBounds(AABB& out) const;

private:
    std::vector<Mesh> meshes;
	AABB bounds;
};