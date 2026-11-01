#include <glad/glad.h>
#include "Resources/NormalLines.h"

#include <vector>

#include <glm/glm.hpp>

#include "Resources/Mesh.h"

namespace {

    glm::vec3 PositionOf(const Mesh::Vertex& vertex)
    {
        return glm::vec3(vertex.position[0], vertex.position[1], vertex.position[2]);
    }

    void Upload(unsigned int& vbo, const std::vector<glm::vec3>& points)
    {
        if (vbo == 0)
            glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(points.size() * sizeof(glm::vec3)), points.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void DrawLines(unsigned int vbo, int pointCount, float r, float g, float b)
    {
        if (vbo == 0 || pointCount == 0)
            return;
        glColor3f(r, g, b);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexPointer(3, GL_FLOAT, 0, nullptr);
        glDrawArrays(GL_LINES, 0, pointCount);
    }

}

void NormalLines::Release()
{
    if (vertexVbo) glDeleteBuffers(1, &vertexVbo);
    if (faceVbo) glDeleteBuffers(1, &faceVbo);
    vertexVbo = faceVbo = 0;
    vertexPointCount = facePointCount = 0;
    builtLength = -1.0f;
}

void NormalLines::Build(const Mesh& mesh, float length)
{
    const std::vector<Mesh::Vertex>& vertices = mesh.GetVertices();
    const std::vector<unsigned int>& indices = mesh.GetIndices();

    // Vertex normals, from the vertex position along its normal.
    std::vector<glm::vec3> vertexLines;
    vertexLines.reserve(vertices.size() * 2);
    for (const Mesh::Vertex& vertex : vertices) {
        const glm::vec3 position = PositionOf(vertex);
        const glm::vec3 normal(vertex.normal[0], vertex.normal[1], vertex.normal[2]);
        vertexLines.push_back(position);
        vertexLines.push_back(position + normal * length);
    }

    // Face normals, from the center of each triangle.
    std::vector<glm::vec3> faceLines;
    faceLines.reserve(indices.size() / 3 * 2);
    for (size_t i = 0; i + 2 < indices.size(); i += 3) {
        const glm::vec3 a = PositionOf(vertices[indices[i]]);
        const glm::vec3 b = PositionOf(vertices[indices[i + 1]]);
        const glm::vec3 c = PositionOf(vertices[indices[i + 2]]);

        const glm::vec3 perpendicular = glm::cross(b - a, c - a);
        if (glm::dot(perpendicular, perpendicular) == 0.0f)
            continue;

        const glm::vec3 center = (a + b + c) / 3.0f;
        faceLines.push_back(center);
        faceLines.push_back(center + glm::normalize(perpendicular) * length);
    }

    Upload(vertexVbo, vertexLines);
    vertexPointCount = static_cast<int>(vertexLines.size());
    Upload(faceVbo, faceLines);
    facePointCount = static_cast<int>(faceLines.size());
    builtLength = length;
}

void NormalLines::Draw(const Mesh& mesh, bool vertexNormals, bool faceNormals, float length)
{
    if (!vertexNormals && !faceNormals)
        return;

    if (vertexVbo == 0 || length != builtLength)
        Build(mesh, length);

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);

    if (vertexNormals)
        DrawLines(vertexVbo, vertexPointCount, 0.2f, 1.0f, 0.2f);   // green
    if (faceNormals)
        DrawLines(faceVbo, facePointCount, 0.2f, 0.8f, 1.0f);       // cyan

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glPopAttrib();
}
