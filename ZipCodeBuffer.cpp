#include "ZipCodeBuffer.h"
#include <iomanip>
#include <sstream>


Buffer::Buffer(){
    state = "";
    placeName = "";
    latitude = 0.0;
    longitude = 0.0;
    zipCode = 0;
}

void Buffer::setState(string state){
    this->state = state;
}

void Buffer::setPlaceName(string placeName){
    this->placeName = placeName;
}

void Buffer::setLatitude(double latitude){
    this->latitude = latitude;
}

void Buffer::setLongitude(double longitude){
    this->longitude = longitude;
}

void Buffer::setZipCode(int zipCode){
    this->zipCode = zipCode;
}

string Buffer::getState(){
    return state;
}

string Buffer::getPlaceName(){
    return placeName;
}

double Buffer::getLatitude(){
    return latitude;
}

double Buffer::getLongitude(){
    return longitude;
}

int Buffer::getZipCode(){
    return zipCode;
}


// Reads the CSV file one line at a time.
// Each line is split into its fields, stored in the buffer,
// and then checked against the extremes for its state.
bool Buffer::readCSV(string fileName){
    ifstream file(fileName);
    if(!file.is_open()){
        return false;
    }

    string line;

    // Skip the header line
    getline(file, line);

    while(getline(file, line)){
        stringstream ss(line);

        // The columns in the file are: Zip Code, Place Name, State, County, Lat, Long
        string zip;
        string place;
        string state;
        string county;
        string lat;
        string lon;

        getline(ss, zip, ',');
        getline(ss, place, ',');
        getline(ss, state, ',');
        getline(ss, county, ',');
        getline(ss, lat, ',');
        getline(ss, lon, ',');

        // skip any line that is missing data
        if(zip == "" || state == "" || lat == "" || lon == ""){
            continue;
        }

        // stoi turns a string into an int, stod turns a string into a double
        setZipCode(stoi(zip));
        setPlaceName(place);
        setState(state);
        setLatitude(stod(lat));
        setLongitude(stod(lon));

        findExtrema();
    }

    file.close();
    return true;
}

// Checks the record in the buffer against the farthest Zip Codes
// found so far for its state, and replaces any that it beats.
// If two Zip Codes are tied, the smaller Zip Code is kept so the
// answer is the same no matter what order the file is in.
void Buffer::findExtrema(){

    // First time seeing this state, so this Zip Code is the farthest in every direction
    if(extremes.count(state) == 0){
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

    // Get the current extremes for this state
    Extremes extreme = extremes[state];

    // Longitudes in the CSV are negative for the US (New York is about -72,
    // California is about -122), so the farther east you go, the bigger the number.
    // Easternmost has the biggest longitude
    if(longitude > extreme.eastLongitude){
        extreme.east = zipCode;
        extreme.eastLongitude = longitude;
    }
    else if(longitude == extreme.eastLongitude && zipCode < extreme.east){
        extreme.east = zipCode;
    }

    // Westernmost has the smallest longitude
    if(longitude < extreme.westLongitude){
        extreme.west = zipCode;
        extreme.westLongitude = longitude;
    }
    else if(longitude == extreme.westLongitude && zipCode < extreme.west){
        extreme.west = zipCode;
    }

    // Northernmost has the biggest latitude
    if(latitude > extreme.northLatitude){
        extreme.north = zipCode;
        extreme.northLatitude = latitude;
    }
    else if(latitude == extreme.northLatitude && zipCode < extreme.north){
        extreme.north = zipCode;
    }

    // Southernmost has the smallest latitude
    if(latitude < extreme.southLatitude){
        extreme.south = zipCode;
        extreme.southLatitude = latitude;
    }
    else if(latitude == extreme.southLatitude && zipCode < extreme.south){
        extreme.south = zipCode;
    }

    // Save the updated extremes back into the map
    extremes[state] = extreme;
}

int Buffer::getEasternmostZipCode(string state){
    return extremes[state].east;
}

int Buffer::getWesternmostZipCode(string state){
    return extremes[state].west;
}

int Buffer::getNorthernmostZipCode(string state){
    return extremes[state].north;
}

int Buffer::getSouthernmostZipCode(string state){
    return extremes[state].south;
}

// Prints a header row and then one row for each state.
// The map already keeps the states in alphabetical order.
void Buffer::displayTable(){
    cout << left;
    cout << setw(7) << "State";
    cout << setw(14) << "Easternmost";
    cout << setw(14) << "Westernmost";
    cout << setw(14) << "Northernmost";
    cout << setw(14) << "Southernmost" << endl;
    cout << "---------------------------------------------------------------" << endl;

    // Go through every state in the map
    for(pair<string, Extremes> entry : extremes){
        string state = entry.first;

        cout << left << setfill(' ') << setw(7) << state;

        // setw(5) and setfill('0') print Zip Codes like 501 as 00501
        cout << right << setfill('0');
        cout << setw(5) << getEasternmostZipCode(state) << "         ";
        cout << setw(5) << getWesternmostZipCode(state) << "         ";
        cout << setw(5) << getNorthernmostZipCode(state) << "         ";
        cout << setw(5) << getSouthernmostZipCode(state) << endl;
    }

    cout << setfill(' ');
}
