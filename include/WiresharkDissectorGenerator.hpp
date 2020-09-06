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
  [[nodiscard]] const JSON readJSON(const std::string& filePath) const;
  [[nodiscard]] std::string readCodeTemplate() const;
  void findAndReplaceAll(std::string& buffer, const std::string& toSearch, const std::string& replaceStr) const noexcept;
  [[nodiscard]] const std::string getCurrentDateAndTime() const noexcept;

public:
  WiresharkDissectorGenerator() = default;
  WiresharkDissectorGenerator(const WiresharkDissectorGenerator&) = delete;
  WiresharkDissectorGenerator(const WiresharkDissectorGenerator&&) = delete;

  [[nodiscard]] bool validateDissector(const std::string& _schemaPath, const std::string& _dissectorPath) const;
  void generateDissector(const std::string& _dissectorPath, const std::string& _outputPath) const;
};
