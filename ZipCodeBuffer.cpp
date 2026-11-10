
/**
 * @file ZipCodeBuffer.cpp
 * @brief Implements ZIP code CSV parsing and geographic extrema.
 */

#include "ZipCodeBuffer.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cmath>

using namespace std;

// Remove surrounding whitespace.
static string trim(const string& value) {
    size_t start = value.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";

    size_t end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

// Split a CSV row while respecting quoted fields.
static vector<string> splitCSV(const string& line) {
    vector<string> fields;
    string field;
    bool quoted = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];

        if (c == '"') {
            if (quoted && i + 1 < line.size()
                       && line[i + 1] == '"') {
                field += '"';
                ++i;
            } else {
                quoted = !quoted;
            }
        } else if (c == ',' && !quoted) {
            fields.push_back(trim(field));
            field.clear();
        } else {
            field += c;
        }
    }

    fields.push_back(trim(field));
    return fields;
}

// Convert numeric strings, rejecting partial conversions.
static int parseInteger(const string& value) {
    size_t pos = 0;
    int number = stoi(value, &pos);

    if (pos != value.size()) {
        throw invalid_argument("Invalid ZIP: " + value);
    }

    return number;
}

static double parseDouble(const string& value) {
    size_t pos = 0;
    double number = stod(value, &pos);

    if (pos != value.size() || !isfinite(number)) {
        throw invalid_argument("Invalid coordinate: " + value);
    }

    return number;
}

/**
 * @brief Initializes an empty ZIP code record.
 */
Buffer::Buffer() {
    state = "";
    placeName = "";
    latitude = 0.0;
    longitude = 0.0;
    zipCode = 0;
}

void Buffer::setState(string state) {
    this->state = state;
}

void Buffer::setPlaceName(string placeName) {
    this->placeName = placeName;
}

void Buffer::setLatitude(double latitude) {
    this->latitude = latitude;
}

void Buffer::setLongitude(double longitude) {
    this->longitude = longitude;
}

void Buffer::setZipCode(int zipCode) {
    this->zipCode = zipCode;
}

string Buffer::getState() {
    return state;
}

string Buffer::getPlaceName() {
    return placeName;
}

double Buffer::getLatitude() {
    return latitude;
}

double Buffer::getLongitude() {
    return longitude;
}

int Buffer::getZipCode() {
    return zipCode;
}

/**
 * @brief Reads ZIP code records from a CSV file.
 * @param fileName CSV file path.
 * @return true if opened and read; false otherwise.
 *
 * @details Invalid or incomplete records are skipped.
 * Existing extrema are preserved across calls.
 */
bool Buffer::readCSV(string fileName) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cerr << "Error: Cannot open " << fileName << endl;
        return false;
    }

    string line;
    size_t lineNumber = 0;
    size_t processed = 0;
    size_t skipped = 0;

    // Skip CSV header.
    if (!getline(file, line)) {
        cerr << "Error: Empty CSV: " << fileName << endl;
        return false;
    }

    ++lineNumber;

    while (getline(file, line)) {
        ++lineNumber;

        vector<string> fields = splitCSV(line);

        // Expected columns:
        // ZIP, Place, State, County, Latitude, Longitude
        if (fields.size() < 6) {
            ++skipped;
            continue;
        }

        const string& zip = fields[0];
        const string& place = fields[1];
        const string& stateValue = fields[2];
        const string& lat = fields[4];
        const string& lon = fields[5];

        if (zip.empty() || stateValue.empty() ||
            lat.empty() || lon.empty()) {
            ++skipped;
            continue;
        }

        try {
            int parsedZip = parseInteger(zip);
            double parsedLat = parseDouble(lat);
            double parsedLon = parseDouble(lon);

            if (parsedZip < 0 || parsedZip > 99999 ||
                parsedLat < -90 || parsedLat > 90 ||
                parsedLon < -180 || parsedLon > 180) {
                ++skipped;
                continue;
            }

            setZipCode(parsedZip);
            setPlaceName(place);
            setState(stateValue);
            setLatitude(parsedLat);
            setLongitude(parsedLon);

            findExtrema();
            ++processed;

        } catch (const exception& e) {
            ++skipped;

            // Limit diagnostic output for large datasets.
            if (skipped <= 10) {
                cerr << "Skipping invalid record at line "
                     << lineNumber << " in " << fileName
                     << ": " << e.what() << endl;
            }
        }
    }

    if (file.bad()) {
        cerr << "Error reading file: " << fileName << endl;
        return false;
    }

    cout << "File: " << fileName << endl;
    cout << "Processed records: " << processed << endl;
    cout << "Skipped records: " << skipped << endl;

    if (processed == 0) {
        cerr << "Error: No valid records in "
             << fileName << endl;
        return false;
    }

    return true;
}

