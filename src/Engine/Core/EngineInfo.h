#pragma once

namespace EngineInfo {

inline constexpr const char* kName = "MotorNuestor";
inline constexpr const char* kVersion = "0.1";
inline constexpr const char* kLicense = "MIT License";

// Repository URL
inline constexpr const char* kRepoUrl = "https://github.com/abelnunez5/MotorNuestor";

// Team members (name + GitHub).
inline constexpr const char* kTeam[] = {
    "Abel Nuñez (github.com/abelnunez5)",
    "Andrea Velez (github.com/veawenn)",
    "Pol Cuenca (github.com/Lexmaoo)",
};

inline constexpr const char* kLibraries[] = {
    "SDL3", "OpenGL", "glad", "glm", "Dear ImGui", "Assimp", "DevIL",
};
}
