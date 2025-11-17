#include <triangle.h>

int main(int argc, char* argv[]) {
    auto app = std::make_unique<Triangle>(1280, 720);
    
    try {
        app->run();
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}