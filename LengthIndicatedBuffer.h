/** @file LengthIndicatedBuffer.h
 * @brief Buffer for length indicated records.
 */
#ifndef LENGTHINDICATEDBUFFER_H
#define LENGTHINDICATEDBUFFER_H
#include "ZipCodeBuffer.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/** @brief Reads and writes one length indicated record at a time. */
class LengthIndicatedBuffer{
public:

    /** @brief Constructor. */
    LengthIndicatedBuffer();

    /** @brief Reads the next record.
     * @param file Input file.
     * @return false at end of file or bad record.
     */
    bool read(istream& file);

    /** @brief Unpacks the record into a Buffer.
     * @param buffer Buffer to fill.
     * @param fieldNames Field names in order.
     * @return false if the record is missing data.
     */
    bool unpack(Buffer& buffer, vector<string> fieldNames);

    /** @brief Stores a record.
     * @param line Comma separated fields.
     */
    void pack(string line);

    /** @brief Writes the record with its length.
     * @param file Output file.
     */
    void write(ostream& file);

    /** @brief Returns the record. */
    string getRecord();

    /** @brief Returns the record's fields. */
    vector<string> getFields();

private:
    string record; // Current record.
};

#endif
