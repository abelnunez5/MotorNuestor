#include <glad/glad.h>
#include "Resources/Mesh.h"

#include <cstddef>
#include <utility>

Mesh::Mesh(std::string name, std::vector<Vertex> vertices, std::vector<unsigned int> indices)
    : name(std::move(name)), vertices(std::move(vertices)), indices(std::move(indices))
{
    Upload();
}

Mesh::~Mesh() { Release(); }

Mesh::Mesh(Mesh&& o) noexcept
    : name(std::move(o.name)), vertices(std::move(o.vertices)), indices(std::move(o.indices)),
    vbo(std::exchange(o.vbo, 0)), ebo(std::exchange(o.ebo, 0)) {
}

Mesh& Mesh::operator=(Mesh&& o) noexcept
{
    if (this != &o) {
        Release();
        name = std::move(o.name);
        vertices = std::move(o.vertices);
        indices = std::move(o.indices);
        vbo = std::exchange(o.vbo, 0);
        ebo = std::exchange(o.ebo, 0);
    }
    return *this;
}

void Mesh::Upload()
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::ComputeBounds()
{
    bounds = AABB{};
    for (const Vertex& v : vertices)
        bounds.Expand(glm::vec3(v.position[0], v.position[1], v.position[2]));
}

void Mesh::Release()
{
    if (vbo) glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
    vbo = ebo = 0;
}

void Mesh::Draw() const
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glNormalPointer(GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}