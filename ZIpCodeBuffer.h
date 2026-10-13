#ifndef ZIPCODEBUFFER_H
#define ZIPCODEBUFFER_H
#include <fstream>
#include <iostream>
#include <map>
#include <string>
using namespace std;

// Holds the four farthest Zip Codes for one state
struct Extremes{
    int east;
    int west;
    int north;
    int south;

    double eastLongitude;
    double westLongitude;
    double northLatitude;
    double southLatitude;
};

class Buffer{
public:

    Buffer();

    void setState(string state);

    void setPlaceName(string placeName);

    void setLatitude(double latitude);

    void setLongitude(double longitude);

    void setZipCode(int zipCode);

    string getState();

    string getPlaceName();

    double getLatitude();

    double getLongitude();

    int getZipCode();

    bool readCSV(string fileName);

    void findExtrema();

    int getEasternmostZipCode(string state);

    int getWesternmostZipCode(string state);

    int getNorthernmostZipCode(string state);

    int getSouthernmostZipCode(string state);

    void displayTable();

private:

    string state;
    string placeName;
    double latitude;
    double longitude;
    int zipCode;

    map<string, Extremes> extremes; // one entry per state, a map keeps them in alphabetical order

};

#endif
