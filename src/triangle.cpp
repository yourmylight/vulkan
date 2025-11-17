#include <triangle.h>

void Triangle::prepare() {
    Base::prepare();
}

void Triangle::renderLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

Triangle::Triangle(int _width, int _height) : Base(_width, _height) {
    
}

Triangle::~Triangle() {
    
}