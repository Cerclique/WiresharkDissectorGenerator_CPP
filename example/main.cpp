#include "WiresharkDissectorGenerator.hpp"

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    WDG::WiresharkDissectorGenerator generator;

    try {
      if (generator.validateDissector(DEFAULT_SCHEMA_PATH, DEFAULT_DISSECTOR_PATH)) {
        generator.generateDissector(DEFAULT_DISSECTOR_PATH, DEFAULT_OUTPUT_PATH);
      }
      else {
        std::cout << "Validation failed" << std::endl;
      }
    }
    catch(const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }

    return 0;
}
