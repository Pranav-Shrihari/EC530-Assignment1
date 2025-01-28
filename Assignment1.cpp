#include <iostream>
#include <cmath>
#include <limits>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <regex>

using namespace std;

const double R = 6371.0; //Radius of the earth in kilometers

struct GeoLocation {
    double latitude;
    double longitude;

    GeoLocation(double lat, double lon) : latitude(lat), longitude(lon) {}
};

// Overload the << operator for GeoLocation
ostream& operator<<(ostream& os, const GeoLocation& location) {
    os << "Latitude: " << location.latitude << ", Longitude: " << location.longitude;
    return os;
}

//Function to extract the coordinates from the file
vector<GeoLocation> extractGeoLocations(const string& filename, int latCol, int lonCol) {
    vector<GeoLocation> locations;
    ifstream file(filename);

    if (!file) {
        cerr << "Error: Unable to open file " << filename << endl;
        return locations;
    }

    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        string column;
        vector<string> columns;

        // Split the line into columns
        while (getline(iss, column, ',')) {
            columns.push_back(column);
        }

        try {
            double latitude = stod(columns[latCol]);
            double longitude = stod(columns[lonCol]);
            cout << latitude << " " << longitude << endl;
            if (latitude <= 90.0 && latitude >= -90.0 && longitude <= 180.0 && longitude >= -180.0) {
                locations.push_back({latitude, longitude});
            }
            else {
                cout << "Invalid latitude and longitude format" << endl;
            }
        } catch (const invalid_argument& e) {
            
        } catch (const out_of_range& e) {
            
        }
    }

    file.close();
    return locations;
}

//Haversine function to output closest point from second array to each point in first array (only for .csv files)
void Haversine(vector<GeoLocation> arr1, vector<GeoLocation> arr2) {
    //Initialize mindist to be very large
    double mindist = 999999999999.9;
    int index;

    //Loop through all values in the first array
    for (int i = 0; i < arr1.size(); i++) {
        for (int j = 0; j < arr2.size(); j++) {
            double dist = calcDist(arr1[i],arr2[j]);
            if (dist < mindist) {
                mindist = dist;
                index = j;
            }
        }
        //Print minimum distance found in array 2
        cout << "Point " << arr1[i] << " is closest to " << arr2[index] << endl;
    }
}

double calcDist(const GeoLocation& loc1, const GeoLocation& loc2) {
    // Convert degrees to radians
    double lat1 = loc1.latitude * M_PI / 180.0;
    double lon1 = loc1.longitude * M_PI / 180.0;
    double lat2 = loc2.latitude * M_PI / 180.0;
    double lon2 = loc2.longitude * M_PI / 180.0;

    // Differences in coordinates
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    // Haversine formula
    double a = sin(dlat / 2) * sin(dlat / 2) +
               cos(lat1) * cos(lat2) *
               sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    // Distance in kilometers
    return R * c;
}


