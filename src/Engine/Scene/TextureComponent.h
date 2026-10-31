#pragma once
#include <memory>
#include <utility>
#include "Resources/Texture.h"
#include "Scene/Component.h"

class TextureComponent : public Component {
public:
    static constexpr Type StaticType = Type::Texture;
    explicit TextureComponent(GameObject& owner) : Component(owner, StaticType) {}

    void SetTexture(std::shared_ptr<Texture> newTexture) { texture = std::move(newTexture); }
    const std::shared_ptr<Texture>& GetTexture() const { return texture; }

    void SetShowChecker(bool enabled) { showChecker = enabled; }
    bool IsShowingChecker() const { return showChecker; }

    // Textura que se dibujaría ahora mismo (checker, la propia o nullptr)
    const Texture* GetActive() const;

    void Bind() const;
    static void Unbind();

private:
    std::shared_ptr<Texture> texture;
    bool showChecker = false;
};

