#ifndef _H_WIRESHARK_DISSECTOR_GENERATOR
#define _H_WIRESHARK_DISSECTOR_GENERATOR

#include <fstream>
#include <string_view>
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
  [[nodiscard]] const JSON readJSON(const std::string_view& filePath) const;
  [[nodiscard]] std::string readCodeTemplate() const;
  void findAndReplaceAll(std::string& buffer, const std::string_view& toSearch, const std::string_view& replaceStr) const noexcept;
  [[nodiscard]] const std::string getCurrentDateAndTime() const noexcept;

public:
  WiresharkDissectorGenerator() = default;
  WiresharkDissectorGenerator(const WiresharkDissectorGenerator&) = delete;
  WiresharkDissectorGenerator(const WiresharkDissectorGenerator&&) = delete;

  WiresharkDissectorGenerator& operator=(const WiresharkDissectorGenerator& rhs) = delete;

  [[nodiscard]] bool validateDissector(const std::string_view& _schemaPath, const std::string_view& _dissectorPath) const;
  void generateDissector(const std::string_view& _dissectorPath, const std::string_view& _outputPath) const;
};

#endif
