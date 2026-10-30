#pragma once
#include <string>
#include <vector>
#include "Resources/Mesh.h"

namespace FBXLoader {
    void Init();
    void CleanUp();
    std::vector<Mesh> Load(const std::string& path);
}