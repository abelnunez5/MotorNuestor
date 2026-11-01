#include <glad/glad.h>

#include "Modules/ModuleWindow.h"

#include "Core/Log.h"

namespace {

constexpr const char* kTitle = "Engine";
constexpr int kDefaultWidth = 1280;
constexpr int kDefaultHeight = 720;

#ifdef _DEBUG
const char* SourceName(GLenum source)
{
    switch (source) {
    case GL_DEBUG_SOURCE_API:             return "API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Window System";
    case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
    case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Third Party";
    case GL_DEBUG_SOURCE_APPLICATION:     return "Application";
    default:                              return "Other";
    }
}

const char* TypeName(GLenum type)
{
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:               return "Error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behaviour";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined Behaviour";
    case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
    case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
    case GL_DEBUG_TYPE_MARKER:              return "Marker";
    case GL_DEBUG_TYPE_PUSH_GROUP:          return "Push Group";
    case GL_DEBUG_TYPE_POP_GROUP:           return "Pop Group";
    default:                                return "Other";
    }
}

void APIENTRY OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                  GLsizei /*length*/, const GLchar* message, const void* /*userParam*/)
{
    if (severity == GL_DEBUG_SEVERITY_HIGH)
        LOG_ERROR("OpenGL <%s> <%s> <ID:%u>: %s", SourceName(source), TypeName(type), id, message);
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
        LOG_WARNING("OpenGL <%s> <%s> <ID:%u>: %s", SourceName(source), TypeName(type), id, message);
}
#endif

}

bool ModuleWindow::Init()
{
    LOG("Initializing SDL...");
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        LOG_ERROR("SDL_Init failed: %s", SDL_GetError());
        return false;
    }
    const int sdlVersion = SDL_GetVersion();
    LOG("SDL %d.%d.%d initialized",
        SDL_VERSIONNUM_MAJOR(sdlVersion), SDL_VERSIONNUM_MINOR(sdlVersion), SDL_VERSIONNUM_MICRO(sdlVersion));

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    window = SDL_CreateWindow(kTitle, kDefaultWidth, kDefaultHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        LOG_ERROR("SDL_CreateWindow failed: %s", SDL_GetError());
        return false;
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        LOG_ERROR("SDL_GL_CreateContext failed: %s", SDL_GetError());
        return false;
    }

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)) == 0) {
        LOG_ERROR("Failed to load OpenGL functions (glad)");
        return false;
    }

#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLDebugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

    LOG("OpenGL %s", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    LOG("GPU: %s (%s)", reinterpret_cast<const char*>(glGetString(GL_RENDERER)),
        reinterpret_cast<const char*>(glGetString(GL_VENDOR)));

    SetVSync(true);

    SDL_GetWindowSizeInPixels(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    return true;
}

UpdateStatus ModuleWindow::PreUpdate()
{
    // Polling the size every frame avoids depending on resize events.
    int w = 0;
    int h = 0;
    SDL_GetWindowSizeInPixels(window, &w, &h);
    if (w > 0 && h > 0 && (w != width || h != height)) {
        width = w;
        height = h;
        glViewport(0, 0, width, height);
    }

    return UpdateStatus::Continue;
}

void ModuleWindow::CleanUp()
{
    LOG("Destroying window and SDL");

    if (glContext) {
        SDL_GL_DestroyContext(glContext);
        glContext = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void ModuleWindow::SwapBuffers() const
{
    SDL_GL_SwapWindow(window);
}

void ModuleWindow::SetVSync(bool enabled)
{
    if (SDL_GL_SetSwapInterval(enabled ? 1 : 0))
        vsync = enabled;
    else
        LOG_WARNING("Could not change vsync: %s", SDL_GetError());
}

bool ModuleWindow::IsFullscreen() const
{
    return (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) != 0;
}

void ModuleWindow::SetFullscreen(bool enabled)
{
    if (!SDL_SetWindowFullscreen(window, enabled))
        LOG_WARNING("Could not change fullscreen: %s", SDL_GetError());
}

bool ModuleWindow::IsResizable() const
{
    return (SDL_GetWindowFlags(window) & SDL_WINDOW_RESIZABLE) != 0;
}

void ModuleWindow::SetResizable(bool enabled)
{
    SDL_SetWindowResizable(window, enabled);
}

bool ModuleWindow::IsBordered() const
{
    return (SDL_GetWindowFlags(window) & SDL_WINDOW_BORDERLESS) == 0;
}

void ModuleWindow::SetBordered(bool enabled)
{
    SDL_SetWindowBordered(window, enabled);
}

void ModuleWindow::GetSize(int& w, int& h) const
{
    SDL_GetWindowSize(window, &w, &h);
}

void ModuleWindow::SetSize(int w, int h)
{
    SDL_SetWindowSize(window, w, h);
}
