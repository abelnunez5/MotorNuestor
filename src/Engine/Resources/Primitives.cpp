#include "Resources/Primitives.h"

#include <cmath>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

namespace {

constexpr float kPi = 3.14159265358979323846f;

using Vertices = std::vector<Mesh::Vertex>;
using Indices = std::vector<unsigned int>;
using V = glm::vec3;

Mesh::Vertex MakeVertex(const glm::vec3& position, const glm::vec3& normal, float u, float v)
{
    Mesh::Vertex vertex{};
    vertex.position[0] = position.x;
    vertex.position[1] = position.y;
    vertex.position[2] = position.z;
    vertex.normal[0] = normal.x;
    vertex.normal[1] = normal.y;
    vertex.normal[2] = normal.z;
    vertex.uv[0] = u;
    vertex.uv[1] = v;
    return vertex;
}

void AddQuad(Vertices& vertices, Indices& indices, const glm::vec3& normal,
             const glm::vec3& u, const glm::vec3& v, float distance, float halfSize)
{
    const unsigned int base = static_cast<unsigned int>(vertices.size());
    const glm::vec3 center = normal * distance;
    const glm::vec3 du = u * halfSize;
    const glm::vec3 dv = v * halfSize;

    vertices.push_back(MakeVertex(center - du - dv, normal, 0.0f, 0.0f));
    vertices.push_back(MakeVertex(center + du - dv, normal, 1.0f, 0.0f));
    vertices.push_back(MakeVertex(center + du + dv, normal, 1.0f, 1.0f));
    vertices.push_back(MakeVertex(center - du + dv, normal, 0.0f, 1.0f));

    indices.insert(indices.end(), { base, base + 1, base + 2, base, base + 2, base + 3 });
}

void BuildCube(Vertices& vertices, Indices& indices)
{
    constexpr float h = 0.5f;
    AddQuad(vertices, indices, V(1, 0, 0), V(0, 0, -1), V(0, 1, 0), h, h);   // +X
    AddQuad(vertices, indices, V(-1, 0, 0), V(0, 0, 1), V(0, 1, 0), h, h);   // -X
    AddQuad(vertices, indices, V(0, 1, 0), V(1, 0, 0), V(0, 0, -1), h, h);   // +Y
    AddQuad(vertices, indices, V(0, -1, 0), V(1, 0, 0), V(0, 0, 1), h, h);   // -Y
    AddQuad(vertices, indices, V(0, 0, 1), V(1, 0, 0), V(0, 1, 0), h, h);    // +Z
    AddQuad(vertices, indices, V(0, 0, -1), V(-1, 0, 0), V(0, 1, 0), h, h);  // -Z
}

void BuildPlane(Vertices& vertices, Indices& indices)
{
    AddQuad(vertices, indices, V(0, 1, 0), V(1, 0, 0), V(0, 0, -1), 0.0f, 5.0f);
}

void BuildSphere(Vertices& vertices, Indices& indices)
{
    constexpr int slices = 32;   // around the Y axis
    constexpr int stacks = 16;   // from the top pole to the bottom one
    constexpr float radius = 0.5f;

    for (int i = 0; i <= stacks; ++i) {
        const float theta = kPi * static_cast<float>(i) / stacks;
        const float y = std::cos(theta);
        const float ring = std::sin(theta);
        for (int j = 0; j <= slices; ++j) {
            const float phi = 2.0f * kPi * static_cast<float>(j) / slices;
            const glm::vec3 normal(ring * std::cos(phi), y, ring * std::sin(phi));
            vertices.push_back(MakeVertex(normal * radius, normal,
                                          static_cast<float>(j) / slices, 1.0f - static_cast<float>(i) / stacks));
        }
    }

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            const unsigned int a = static_cast<unsigned int>(i * (slices + 1) + j);
            const unsigned int b = a + slices + 1;
            indices.insert(indices.end(), { a, a + 1, b, a + 1, b + 1, b });
        }
    }
}

void BuildCylinder(Vertices& vertices, Indices& indices)
{
    constexpr int slices = 32;
    constexpr float radius = 0.5f;
    constexpr float halfHeight = 0.5f;

    for (int j = 0; j <= slices; ++j) {
        const float phi = 2.0f * kPi * static_cast<float>(j) / slices;
        const glm::vec3 direction(std::cos(phi), 0.0f, std::sin(phi));
        const float u = static_cast<float>(j) / slices;
        vertices.push_back(MakeVertex(direction * radius - glm::vec3(0, halfHeight, 0), direction, u, 0.0f));
        vertices.push_back(MakeVertex(direction * radius + glm::vec3(0, halfHeight, 0), direction, u, 1.0f));
    }
    for (int j = 0; j < slices; ++j) {
        const unsigned int bottom0 = static_cast<unsigned int>(2 * j);
        const unsigned int top0 = bottom0 + 1;
        const unsigned int bottom1 = bottom0 + 2;
        const unsigned int top1 = bottom0 + 3;
        indices.insert(indices.end(), { bottom0, top0, bottom1, bottom1, top0, top1 });
    }

    for (int cap = 0; cap < 2; ++cap) {
        const bool top = cap == 1;
        const float y = top ? halfHeight : -halfHeight;
        const glm::vec3 normal(0.0f, top ? 1.0f : -1.0f, 0.0f);

        const unsigned int center = static_cast<unsigned int>(vertices.size());
        vertices.push_back(MakeVertex(glm::vec3(0.0f, y, 0.0f), normal, 0.5f, 0.5f));

        const unsigned int firstRing = static_cast<unsigned int>(vertices.size());
        for (int j = 0; j <= slices; ++j) {
            const float phi = 2.0f * kPi * static_cast<float>(j) / slices;
            const float c = std::cos(phi);
            const float s = std::sin(phi);
            vertices.push_back(MakeVertex(glm::vec3(c * radius, y, s * radius), normal, 0.5f + 0.5f * c, 0.5f + 0.5f * s));
        }
        for (int j = 0; j < slices; ++j) {
            const unsigned int a = firstRing + static_cast<unsigned int>(j);
            if (top)
                indices.insert(indices.end(), { center, a + 1, a });
            else
                indices.insert(indices.end(), { center, a, a + 1 });
        }
    }
}

}

const char* Primitives::Name(Type type)
{
    switch (type) {
    case Type::Cube:     return "Cube";
    case Type::Sphere:   return "Sphere";
    case Type::Plane:    return "Plane";
    case Type::Cylinder: return "Cylinder";
    }
    return "Primitive";
}

Mesh Primitives::Create(Type type)
{
    Vertices vertices;
    Indices indices;

    switch (type) {
    case Type::Cube:     BuildCube(vertices, indices);     break;
    case Type::Sphere:   BuildSphere(vertices, indices);   break;
    case Type::Plane:    BuildPlane(vertices, indices);    break;
    case Type::Cylinder: BuildCylinder(vertices, indices); break;
    }

    return Mesh(Name(type), std::move(vertices), std::move(indices));
}
