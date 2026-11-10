/** @file Search.cpp
 * @brief Seeks directly to indexed ZIP records and unpacks their fields.
 */
#include "Search.h"
#include "LengthIndicatedBuffer.h"
#include <stdexcept>
#include <vector>

bool Search::open(const std::string& fileName) {
    if (file.is_open()) {
        file.close();
    }
    file.clear();
    if (!index.build(fileName)) {
        return false;
    }
    file.open(fileName, std::ios::binary);
    return file.is_open();
}

bool Search::findZipCode(int zipCode, Buffer& buffer) {
    std::streamoff position;
    if (!file.is_open() || !index.find(zipCode, position)) {
        return false;
    }
    file.clear();
    file.seekg(position);
    std::string line;
    if (!std::getline(file, line)) {
        return false;
    }

    LengthIndicatedBuffer record;
    record.pack(line);
    Buffer result;
    const std::vector<std::string> fieldNames = {
        "Zip Code", "Place Name", "State", "County", "Lat", "Long"
    };
    try {
        if (!record.unpack(result, fieldNames) || result.getZipCode() != zipCode) {
            return false;
        }
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }

    // Copy only record fields; retain the caller's accumulated extrema.
    buffer.setZipCode(result.getZipCode());
    buffer.setPlaceName(result.getPlaceName());
    buffer.setState(result.getState());
    buffer.setLatitude(result.getLatitude());
    buffer.setLongitude(result.getLongitude());
    return true;
}
