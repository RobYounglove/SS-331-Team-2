
/**
 * @file main.cpp
 * @brief Application entry point for the ZIP Code group project.
 *
 * Reads ZIP code data from two CSV files and displays
 * the results using the Buffer class.
 */

#include <iostream>
#include <string>
#include "ZipCodeBuffer.h"

using namespace std;

/**
 * @brief Runs the ZIP code table application.
 *
 * @details Reads us_postal_codes.csv and
 * us_postal_codes_ROWS_RANDOMIZED.csv from the
 * working directory, processes ZIP code records,
 * and displays the resulting table.
 *
 * @return 0 on success; 1 if a CSV file cannot be read.
 */
int main() {

    // CSV file names
    string fileName = "us_postal_codes.csv";
    string randomFileName =
        "us_postal_codes_ROWS_RANDOMIZED.csv";

    // Create the ZIP code buffer
    Buffer buffer;

    // Read the original CSV file
    if (!buffer.readCSV(fileName)) {
        cerr << "Error: could not open or read "
             << fileName << endl;
        return 1;
    }

    // Read the randomized CSV file
    if (!buffer.readCSV(randomFileName)) {
        cerr << "Error: could not open or read "
             << randomFileName << endl;
        return 1;
    }

    // Display the ZIP code table
    cout << "\nZIP Code Results\n";
    cout << "-----------------------------\n";

    buffer.displayTable();

    cout << "\nProgram completed successfully."
         << endl;

    return 0;
}
