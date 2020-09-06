#include <fstream>
#include <string>
#include <ctime>

#include "nlohmann/json.hpp"
using JSON = nlohmann::json;

#include "valijson/adapters/nlohmann_json_adapter.hpp"
#include "valijson/utils/nlohmann_json_utils.hpp"
#include "valijson/schema.hpp"
#include "valijson/schema_parser.hpp"
#include "valijson/validator.hpp"

#define DEFAULT_SCHEMA_PATH "data/schema.json"
#define DEFAULT_DISSECTOR_PATH "data/example_dissector.json"
#define DEFAULT_OUTPUT_PATH "data/dissector.lua"
#define DEFAULT_CODE_TEMPLATE_PATH "data/code_template"

#define PROJECT_NAME "WiresharkDissectorGenerator"

class WiresharkDissectorGenerator {
private:
  JSON readJSON(const std::string& filePath);
  std::string readCodeTemplate();
  void findAndReplaceAll(std::string& buffer, const std::string& toSearch, const std::string& replaceStr);
  std::string getCurrentDateAndTime();

public:
  WiresharkDissectorGenerator() = default;
  WiresharkDissectorGenerator(const WiresharkDissectorGenerator&) = delete;
  WiresharkDissectorGenerator(const WiresharkDissectorGenerator&&) = delete;

  bool validateDissector(const std::string& _schemaPath, const std::string& _dissectorPath);
  void generateDissector(const std::string& _dissectorPath, const std::string& _outputPath);
};
