#pragma once

class Mesh;

// Debug lines for a mesh, one per vertex normal and one per face normal
class NormalLines {
public:
    NormalLines() = default;
    ~NormalLines() { Release(); }

    NormalLines(const NormalLines&) = delete;
    NormalLines& operator=(const NormalLines&) = delete;

    void Draw(const Mesh& mesh, bool vertexNormals, bool faceNormals, float length);

    void Release();

private:
    void Build(const Mesh& mesh, float length);

    unsigned int vertexVbo = 0;
    int vertexPointCount = 0;
    unsigned int faceVbo = 0;
    int facePointCount = 0;
    float builtLength = -1.0f;
};
