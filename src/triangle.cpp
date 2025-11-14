#include <triangle.h>

void Triangle::run() {
    Base::setupWindow();
    Base::initVulkan();
    prepare();
    renderLoop();
}

void Triangle::prepare() {
    Base::prepare();
}

void Triangle::renderLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

Triangle::~Triangle() {
    
}