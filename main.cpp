#include "ZipCodeBuffer.h"

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
