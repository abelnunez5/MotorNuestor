cmake_minimum_required(VERSION 3.10)

project(MotorNuestor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(DevIL REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/*.h)

add_library(Engine STATIC ${ENGINE_SOURCES})

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine)

target_link_libraries(Engine
    PUBLIC  SDL3::SDL3 glad::glad glm::glm
    PRIVATE DevIL::IL DevIL::ILU assimp::assimp)

target_compile_options(Engine PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine FILES ${ENGINE_SOURCES})

file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor/*.h)

add_library(Editor STATIC ${EDITOR_SOURCES})
target_include_directories(Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor)
target_link_libraries(Editor PUBLIC Engine imgui::imgui)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor FILES ${EDITOR_SOURCES})

add_executable(MotorNuestor src/main.cpp)
target_link_libraries(MotorNuestor PRIVATE Engine Editor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MotorNuestor)
set_target_properties(MotorNuestor PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:MotorNuestor>")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Assets)
 
add_custom_command(TARGET MotorNuestor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/Assets
            $<TARGET_FILE_DIR:MotorNuestor>/Assets)