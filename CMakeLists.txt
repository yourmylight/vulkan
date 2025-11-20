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
    target_link_libraries(vulkan PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
    target_link_libraries(vulkan PRIVATE glm::glm)

find_package(Vulkan REQUIRED)
    target_link_libraries(vulkan PRIVATE Vulkan::Vulkan)

target_include_directories(vulkan PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_precompile_headers(vulkan PRIVATE
    ${PROJECT_SOURCE_DIR}/include/pch.h
)