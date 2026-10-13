#pragma once

#include <assimp/cimport.h>    
#include <assimp/scene.h>    
#include <assimp/postprocess.h>

namespace FBXLoader {
    struct FBX {
        unsigned int id_index = 0;
        unsigned int num_index = 0;
        unsigned int* index = nullptr;

        unsigned int id_vertex = 0;
        unsigned int num_vertex = 0;
        float* vertex = nullptr;
    };

    void LoadFBX();

    void Init();
    void CleanUp();
};