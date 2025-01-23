#include <iostream>
#include <cmath>
#include <limits>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

const double R = 6371.0; //Radius of the earth in kilometers

//Haversine function to output closest point from second array to each point in first array
void Haversine(int size1, string arr1[], int size2, string arr2[]) {

    //Initialize mindist to be very large
    double mindist = 999999999999.9;
    float minlat, minlong;

    //Loop through all values in the first array
    for (int i = 0; i < size1; i++) {
        //Split each entry in the first array into latitude and longitude
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

        //Convert latitude and longitude from degrees to radians
        double rad_lat1 = lat1 * M_PI / 180.0;
        double rad_long1 = long1 * M_PI / 180.0;

        //For each value in the first array, loop through all values in the second array
        for (int j = 0; j < size2; j++) {
            //Split each entry in the second array into latitude and longitude
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

            //Convert latitude and longitude from degrees to radians
            double rad_lat2 = lat2 * M_PI / 180.0;
            double rad_long2 = long2 * M_PI / 180.0;

            //Use Haversine formula to find distance between two points (used https://www.geeksforgeeks.org/program-distance-two-points-earth/)
            long double dlat = rad_lat2 - rad_lat1;
            long double dlong = rad_long2 - rad_long1;

            double a = sin(dlat / 2) * sin(dlat / 2) + cos(rad_lat1) * cos(rad_lat2) * sin(dlong / 2) * sin(dlong / 2);
            double c = 2 * atan2(sqrt(a),sqrt(1-a));
            double dist = R * c;

            //If distance = 0, break loop and output
            if (dist == 0) {
                minlat = lat2;
                minlong = long2;
                break;
            }
            //Else, compare distance with minimum distance found so far
            else if (dist < mindist) {
                //Append new minimum distance
                mindist = dist;
                //Add new values of latitude and longitude
                minlat = lat2;
                minlong = long2;
            }
        }
        //Print minimum distance found in array 2
        cout << "Point " << arr1[i] << " is closest to " << minlat << "," << minlong << endl;
    }
}

int main() {

    int size1, size2;

    //User enters size of the first array
    cout << "Enter the size of the first array: " << endl;
    cin >> size1;
    cout << endl;

    string arr1[size1];
    
    //User enters each entry in the first array
    for (int i = 0; i < size1; i++) {
        cout << "Enter location number " << i + 1 << " in the format 'latitude,longitude', with latitude and longitude in degrees (e.g. New York = 40.7128,74.0060): " << endl;
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
        cout << "Enter location number " << j + 1 << " in the format 'latitude,longitude', with latitude and longitude in degrees (e.g. New York = 40.7128,74.0060): " << endl;
        cin >> arr2[j];
    }
    cout << endl;

    //Call function to match each point in first array to closest point in second array
    Haversine(size1,arr1,size2,arr2);
}
