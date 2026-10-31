#pragma once
#include <string>

class Texture {
public:
    Texture() = default;
    Texture(unsigned int id, int width, int height, std::string path)
        : id(id), width(width), height(height), path(std::move(path)) {
    }
    ~Texture() { Release(); }

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&& other) noexcept { *this = std::move(other); }
    Texture& operator=(Texture&& other) noexcept;

    void Bind() const;
    static void Unbind();

    bool IsValid() const { return id != 0; }
    unsigned int Id() const { return id; }
    int Width() const { return width; }
    int Height() const { return height; }
    const std::string& Path() const { return path; }

private:
    void Release();

    unsigned int id = 0;
    int width = 0;
    int height = 0;
    std::string path;
};

