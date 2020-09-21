# WiresharkDissectorGenerator (C++ version)

## Introduction

This tool generate for you a dissector plugin for Wireshark based on a JSON description of the packet you want to analyze. 
Once generated, you just have import the LUA plugin into Wireshark and let the magic begin !

This project is a C++ porting of my orignal JS Dissector Generator for Wireshark : [WiresharkDissectorGenerator](https://github.com/Cerclique/WiresharkDissectorGenerator).

## Dependencies

In order to compile :
- Clang 5 or later (Support for C++17)
- [Alternative] GCC 5 or later.

Following your compiler, you might need to modify the `Makefile` (CXX variable). By default the project use `clang` to compile.

This project depends on two "header-only" projects (included in `external_include` folder) :
- [nlohmann/json](https://github.com/nlohmann/json) (v3.9.1) : JSON for modern C++
- [tristanpenman/valijson](https://github.com/tristanpenman/valijson) (v0.3) : JSON schema validation

## Installation && Usage

- Clone the repository
- Compile the project using the `Makefile`
- Run the sample `WiresharkDissectorGenerator`

The generated LUA plugin can be found in `data` folder.
