/** @file LengthIndicatedBuffer.cpp
 * @brief Implements the length indicated buffer.
 */
#include "LengthIndicatedBuffer.h"
#include <iomanip>
#include <sstream>


LengthIndicatedBuffer::LengthIndicatedBuffer(){
    record = "";
}

bool LengthIndicatedBuffer::read(istream& file){
    // The first 4 characters are the length
    char lengthText[5] = "";
    if(!file.read(lengthText, 4)){
        return false;
    }

    int length = atoi(lengthText);
    if(length <= 0){
        cout << "Error: bad record length " << lengthText << endl;
        return false;
    }

    // Read that many characters for the record
    record = string(length, ' ');
    if(!file.read(&record[0], length)){
        cout << "Error: record is cut off" << endl;
        return false;
    }

    // skip the newline at the end (\r\n on Windows)
    if(file.peek() == '\r'){
        file.get();
    }
    if(file.peek() == '\n'){
        file.get();
    }

    return true;
}

bool LengthIndicatedBuffer::unpack(Buffer& buffer, vector<string> fieldNames){
    vector<string> fields = getFields();

    string zip;
    string place;
    string state;
    string lat;
    string lon;

    // Use the field names so it still works if the columns are in a different order
    for(int i = 0; i < fields.size() && i < fieldNames.size(); i++){
        if(fieldNames[i] == "Zip Code"){
            zip = fields[i];
        }
        else if(fieldNames[i] == "Place Name"){
            place = fields[i];
        }
        else if(fieldNames[i] == "State"){
            state = fields[i];
        }
        else if(fieldNames[i] == "Lat"){
            lat = fields[i];
        }
        else if(fieldNames[i] == "Long"){
            lon = fields[i];
        }
    }

    // skip any record that is missing data
    if(zip == "" || state == "" || lat == "" || lon == ""){
        return false;
    }

    buffer.setZipCode(stoi(zip));
    buffer.setPlaceName(place);
    buffer.setState(state);
    buffer.setLatitude(stod(lat));
    buffer.setLongitude(stod(lon));

    return true;
}

void LengthIndicatedBuffer::pack(string line){
    // take off the \r so it isn't counted in the length
    if(line != "" && line.back() == '\r'){
        line.pop_back();
    }
    record = line;
}

void LengthIndicatedBuffer::write(ostream& file){
    // setw and setfill('0') make the length 4 digits, like 0042
    file << setw(4) << setfill('0') << record.length() << record << endl;
}

string LengthIndicatedBuffer::getRecord(){
    return record;
}

vector<string> LengthIndicatedBuffer::getFields(){
    vector<string> fields;
    stringstream ss(record);
    string field;

    while(getline(ss, field, ',')){
        fields.push_back(field);
    }

    return fields;
}
