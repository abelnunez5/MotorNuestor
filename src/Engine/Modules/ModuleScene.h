#pragma once
#include <memory>
#include <string>
#include "Core/Module.h"
#include "Math/AABB.h"
#include "Scene/GameObject.h"

class ModuleScene : public Module {
public:
    using Module::Module;

    bool Init() override;
    UpdateStatus Update() override;
    void CleanUp() override;

    bool LoadModel(const std::string& path);

    GameObject& GetRoot() { return *root; }
    const GameObject& GetRoot() const { return *root; }

    GameObject* GetSelected() const { return selected; }   // nullptr = ninguno
    void Select(GameObject* gameObject) { selected = gameObject; }

    // Aplica la textura al seleccionado (y sus hijos), o a toda la escena si no hay selección
    void ApplyTexture(const std::string& path);

    // Bounds en mundo del GameObject seleccionado, o de toda la escena si no hay selección (lo usa la cámara)
    bool GetBounds(AABB& out) const;

private:
    // Se crea aquí y no en Init() para que GetRoot() sea siempre válido
    std::unique_ptr<GameObject> root = std::make_unique<GameObject>("Root");
    GameObject* selected = nullptr;
};