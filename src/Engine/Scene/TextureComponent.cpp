#include <glad/glad.h>
#include "Scene/TextureComponent.h"
#include "Importers/TextureImporter.h"

const Texture* TextureComponent::GetActive() const
{
    return showChecker ? &TextureImporter::GetChecker() : texture.get();
}

void TextureComponent::Bind() const
{
    const Texture* active = GetActive();
    if (!active)
        return;
    glEnable(GL_TEXTURE_2D);
    active->Bind();
}

void TextureComponent::Unbind()
{
    Texture::Unbind();
    glDisable(GL_TEXTURE_2D);
}