cmake_minimum_required(VERSION 4.0.0)

project(vulkan)

set(CMAKE_CXX_STANDARD 23)

add_executable(vulkan 
    src/main.cpp
    src/base.cpp
    src/triangle.cpp
    src/instance.cpp
    src/debugUtils.cpp
    src/surface.cpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h
)