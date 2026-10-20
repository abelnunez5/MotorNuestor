#pragma once

#include <assimp/cimport.h>    
#include <assimp/scene.h>    
#include <assimp/postprocess.h>

struct Mesh {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    //  std::vector<unsigned int> colores;
    unsigned int idVertex;
    unsigned int idIndices;
    //unsigned int idColores;

    void CreateVertexBuffer();
    void Draw() const;

};

namespace FBXLoader {
    struct FBX {
        unsigned int id_index = 0;
        unsigned int num_index = 0;
        unsigned int* index = nullptr;

        unsigned int id_vertex = 0;
        unsigned int num_vertex = 0;
        float* vertex = nullptr;
    };

    void LoadFBX(const char* path, std::vector<Mesh>& mesh);

    void Init();
    void CleanUp();
};