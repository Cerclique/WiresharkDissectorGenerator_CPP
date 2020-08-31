#include "WiresharkDissectorGenerator.hpp"
#include <iostream>

WiresharkDissectorGenerator::WiresharkDissectorGenerator() :
  schemaPath(DEFAULT_SCHEMA_PATH), dissectorPath(DEFAULT_DISSECTOR_PATH), outputPath(DEFAULT_OUTPUT_PATH) {

}

WiresharkDissectorGenerator::WiresharkDissectorGenerator(const std::string& _schemaPath, const std::string& _dissectorPath, const std::string& _outputPath) :
  schemaPath(_schemaPath), dissectorPath(_dissectorPath), outputPath(_outputPath) {

}

WiresharkDissectorGenerator::WiresharkDissectorGenerator(const std::string& _dissectorPath, const std::string& _outputPath) :
  schemaPath(DEFAULT_SCHEMA_PATH), dissectorPath(_dissectorPath), outputPath(_outputPath) {

}

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
