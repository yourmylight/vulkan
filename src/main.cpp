#include <triangle.h>

int main() {
    Base* app = new Triangle;
    
    try {
        app->run();
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}