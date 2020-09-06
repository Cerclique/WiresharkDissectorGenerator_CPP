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

void WiresharkDissectorGenerator::findAndReplaceAll(std::string& buffer, const std::string& toSearch, const std::string& replaceStr) {
  auto index = buffer.find(toSearch);

  while (index != std::string::npos) {
    buffer.replace(index, toSearch.size(), replaceStr);
    index = buffer.find(toSearch, index + replaceStr.size());
  }
}

std::string WiresharkDissectorGenerator::getCurrentDateAndTime() {
  std::locale::global(std::locale(""));
  std::time_t t = std::time(nullptr);
  char mbstr[100];
  std::strftime(mbstr, sizeof(mbstr), "%A %c", std::localtime(&t));

  return std::string(mbstr);
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

void WiresharkDissectorGenerator::generateDissector(const std::string& _dissectorPath, const std::string& _outputPath) {
  auto dissectorJSON = this->readJSON(_dissectorPath);
  auto outputBuffer = this->readCodeTemplate();

  /** %PROJECT_NAME% **/
  this->findAndReplaceAll(outputBuffer, "%PROJECT_NAME%", PROJECT_NAME);

  /** %PROTOCOL_NAME **/
  const auto dissectorName = dissectorJSON.at("name").get<std::string>();
  this->findAndReplaceAll(outputBuffer, "%PROTOCOL_NAME%", dissectorName);

  /** %DATE% **/
  this->findAndReplaceAll(outputBuffer, "%DATE%", getCurrentDateAndTime());

  /**
     %FIELDS_LIST%
     %FIELDS_DECLARATION%
     %LOCAL_VAR_DECLARATION%
     %SUBTREE_POPULATION%
  **/
  auto data = dissectorJSON.at("data").get<std::vector<JSON>>();

  std::string fieldDeclarationBuffer = "";
  std::string fieldListBuffer = "";
  std::string localVariableDeclarationBuffer = "";
  std::string subtreePopulationBuffer = "";

  for (const auto& field : data) {
    const auto fullFilterName = dissectorJSON.at("name").get<std::string>() + "." + field.at("filter").get<std::string>();
    const auto filter = field.at("filter").get<std::string>();
    const auto type = field.at("type").get<std::string>();
    const auto description = field.at("short_description").get<std::string>();
    const auto base = field.at("base").get<std::string>();
    const auto name = field.at("name").get<std::string>();
    const auto offset = std::to_string(field.at("offset").get<int>());
    const auto size = std::to_string(field.at("size").get<int>());

    fieldDeclarationBuffer += filter + "=ProtoField." + type + "(\"" + fullFilterName + "\", \"" + description + "\", base." + base + ")\n";
    fieldListBuffer += filter + ",\n\t";
    localVariableDeclarationBuffer += "local" + filter + " = buffer(" + offset + ", " + size + ")\n\t";
    subtreePopulationBuffer += "subtree:add(" + filter + ", " + name + ")\n\t";
  }

  fieldDeclarationBuffer.resize(fieldDeclarationBuffer.size() - 1);
  fieldListBuffer.resize(fieldListBuffer.size() - 2);
  localVariableDeclarationBuffer.resize(localVariableDeclarationBuffer.size() - 2);

  /** %FIELDS_LIST% **/
  this->findAndReplaceAll(outputBuffer, "%FIELDS_LIST%", fieldListBuffer);

  /** %FIELDS_DECLARATION% **/
  this->findAndReplaceAll(outputBuffer, "%FIELDS_DECLARATION%", fieldDeclarationBuffer);

  /** "%LOCAL_VAR_DECLARATION% **/
  this->findAndReplaceAll(outputBuffer, "%LOCAL_VAR_DECLARATION%", localVariableDeclarationBuffer);

  /** %SUBTREE_POPULATION% **/
  this->findAndReplaceAll(outputBuffer, "%SUBTREE_POPULATION%", fieldListBuffer);

  /* %PROTOCOL% **/
  const auto protocol = dissectorJSON.at("connection").get<JSON>().at("protocol").get<std::string>();
  this->findAndReplaceAll(outputBuffer, "%PROTOCOL%", protocol);

  /** %PORTS% **/
  const auto ports = dissectorJSON.at("connection").get<JSON>().at("ports").get<std::vector<uint32_t>>();
  std::string portBuffer = "";
  for (const auto& port : ports) {
    portBuffer += protocol + "_port:add(" + std::to_string(port) + ", " + dissectorName + ")\n";
  }
  this->findAndReplaceAll(outputBuffer, "%PORTS%", portBuffer);

  /** Write to output file **/
  std::ofstream fileStream(_outputPath, std::ofstream::trunc);
  if (not fileStream.is_open()) {
    throw std::runtime_error("Could not open " + _outputPath);
  }

  fileStream << outputBuffer;
  fileStream.close();
}
