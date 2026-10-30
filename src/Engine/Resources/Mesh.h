#pragma once
#include <string>
#include <vector>

class Mesh {
public:
    struct Vertex {
        float position[3];
        float normal[3];
        float uv[2];
    };

    Mesh(std::string name, std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void Draw() const;

    const std::string& GetName() const { return name; }
    const std::vector<Vertex>& GetVertices() const { return vertices; }
    const std::vector<unsigned int>& GetIndices() const { return indices; }

private:
    void Upload();
    void Release();

    std::string name;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int vbo = 0;
    unsigned int ebo = 0;
};