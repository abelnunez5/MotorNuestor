#pragma once
#include <memory>
#include <string>
#include "Resources/Texture.h"

namespace TextureImporter {
    void Init();
    void CleanUp();   // vaciar la caché ANTES de que se destruya el contexto GL

    // Devuelve la textura cacheada si ya estaba cargada (DDS, PNG, ...)
    std::shared_ptr<Texture> Load(const std::string& path);
    const Texture& GetChecker();
}

