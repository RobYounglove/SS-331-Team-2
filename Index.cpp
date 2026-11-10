/** @file Index.cpp
 * @brief Builds a ZIP code index without assuming CSV record order.
 */
#include "Index.h"
#include <fstream>
#include <stdexcept>

bool Index::build(const std::string& fileName) {
    entries.clear();
    // Binary mode keeps seek positions correct for Windows CRLF files.
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    std::getline(file, line); // Skip the column names.
    while (file) {
        std::streamoff position = file.tellg();
        if (!std::getline(file, line)) {
            break;
        }
        std::size_t comma = line.find(',');
        if (comma == std::string::npos) {
            continue;
        }
        std::string zip = line.substr(0, comma);
        if (zip.empty() || zip.size() > 5 ||
            zip.find_first_not_of("0123456789") != std::string::npos) {
            continue;
        }
        entries.insert(std::make_pair(std::stoi(zip), position));
    }
    if (file.bad() || (file.fail() && !file.eof())) {
        entries.clear();
        return false;
    }
    return true;
}

bool Index::find(int zipCode, std::streamoff& position) const {
    auto entry = entries.find(zipCode);
    if (entry == entries.end()) {
        return false;
    }
    position = entry->second;
    return true;
}
