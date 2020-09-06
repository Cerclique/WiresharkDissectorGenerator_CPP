#include <fstream>
#include <string>

#include "nlohmann/json.hpp"
using JSON = nlohmann::json;

#define DEFAULT_SCHEMA_PATH "data/schema.json"
#define DEFAULT_DISSECTOR_PATH "data/example_dissector.json"
#define DEFAULT_OUTPUT_PATH "data/dissector.lua"
#define DEFAULT_CODE_TEMPLATE_PATH "data/code_template"

class WiresharkDissectorGenerator {
private:
  std::string schemaPath;
  std::string dissectorPath;
  std::string outputPath;

  JSON readJSON(const std::string& filePath);
  std::string readCodeTemplate();

public:
  WiresharkDissectorGenerator();
  WiresharkDissectorGenerator(const std::string& _schemaPath,const std::string& _dissectorPath,const  std::string& _outputPath);
  WiresharkDissectorGenerator(const std::string& _dissectorPath, const std::string& _ouputPath);
  WiresharkDissectorGenerator(const WiresharkDissectorGenerator&) = delete;
  WiresharkDissectorGenerator(const WiresharkDissectorGenerator&&) = delete;

  void setSchemaPath(const std::string& _schemaPath);
  void setDissectorPath(const std::string& _dissectorPath);
  void setOutputPath(const std::string& _outputPath);

  std::string getSchemaPath() const;
  std::string getDissectorPath() const;
  std::string getOutputPath() const;
};
