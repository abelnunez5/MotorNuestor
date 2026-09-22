cmake_minimum_required(VERSION 3.10)

project(MotorNuestor)

find_package(SDL3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(MotorNuestor main.cpp)

target_link_libraries(MotorNuestor PRIVATE SDL3::SDL3 glad::glad glm:glm)