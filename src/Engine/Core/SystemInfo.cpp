#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <psapi.h>
#ifdef _MSC_VER
#pragma comment(lib, "psapi.lib")
#endif
#endif

#include <glad/glad.h>
#include "Core/SystemInfo.h"

#include <cstdio>
#include <cstring>

#include <IL/il.h>
#include <SDL3/SDL.h>
#include <assimp/version.h>
#include <glm/glm.hpp>

namespace {

    enum class GpuMemoryApi { None, Nvidia, Ati };

    constexpr GLenum kNvxTotalAvailableKB = 0x9048;
    constexpr GLenum kNvxCurrentAvailableKB = 0x9049;
    constexpr GLenum kAtiTextureFreeMemory = 0x87FC;

    std::string Version(int major, int minor, int patch)
    {
        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "%d.%d.%d", major, minor, patch);
        return buffer;
    }

    const char* GlString(GLenum name)
    {
        const char* value = reinterpret_cast<const char*>(glGetString(name));
        return value ? value : "unknown";
    }

    bool HasGlExtension(const char* extension)
    {
        GLint count = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &count);
        for (GLint i = 0; i < count; ++i) {
            const char* name = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, static_cast<GLuint>(i)));
            if (name && std::strcmp(name, extension) == 0)
                return true;
        }
        return false;
    }

    GpuMemoryApi DetectGpuMemoryApi()
    {
        if (HasGlExtension("GL_NVX_gpu_memory_info"))
            return GpuMemoryApi::Nvidia;
        if (HasGlExtension("GL_ATI_meminfo"))
            return GpuMemoryApi::Ati;
        return GpuMemoryApi::None;
    }

    std::string CpuFeatures()
    {
        std::string features;
        const auto add = [&features](bool supported, const char* name) {
            if (!supported)
                return;
            if (!features.empty())
                features += ", ";
            features += name;
        };

        add(SDL_HasMMX(), "MMX");
        add(SDL_HasSSE(), "SSE");
        add(SDL_HasSSE2(), "SSE2");
        add(SDL_HasSSE3(), "SSE3");
        add(SDL_HasSSE41(), "SSE4.1");
        add(SDL_HasSSE42(), "SSE4.2");
        add(SDL_HasAVX(), "AVX");
        add(SDL_HasAVX2(), "AVX2");
        add(SDL_HasAVX512F(), "AVX512F");
        return features;
    }

    SystemInfo::Info Gather()
    {
        SystemInfo::Info info;

        const int sdl = SDL_GetVersion();
        info.sdlVersion = Version(SDL_VERSIONNUM_MAJOR(sdl), SDL_VERSIONNUM_MINOR(sdl), SDL_VERSIONNUM_MICRO(sdl));
        info.glVersion = GlString(GL_VERSION);
        info.glslVersion = GlString(GL_SHADING_LANGUAGE_VERSION);

        // DevIL reports its version as one number: 180 = 1.8.0
        const int il = static_cast<int>(ilGetInteger(IL_VERSION_NUM));
        info.devilVersion = Version(il / 100, (il / 10) % 10, il % 10);

        info.assimpVersion = Version(static_cast<int>(aiGetVersionMajor()),
                                     static_cast<int>(aiGetVersionMinor()),
                                     static_cast<int>(aiGetVersionPatch()));
        info.glmVersion = Version(GLM_VERSION_MAJOR, GLM_VERSION_MINOR, GLM_VERSION_PATCH);

        info.gpuRenderer = GlString(GL_RENDERER);
        info.gpuVendor = GlString(GL_VENDOR);
        info.cpuFeatures = CpuFeatures();
        info.cpuCores = SDL_GetNumLogicalCPUCores();
        info.cpuCacheLineBytes = SDL_GetCPUCacheLineSize();
        info.systemRamMB = SDL_GetSystemRAM();
        return info;
    }

}

const SystemInfo::Info& SystemInfo::Get()
{
    static const Info info = Gather();
    return info;
}

SystemInfo::MemoryUsage SystemInfo::GetMemoryUsage()
{
    MemoryUsage usage;

#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS counters{};
    counters.cb = sizeof(counters);
    if (GetProcessMemoryInfo(GetCurrentProcess(), &counters, sizeof(counters))) {
        usage.processWorkingSet = counters.WorkingSetSize;
        usage.processPeak = counters.PeakWorkingSetSize;
    }
#endif

    static const GpuMemoryApi api = DetectGpuMemoryApi();
    if (api == GpuMemoryApi::Nvidia) {
        GLint totalKB = 0;
        GLint availableKB = 0;
        glGetIntegerv(kNvxTotalAvailableKB, &totalKB);
        glGetIntegerv(kNvxCurrentAvailableKB, &availableKB);
        usage.gpuTotal = static_cast<uint64_t>(totalKB) * 1024;
        usage.gpuAvailable = static_cast<uint64_t>(availableKB) * 1024;
    }
    else if (api == GpuMemoryApi::Ati) {
        GLint free[4] = {};   // [0] = total free memory in the pool, in KB
        glGetIntegerv(kAtiTextureFreeMemory, free);
        usage.gpuAvailable = static_cast<uint64_t>(free[0]) * 1024;
    }

    return usage;
}
