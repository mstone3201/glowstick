#include <glowstick/glowstick.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    try {
        glowstick::renderer renderer;
    } catch(glowstick::error& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
