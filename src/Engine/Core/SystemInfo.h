#pragma once

#include <cstdint>
#include <string>

namespace SystemInfo {

struct Info {
    // Software versions
    std::string sdlVersion;
    std::string glVersion;
    std::string glslVersion;
    std::string devilVersion;
    std::string assimpVersion;
    std::string glmVersion;

    // Hardware
    std::string gpuRenderer;
    std::string gpuVendor;
    std::string cpuFeatures;
    int cpuCores = 0;
    int cpuCacheLineBytes = 0;
    int systemRamMB = 0;
};

const Info& Get();

struct MemoryUsage {
    uint64_t processWorkingSet = 0;
    uint64_t processPeak = 0;
    uint64_t gpuTotal = 0;
    uint64_t gpuAvailable = 0;
};

MemoryUsage GetMemoryUsage();

}
