#pragma once

class Framebuffer {
public:
    Framebuffer() = default;
    ~Framebuffer() { Release(); }

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    void Resize(int width, int height);

    void Bind() const;
    void Unbind() const;

    void Release();

    bool IsValid() const { return fbo != 0; }
    unsigned int ColorTexture() const { return colorTexture; }
    int Width() const { return width; }
    int Height() const { return height; }

private:
    unsigned int fbo = 0;
    unsigned int colorTexture = 0;
    unsigned int depthStencil = 0;
    int width = 0;
    int height = 0;
};