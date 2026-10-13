#ifndef ZIPCODEBUFFER_H
#define ZIPCODEBUFFER_H
import fstream;
#include <iostream>
using namespace std;

class Buffer{
    
    string state;
    string placeName;
    double latitude;
    double longitude;
    int zipCode;

}


default constructor(){
    state = "";
    placeName = "";
    latitude = 0.0;
    longitude = 0.0;
    zipCode = 0;
}

setState(string state){
    this->state = state;
}


setPlaceName(string placeName){
    this->placeName = placeName;
}


setLatitude(double latitude){
    this->latitude = latitude;
}

setLongitude(double longitude){
    this->longitude = longitude;
}

setZipCode(int zipCode){
    this->zipCode = zipCode;
}

getState(){
    return state;
}

getPlaceName(){
    return placeName;
}

getLatitude(){
    return latitude;
}

getLongitude(){
    return longitude;
}

getZipCode(){
    return zipCode;
}


readCSV(string fileName){
    ifstream file(fileName);
    string line;
    while(getline(file, line)){
        stringstream ss(line);
        string state, placeName, latitude, longitude, zipCode;
        getline(ss, state, ',');
        getline(ss, placeName, ',');
        getline(ss, latitude, ',');
        getline(ss, longitude, ',');
        getline(ss, zipCode, ',');
        
        setState(state);
        setPlaceName(placeName);
        setLatitude(stod(latitude));
        setLongitude(stod(longitude));
        setZipCode(stoi(zipCode));
    }
    file.close();
}

findExtrema(){
    // Logic to find the easternmost, westernmost, northernmost, and southernmost zip codes
}

getEasternmostZipCode(state);

getWesternmostZipCode(state);

getNorthernmostZipCode(state);

getSouthernmostZipCode(state);

displayTable();