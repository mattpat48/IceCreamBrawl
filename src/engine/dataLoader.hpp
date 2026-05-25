#pragma once

#include <string>
#include <fstream>
#include <streambuf>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class DataLoader {
public:
    // Load and parse a JSON file. Throws on error.
    static json loadJsonFile(const std::string& path) {
        std::ifstream in(path);
        if (!in.is_open()) {
            throw std::runtime_error("Cannot open JSON file: " + path);
        }
        json j;
        in >> j;
        return j;
    }
};
