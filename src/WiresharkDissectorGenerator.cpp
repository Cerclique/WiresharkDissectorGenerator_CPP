#include "WiresharkDissectorGenerator.hpp"
#include <iostream>

JSON WiresharkDissectorGenerator::readJSON(const std::string& filePath) {
  std::ifstream fileStream(filePath);

  if (not fileStream.is_open()) {
    throw std::runtime_error("File not found: " + filePath);
  }

  JSON fileJSON;
  fileStream >> fileJSON;
  fileStream.close();

  return fileJSON;
}

std::string WiresharkDissectorGenerator::readCodeTemplate() {
  std::ifstream fileStream(DEFAULT_CODE_TEMPLATE_PATH);

  if (not fileStream.is_open()) {
    throw std::runtime_error("File not found: " + std::string(DEFAULT_CODE_TEMPLATE_PATH));
  }

  std::string buffer;

  fileStream.seekg(0, std::ios::end);
  buffer.reserve(fileStream.tellg());
  fileStream.seekg(0, std::ios::beg);

  buffer.assign((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
  fileStream.close();

  return buffer;
}

bool WiresharkDissectorGenerator::validateDissector(const std::string& _schemaPath, const std::string& _dissectorPath) {

  auto schemaJSON = this->readJSON(_schemaPath);
  auto dissectorJSON = this->readJSON(_dissectorPath);

  valijson::adapters::NlohmannJsonAdapter schemaAdapter(schemaJSON);
  valijson::adapters::NlohmannJsonAdapter dissectorAdapter(dissectorJSON);

  valijson::Schema schema;
  valijson::SchemaParser schemaParser;
  schemaParser.populateSchema(schemaAdapter, schema);

  valijson::ValidationResults validationResults;
  valijson::Validator validator;

  auto isDissectorValid = validator.validate(schema, dissectorAdapter, &validationResults);

  if (not isDissectorValid) {
    valijson::ValidationResults::Error error;
    uint32_t errorNum = 1;
    while (validationResults.popError(error)) {
      std::cerr << "Error #" << errorNum << std::endl;
      std::cerr << "\t";
      for (const std::string &contextElement : error.context) {
        std::cerr << contextElement << " ";
      }
      std::cerr << std::endl;
      std::cerr << "    - " << error.description << std::endl;
      ++errorNum;
    }
  }

  return isDissectorValid;
}
