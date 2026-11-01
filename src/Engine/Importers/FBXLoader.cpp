#include "Importers/FBXLoader.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Core/Log.h"

namespace {
    aiLogStream logStream;

    void AssimpLogCallback(const char* message, char* /*user*/)
    {
        std::string text = message;
        while (!text.empty() && (text.back() == '\n' || text.back() == '\r'))
            text.pop_back();
        LOG("Assimp: %s", text.c_str());
    }
}

namespace FBXLoader {

    void Init()
    {
        logStream.callback = AssimpLogCallback;
        logStream.user = nullptr;
        aiAttachLogStream(&logStream);
    }

    void CleanUp() { aiDetachAllLogStreams(); }

    std::vector<Mesh> Load(const std::string& path)
    {
        std::vector<Mesh> meshes;

        const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
        if (!scene || !scene->HasMeshes()) {
            LOG_ERROR("Error loading scene %s: %s", path.c_str(), aiGetErrorString());
            if (scene)
                aiReleaseImport(scene);
            return meshes;
        }

        meshes.reserve(scene->mNumMeshes);
        for (unsigned int m = 0; m < scene->mNumMeshes; ++m) {
            const aiMesh& src = *scene->mMeshes[m];

            std::vector<Mesh::Vertex> vertices(src.mNumVertices);
            for (unsigned int v = 0; v < src.mNumVertices; ++v) {
                Mesh::Vertex& out = vertices[v];
                out.position[0] = src.mVertices[v].x;
                out.position[1] = src.mVertices[v].y;
                out.position[2] = src.mVertices[v].z;
                if (src.HasNormals()) {
                    out.normal[0] = src.mNormals[v].x;
                    out.normal[1] = src.mNormals[v].y;
                    out.normal[2] = src.mNormals[v].z;
                }
                else {
                    out.normal[0] = out.normal[1] = out.normal[2] = 0.0f;
                }
                if (src.HasTextureCoords(0)) {
                    out.uv[0] = src.mTextureCoords[0][v].x;
                    out.uv[1] = src.mTextureCoords[0][v].y;
                }
                else {
                    out.uv[0] = out.uv[1] = 0.0f;
                }
            }

            std::vector<unsigned int> indices;
            indices.reserve(src.mNumFaces * 3);
            for (unsigned int f = 0; f < src.mNumFaces; ++f) {
                const aiFace& face = src.mFaces[f];
                if (face.mNumIndices != 3) {
                    LOG_WARNING("Face with %u indices skipped in %s", face.mNumIndices, src.mName.C_Str());
                    continue;
                }
                indices.insert(indices.end(), face.mIndices, face.mIndices + 3);
            }

            LOG("Mesh '%s': %u vertices, %zu indices", src.mName.C_Str(), src.mNumVertices, indices.size());
            meshes.emplace_back(src.mName.C_Str(), std::move(vertices), std::move(indices));

            if (src.mMaterialIndex < scene->mNumMaterials) {
                aiString texturePath;
                if (scene->mMaterials[src.mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
                    meshes.back().SetTexturePath(texturePath.C_Str());
            }
        }

        aiReleaseImport(scene);
        return meshes;
    }

}