cmake_minimum_required(VERSION 4.0.0)

project(vulkan)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE CORE_SOURCES "${PROJECT_SOURCE_DIR}/src/core/*.cpp")

add_executable(vulkan 
    src/main.cpp
    src/base.cpp
    src/triangle.cpp
    src/debugUtils.cpp
    ${CORE_SOURCES}
)

find_package(glfw3 CONFIG REQUIRED)

find_package(glm CONFIG REQUIRED)

find_package(Vulkan REQUIRED)

target_include_directories(vulkan PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(vulkan PRIVATE
    glfw
    glm::glm
    Vulkan::Vulkan
)

target_precompile_headers(vulkan PRIVATE
    ${PROJECT_SOURCE_DIR}/include/pch.h
)