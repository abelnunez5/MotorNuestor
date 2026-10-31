#include <glad/glad.h>
#include "Importers/TextureImporter.h"

#include <IL/il.h>
#include <fstream>
#include <iterator>
#include <unordered_map>
#include <vector>

#include "Core/Log.h"

namespace {
    std::unordered_map<std::string, std::weak_ptr<Texture>> cache;
    std::unique_ptr<Texture> checker;

    unsigned int Upload(const unsigned char* pixels, int w, int h)
    {
        GLuint id = 0;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        return id;
    }
}

void TextureImporter::Init()
{
    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);   // OpenGL espera v=0 abajo
    LOG("DevIL initialized");
}

void TextureImporter::CleanUp()
{
    cache.clear();
    checker.reset();
    ilShutDown();
}

std::shared_ptr<Texture> TextureImporter::Load(const std::string& path)
{
    if (auto it = cache.find(path); it != cache.end())
        if (auto alive = it->second.lock())
            return alive;

    // Leemos el fichero nosotros y se lo damos a DevIL desde memoria:
    // evita líos de char/wchar_t en Windows y sirve igual para PNG y DDS.
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        LOG_WARNING("Texture not found: %s", path.c_str());
        return nullptr;
    }
    std::vector<unsigned char> data((std::istreambuf_iterator<char>(file)), {});

    ILuint image = ilGenImage();
    ilBindImage(image);
    if (!ilLoadL(IL_TYPE_UNKNOWN, data.data(), static_cast<ILuint>(data.size()))) {
        LOG_WARNING("DevIL could not decode: %s", path.c_str());
        ilDeleteImage(image);
        return nullptr;
    }
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);   // también descomprime DDS

    const int w = ilGetInteger(IL_IMAGE_WIDTH);
    const int h = ilGetInteger(IL_IMAGE_HEIGHT);
    const unsigned int id = Upload(ilGetData(), w, h);
    ilDeleteImage(image);

    LOG("Texture loaded: %s (%dx%d)", path.c_str(), w, h);
    auto texture = std::make_shared<Texture>(id, w, h, path);
    cache[path] = texture;
    return texture;
}

const Texture& TextureImporter::GetChecker()
{
    if (!checker) {
        constexpr int size = 64;
        std::vector<unsigned char> pixels(size * size * 4);
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                const unsigned char c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
                unsigned char* p = &pixels[(i * size + j) * 4];
                p[0] = p[1] = p[2] = c;
                p[3] = 255;
            }
        }
        checker = std::make_unique<Texture>(Upload(pixels.data(), size, size), size, size, "[checker]");
    }
    return *checker;
}