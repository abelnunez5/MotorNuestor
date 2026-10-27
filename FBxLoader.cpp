#include <vector>
#include <glad/glad.h>
#include <assimp/cimport.h>    
#include <assimp/scene.h>    
#include <assimp/postprocess.h>
#include <SDL3/SDL_events.h>
#include "FBXLoader.h"


void Mesh::CreateVertexBuffer()
{
    glGenBuffers(1, &idVertex);
    glBindBuffer(GL_ARRAY_BUFFER, idVertex);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);


    glGenBuffers(1, &idIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    //glGenBuffers(1, &idColores);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idColores);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, colores.size() * sizeof(unsigned int), colores.data(), GL_STATIC_DRAW);

}

void Mesh::Draw() const {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, idVertex);

    glVertexPointer(3, GL_FLOAT, sizeof(float) * 6, NULL);
    glNormalPointer(GL_FLOAT, sizeof(float) * 6, reinterpret_cast<void*>(sizeof(float) * 3));
    //glColorPointer(GL_FLOAT, );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndices);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
}


namespace FBXLoader {

    void Init() {
        struct aiLogStream stream;
        stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
        aiAttachLogStream(&stream);
    }

    void LoadFBX(const char* path, std::vector<Mesh>& mesh) {
        const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
        if (scene != nullptr && scene->HasMeshes())
        {
            mesh.reserve(scene->mNumMeshes);
            for (int i = 0; i < scene->mNumMeshes; i++)
            {

                const aiMesh& aimesh = *scene->mMeshes[i];

                mesh.emplace_back();
                Mesh& myMesh = mesh[i];
                mesh[i].vertices.reserve(aimesh.mNumVertices * 6);


                for (int i = 0; i < aimesh.mNumVertices; i++)
                {
                    myMesh.vertices.push_back(aimesh.mVertices[i].x);
                    myMesh.vertices.push_back(aimesh.mVertices[i].y);
                    myMesh.vertices.push_back(aimesh.mVertices[i].z);

                    if (aimesh.HasNormals())
                    {
                        myMesh.vertices.push_back(aimesh.mNormals[i].x);
                        myMesh.vertices.push_back(aimesh.mNormals[i].y);
                        myMesh.vertices.push_back(aimesh.mNormals[i].z);
                    }
                }

                myMesh.indices.reserve(aimesh.mNumFaces * 3);
                //if (!aimesh.HasFaces())
                //    return;
                for (int i = 0; i < aimesh.mNumFaces; i++)
                {
                    if (aimesh.mFaces->mNumIndices = !3)
                    {
                        break;
                    }

                    myMesh.indices.push_back(aimesh.mFaces[i].mIndices[0]);
                    myMesh.indices.push_back(aimesh.mFaces[i].mIndices[1]);
                    myMesh.indices.push_back(aimesh.mFaces[i].mIndices[2]);
                }

                myMesh.CreateVertexBuffer();
            }

            aiReleaseImport(scene);
        }
        //else
            //LOG(“Error loading scene % s”, path);
    }

    void CleanUp() {
        aiDetachAllLogStreams();
    }
}