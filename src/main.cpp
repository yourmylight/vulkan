#include <triangle.h>

int main(int argc, char* argv[]) {
    Base* app = new Triangle();
    
    try {
        app->run();
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        delete app;
        return EXIT_FAILURE;
    }
    delete app;

    return 0;
}