/**
 * @brief Finds easternmost, westernmost, northernmost,
 * and southernmost ZIP codes for each state.
 *
 * @details Coordinate ties select the smaller ZIP code.
 */
void Buffer::findExtrema() {

    if (extremes.count(state) == 0) {
        Extremes extreme;

        extreme.east = zipCode;
        extreme.west = zipCode;
        extreme.north = zipCode;
        extreme.south = zipCode;

        extreme.eastLongitude = longitude;
        extreme.westLongitude = longitude;
        extreme.northLatitude = latitude;
        extreme.southLatitude = latitude;

        extremes[state] = extreme;
        return;
    }

    Extremes& extreme = extremes[state];

    // Easternmost: highest longitude.
    if (longitude > extreme.eastLongitude ||
        (longitude == extreme.eastLongitude &&
         zipCode < extreme.east)) {
        extreme.east = zipCode;
        extreme.eastLongitude = longitude;
    }

    // Westernmost: lowest longitude.
    if (longitude < extreme.westLongitude ||
        (longitude == extreme.westLongitude &&
         zipCode < extreme.west)) {
        extreme.west = zipCode;
        extreme.westLongitude = longitude;
    }

    // Northernmost: highest latitude.
    if (latitude > extreme.northLatitude ||
        (latitude == extreme.northLatitude &&
         zipCode < extreme.north)) {
        extreme.north = zipCode;
        extreme.northLatitude = latitude;
    }

    // Southernmost: lowest latitude.
    if (latitude < extreme.southLatitude ||
        (latitude == extreme.southLatitude &&
         zipCode < extreme.south)) {
        extreme.south = zipCode;
        extreme.southLatitude = latitude;
    }
}

/**
 * @brief Returns the easternmost ZIP code.
 */
int Buffer::getEasternmostZipCode(string state) {
    return extremes.at(state).east;
}

/**
 * @brief Returns the westernmost ZIP code.
 */
int Buffer::getWesternmostZipCode(string state) {
    return extremes.at(state).west;
}

/**
 * @brief Returns the northernmost ZIP code.
 */
int Buffer::getNorthernmostZipCode(string state) {
    return extremes.at(state).north;
}

/**
 * @brief Returns the southernmost ZIP code.
 */
int Buffer::getSouthernmostZipCode(string state) {
    return extremes.at(state).south;
}

/**
 * @brief Displays geographic extrema sorted by state.
 */
void Buffer::displayTable() {

    cout << left << setfill(' ');
    cout << setw(7) << "State"
         << setw(14) << "Easternmost"
         << setw(14) << "Westernmost"
         << setw(14) << "Northernmost"
         << setw(14) << "Southernmost"
         << endl;

    cout << string(63, '-') << endl;

    for (const auto& entry : extremes) {
        const string& stateValue = entry.first;

        cout << left << setfill(' ')
             << setw(7) << stateValue;

        cout << right << setfill('0');

        cout << setw(5)
             << getEasternmostZipCode(stateValue)
             << "         ";

        cout << setw(5)
             << getWesternmostZipCode(stateValue)
             << "         ";

        cout << setw(5)
             << getNorthernmostZipCode(stateValue)
             << "         ";

        cout << setw(5)
             << getSouthernmostZipCode(stateValue)
             << endl;
    }

    cout << setfill(' ') << left;
}
