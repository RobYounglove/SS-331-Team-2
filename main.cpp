/** @file main.cpp
 * @brief Application entry point for the ZIP code group project.
 */
#include "ZipCodeBuffer.h"

/** @brief Runs the ZIP code table application.
 * @return 0 on success; 1 if the CSV cannot be opened.
 * @details Reads us_postal_codes.csv from the working directory and prints
 * one row per state. Numeric conversion exceptions are not caught here.
 */
int main(){
    string fileName = "us_postal_codes.csv";

    Buffer buffer;

    // Read the file and find the farthest Zip Codes for each state
    if(!buffer.readCSV(fileName)){
        cout << "Error: could not open " << fileName << endl;
        return 1;
    }

    // Print the table
    buffer.displayTable();

    return 0;
}
