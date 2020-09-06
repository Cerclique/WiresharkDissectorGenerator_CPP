#include <iostream>
#include "WiresharkDissectorGenerator.hpp"

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    WiresharkDissectorGenerator generator;

    try {
      if (not generator.validateDissector(DEFAULT_SCHEMA_PATH, DEFAULT_DISSECTOR_PATH)) {
        std::cout << "Validation failed" << std::endl;
      }
    }
    catch(const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
    return 0;
}
