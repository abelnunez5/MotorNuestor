#include <glad/glad.h>
#include "Resources/Texture.h"
#include <utility>

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other) {
        Release();
        id = std::exchange(other.id, 0);
        width = other.width;
        height = other.height;
        path = std::move(other.path);
    }
    return *this;
}

void Texture::Bind() const { glBindTexture(GL_TEXTURE_2D, id); }
void Texture::Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

void Texture::Release()
{
    if (id != 0) {
        glDeleteTextures(1, &id);
        id = 0;
    }
}