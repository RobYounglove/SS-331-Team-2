/** @file Index.h
 * @brief Maps ZIP codes to record positions in a CSV file.
 */
#ifndef INDEX_H
#define INDEX_H

#include <ios>
#include <map>
#include <string>

class Index {
public:
    /** @brief Builds an index for a CSV with ZIP code in its first column.
     * @return false on an open or read error; the index is then empty.
     * @details Skips the header and invalid ZIP fields. For duplicate ZIP
     * codes, keeps the first record. Rebuild after changing the CSV file.
     */
    bool build(const std::string& fileName);

    /** @brief Finds a ZIP's byte offset without changing position on failure. */
    bool find(int zipCode, std::streamoff& position) const;

private:
    std::map<int, std::streamoff> entries;
};

#endif
