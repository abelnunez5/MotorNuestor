#pragma once
#include <string>
#include <vector>
#include "Core/Module.h"
#include "Math/AABB.h"
#include "Resources/Mesh.h"
#include "Scene/TextureComponent.h"

class ModuleScene : public Module {
public:
    using Module::Module;

    bool Init() override;
    UpdateStatus Update() override;
    void CleanUp() override;

    bool LoadModel(const std::string& path);
    const std::vector<Mesh>& GetMeshes() const { return meshes; }

    // Temporal, cuando estén los GameObjects, estos tres bloques se mueven dentro de ellos
    const std::vector<TextureComponent>& GetTextures() const { return textures; }
    TextureComponent& GetTexture(size_t index) { return textures[index]; }
    int GetSelected() const { return selected; }          // -1 = ninguno
    void Select(int index) { selected = index; }

    
    void ApplyTexture(const std::string& path);

    // Temporal, cuando estén los GameObjects estos darán los bounds
    bool GetBounds(AABB& out) const;

private:
    std::vector<Mesh> meshes;
    std::vector<TextureComponent> textures;
    int selected = -1;
    AABB bounds;
};