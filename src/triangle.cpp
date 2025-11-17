#include <triangle.h>

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