//Haversine function to output closest point from second array to each point in first array (only for manually inputted arrays)
void ManualHaversine(int size1, string arr1[], int size2, string arr2[]) {

    //Initialize mindist to be very large
    double mindist = 999999999999.9;
    float minlat, minlong;

    //Loop through all values in the first array
    for (int i = 0; i < size1; i++) {
        //Split each entry in the first array into latitude and longtitude
        float lat1, long1;

        // Create a stringstream to split the input string
        string input = arr1[i];
        stringstream ss(input);
        string token;

        // Get the first value (latitude)
        getline(ss, token, ',');  // Split at comma
        lat1 = stof(token);  // Convert string to float

        // Get the second value (longitude)
        getline(ss, token, ',');  // Split at comma
        long1 = stof(token);  // Convert string to float

        //Convert latitude and longtitude from degrees to radians
        double rad_lat1 = lat1 * M_PI / 180.0;
        double rad_long1 = long1 * M_PI / 180.0;

        //For each value in the first array, loop through all values in the second array
        for (int j = 0; j < size2; j++) {
            //Split each entry in the second array into latitude and longtitude
            float lat2, long2;

            // Create a stringstream to split the input string
            string input2 = arr2[j];
            stringstream ss(input2);
            string token;

            // Get the first value (latitude)
            getline(ss, token, ',');  // Split at comma
            lat2 = stof(token);  // Convert string to float

            // Get the second value (longitude)
            getline(ss, token, ',');  // Split at comma
            long2 = stof(token);  // Convert string to float

            //Convert latitude and longtitude from degrees to radians
            double rad_lat2 = lat2 * M_PI / 180.0;
            double rad_long2 = long2 * M_PI / 180.0;

            //Use Haversine formula to find distance between two points (used https://www.geeksforgeeks.org/program-distance-two-points-earth/)
            long double dlat = rad_lat2 - rad_lat1;
            long double dlong = rad_long2 - rad_long1;

            double a = sin(dlat / 2) * sin(dlat / 2) + cos(rad_lat1) * cos(rad_lat2) * sin(dlong / 2) * sin(dlong / 2);
            double c = 2 * atan2(sqrt(a),sqrt(1-a));
            double dist = R * c;

            cout << dist << endl;

            //Else, compare distance with minimum distance found so far
            if (dist < mindist) {
                //Append new minimum distance
                mindist = dist;
                //Add new values of latitude and longtitude
                minlat = lat2;
                minlong = long2;
            }
        }
        //Print minimum distance found in array 2
        cout << "Point " << arr1[i] << " is closest to " << minlat << "," << minlong << endl;
    }
}

int main() {
    
    int choice;

    //User enters if wants to use manual inputs or inputs from .csv file
    cout << "Please enter '1' if you want to enter the arrays manually, or '2' if you want to use .csv files: " << endl;
    cin >> choice;

    if (choice == 1) {
        
        int size1, size2;

        //User enters size of the first array
        cout << "Enter the size of the first array: " << endl;
        cin >> size1;
        cout << endl;

        string arr1[size1];
    
        //User enters each entry in the first array
        for (int i = 0; i < size1; i++) {
            cout << "Enter location number " << i + 1 << " in the format 'latitude,longtitude', with latitude and longtitude in degrees (e.g. New York = 40.7128,74.0060): " << endl;
            cin >> arr1[i]; 
        }
        cout << endl;

        //User enters size of the second array
        cout << "Enter the size of the second array: " << endl;
        cin >> size2;
        cout << endl;

        string arr2[size2];

        //User enters each entry in the second array
        for (int j = 0; j < size2; j++) {
            cout << "Enter location number " << j + 1 << " in the format 'latitude,longtitude', with latitude and longtitude in degrees (e.g. New York = 40.7128,74.0060): " << endl;
            cin >> arr2[j];
        }
        cout << endl;

        //Call function to match each point in first array to closest point in second array
        ManualHaversine(size1,arr1,size2,arr2);
    }

    else if (choice == 2) {
        //Array 1
        string filename1;
        int latcol1, lngcol1;
        cout << "Enter the filename of the first .csv: " << endl;
        cin >> filename1;
        cout << "Also please enter the column that the latitudes are found in: " << endl;
        cin >> latcol1;
        cout << "Also please enter the column that the longitudes are found in: " << endl;
        cin >> lngcol1;
        vector<GeoLocation> coordinates1 = extractGeoLocations(filename1,latcol1,lngcol1);

        //Array 2
        string filename2;
        int latcol2, lngcol2;
        cout << "Enter the filename of the first .csv: " << endl;
        cin >> filename2;
        cout << "Also please enter the column that the latitudes are found in: " << endl;
        cin >> latcol2;
        cout << "Also please enter the column that the longitudes are found in: " << endl;
        cin >> lngcol2;
        vector<GeoLocation> coordinates2 = extractGeoLocations(filename2,latcol2,lngcol2);

        //Call function
        Haversine(coordinates1,coordinates2);
    }
}
