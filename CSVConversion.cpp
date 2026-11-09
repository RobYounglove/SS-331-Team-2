#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;


struct FileHeader {
    std::string structureType= "Length indicated"; // 20 bytes for the structure type
    int version= 1;       // 4 bytes for the version
    int headerSize= 0;    // 4 bytes for the header size
    int sizeFieldBytes= 4; // 4 bytes for the size of each field
    bool sizeincluded= false; // 1 byte for the size included flag

    std::string primaryKeyName= "Zip Code"; // 50 bytes for the primary key name

    int recordCount= 0;   // 4 bytes for the number of records
    int fieldCount= 0;    // 4 bytes for the number of fields

    vector<Fieldinfo> fields; // Vector to hold field information
};
struct Fieldinfo {
    std::string fieldName; // 50 bytes for the field name
    int fieldType;         // 4 bytes for the type of the field
};
void writeHeaderToFile(ofstream& output, const FileHeader& fileHeader) {
    output << "HEADER\n";
    output << "Structure Type: " << fileHeader.structureType << "\n";
    output << "Version: " << fileHeader.version << "\n";
    output << "Header Size: " << fileHeader.headerSize << "\n";
    output << "Size Field Bytes: " << fileHeader.sizeFieldBytes << "\n";
    output << "Size Included: " << (fileHeader.sizeincluded ? "true" : "false") << "\n";
    output << "Primary Key Name: " << fileHeader.primaryKeyName << "\n";
    output << "Record Count: " << fileHeader.recordCount << "\n";
    output << "Field Count: " << fileHeader.fieldCount << "\n";
    output << "END HEADER\n";
}

void analyzeCSV(const string& csvFilePath, int& recordCount, int& fieldCount) {
    ifstream csv(csvFilePath);
    if (!csv.is_open()) {
        cerr << "Error opening input file: " << csvFilePath << endl;
        return;
    }

    string line;
    recordCount = 0;
    fieldCount = 0; 

    while (getline(csv, line)) {
        recordCount++;
        if (recordCount == 1) {
            // Count the number of fields in the first line
            size_t pos = 0;
            while ((pos = line.find(',', pos)) != string::npos) {
                fieldCount++;
                pos++; // Move past the comma
            }
            fieldCount++; // Add one for the last field
        }
    }

    csv.close();
}
string convertCSVTodat(const string& csvFilePath, const string& outputFilePath) {
    FileHeader header;

    analyzeCSV(csvFilePath, header.recordCount, header.fieldCount);

    ifstream csv(csvFilePath);
    ofstream output(outputFilePath);

    if (!csv.is_open()) {
        cerr << "Error opening input file: " << csvFilePath << endl;
        return "Error opening input file.";
    }

    if (!output.is_open()) {
        cerr << "failed to create DAT file: " << outputFilePath << endl;
        return "failed to create DAT file.";
    }

    writeHeaderToFile(output, header);
    string line;
    
    while (getline(csv, line)) {
        int recordSize = line.length();
        
        output << setw(header.sizeFieldBytes) << setfill('0') << recordSize << line << endl; // Write the line to the output file
    }

    csv.close();
    output.close();

    return "Conversion completed successfully.";
}

int main() {
    string csvFilePath1 = "us_postal_codes.csv"; // Path to the input CSV file
    string outputFilePath1 = "output.dat"; // Path to the output DAT file
    string csvFilePath2 = "us_postal_codes_ROWS_RANDOMIZED.csv"; // Path to the second input CSV file
    string outputFilePath2 = "output2.dat"; // Path to the second output DAT file

    string result1 = convertCSVTodat(csvFilePath1, outputFilePath1);
    cout << result1 << endl;

    string result2 = convertCSVTodat(csvFilePath2, outputFilePath2);
    cout << result2 << endl;

    return 0;
}