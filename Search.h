/** @file Search.h
 * @brief Retrieves indexed CSV records into the existing Buffer class.
 */
#ifndef SEARCH_H
#define SEARCH_H

#include "Index.h"
#include "ZipCodeBuffer.h"
#include <fstream>
#include <string>

class Search {
public:
    /** @brief Opens and indexes a CSV in the existing six-column format.
     * @details Supports both ordered and randomized CSV files. The file must
     * remain unchanged until the next open(). Does not read DAT files.
     * @return false if the file cannot be opened or indexed.
     */
    bool open(const std::string& fileName);

    /** @brief Loads a ZIP record into buffer; leading zeros are optional.
     * @return false if unopened, absent, unreadable, or malformed.
     * @details Leaves buffer unchanged on failure. Uses the existing comma
     * splitting convention, which does not support quoted commas.
     */
    bool findZipCode(int zipCode, Buffer& buffer);

private:
    Index index;
    std::ifstream file;
};

#endif
