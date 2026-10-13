/** @file ZipCodeBuffer.h
 * @brief Record buffer and per-state geographic extrema declarations.
 */
#ifndef ZIPCODEBUFFER_H
#define ZIPCODEBUFFER_H
#include <fstream>
#include <iostream>
#include <map>
#include <string>
using namespace std;

/** @brief ZIP codes and comparison coordinates for one state. */
struct Extremes{
    int east; ///< Stored east value.
    int west; ///< Stored west value.
    int north; ///< Stored north value.
    int south; ///< Stored south value.

    double eastLongitude; ///< Stored eastLongitude value.
    double westLongitude; ///< Stored westLongitude value.
    double northLatitude; ///< Stored northLatitude value.
    double southLatitude; ///< Stored southLatitude value.
};

/** @brief Reads records sequentially and accumulates geographic extrema.
 * @details Stores the current record and a state-keyed map. Equal coordinates
 * select the smaller ZIP code, making results independent of record order.
 */
class Buffer{
public:

    /** @brief Initializes an empty record and empty extrema map. */
    Buffer();

    /** @brief Sets the current record State.
     * @param state Value to store in the record buffer.
     */
    void setState(string state);

    /** @brief Sets the current record PlaceName.
     * @param placeName Value to store in the record buffer.
     */
    void setPlaceName(string placeName);

    /** @brief Sets the current record Latitude.
     * @param latitude Value to store in the record buffer.
     */
    void setLatitude(double latitude);

    /** @brief Sets the current record Longitude.
     * @param longitude Value to store in the record buffer.
     */
    void setLongitude(double longitude);

    /** @brief Sets the current record ZipCode.
     * @param zipCode Value to store in the record buffer.
     */
    void setZipCode(int zipCode);

    /** @brief Returns State.
     * @return Stored value.
     */
    string getState();

    /** @brief Returns PlaceName.
     * @return Stored value.
     */
    string getPlaceName();

    /** @brief Returns Latitude.
     * @return Stored value.
     */
    double getLatitude();

    /** @brief Returns Longitude.
     * @return Stored value.
     */
    double getLongitude();

    /** @brief Returns ZipCode.
     * @return Stored value.
     */
    int getZipCode();

    /** @brief Reads the CSV and accumulates one record at a time.
     * @param fileName Path to a CSV with a header and columns ZIP, place, state, county, latitude, longitude.
     * @return false if opening fails; true after processing the file.
     * @details Skips records with empty ZIP, state, latitude, or longitude.
     * Existing extrema remain when called again. Comma splitting does not handle quoted commas.
     * @throws std::invalid_argument If a numeric field cannot be converted.
     * @throws std::out_of_range If a numeric field is outside the conversion range.
     */
    bool readCSV(string fileName);

    /** @brief Updates state extrema using the current buffered record.
     * @details East uses maximum longitude, west minimum longitude, north
     * maximum latitude, and south minimum latitude. Ties use the smaller ZIP.
     */
    void findExtrema();

    /** @brief Returns EasternmostZipCode.
     * @param state State abbreviation already present in the extrema map.
     * @pre The state has been accumulated by findExtrema().
     * @return ZIP code for the requested direction.
     * @note An unknown state inserts a zero-initialized map entry.
     */
    int getEasternmostZipCode(string state);

    /** @brief Returns WesternmostZipCode.
     * @param state State abbreviation already present in the extrema map.
     * @pre The state has been accumulated by findExtrema().
     * @return ZIP code for the requested direction.
     * @note An unknown state inserts a zero-initialized map entry.
     */
    int getWesternmostZipCode(string state);

    /** @brief Returns NorthernmostZipCode.
     * @param state State abbreviation already present in the extrema map.
     * @pre The state has been accumulated by findExtrema().
     * @return ZIP code for the requested direction.
     * @note An unknown state inserts a zero-initialized map entry.
     */
    int getNorthernmostZipCode(string state);

    /** @brief Returns SouthernmostZipCode.
     * @param state State abbreviation already present in the extrema map.
     * @pre The state has been accumulated by findExtrema().
     * @return ZIP code for the requested direction.
     * @note An unknown state inserts a zero-initialized map entry.
     */
    int getSouthernmostZipCode(string state);

    /** @brief Prints the state extrema table to standard output.
     * @details Rows follow alphabetical state order. Columns are state, east,
     * west, north, south. ZIP codes are padded to five digits.
     */
    void displayTable();

private:

    string state; ///< Current record state.
    string placeName; ///< Current record placeName.
    double latitude; ///< Current record latitude.
    double longitude; ///< Current record longitude.
    int zipCode; ///< Current record zipCode.

    map<string, Extremes> extremes; // one entry per state, a map keeps them in alphabetical order

};

#endif
