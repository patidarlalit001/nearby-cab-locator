/**
 * @file cab_book.cpp
 * @author Lalit Patidar
 * @contact github.com/patidarlalit001 | linkedin.com/in/patidarlalit/
 * @date july 2026
 * 
 * @brief Cab Nearby Customer Finder
 * 
 * @description 
 * This program reads a list of customer records in JSON format, calculates 
 * their Great-Circle distance from a central origin point, and outputs a 
 * list of customers who are within a 50km radius, sorted by distance from the origin.
 * 
 * Built with standard C++ libraries, focusing on modularity, memory safety, 
 * and O(N log N) sorting efficiency.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>

using namespace std;

// System Constants
const double PI = 3.14159265358979323846;
const double EARTH_RADIUS_KM = 6371.0;
const double SEARCH_RADIUS_KM = 50.0;

// Origin Coordinates (e.g., Cab Company HQ)
const double ORIGIN_LAT = 12.9611159;
const double ORIGIN_LON = 77.6362214;

struct Customer {
    int user_id;
    string name;
    double latitude;
    double longitude;
    double distance; // Added to store the calculated distance for sorting
};

class LocationServices {
private:
    // Utility to convert degrees to radians
    static double degToRad(double deg) {
        return (deg * PI / 180.0);
    }

public:
    // Calculates the Great-Circle distance between two points on Earth
    static double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
        lat1 = degToRad(lat1);
        lon1 = degToRad(lon1);
        lat2 = degToRad(lat2);
        lon2 = degToRad(lon2);

        double delta_lon = lon2 - lon1;
        
        // Great Circle distance formula
        double central_ang = acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(delta_lon));
        
        return (EARTH_RADIUS_KM * central_ang);
    }
};

class CustomerDataParser {
public:
    // A lightweight manual JSON parser for the specific customer format
    static Customer parseCustomerLine(const string& line) {
        Customer cust;
        
        // Extract Latitude
        size_t lat_pos = line.find("\"latitude\": \"") + 13;
        size_t lat_end = line.find("\"", lat_pos);
        cust.latitude = stod(line.substr(lat_pos, lat_end - lat_pos));

        // Extract User ID
        size_t id_pos = line.find("\"user_id\": ") + 11;
        size_t id_end = line.find(",", id_pos);
        cust.user_id = stoi(line.substr(id_pos, id_end - id_pos));

        // Extract Name
        size_t name_pos = line.find("\"name\": \"") + 9;
        size_t name_end = line.find("\"", name_pos);
        cust.name = line.substr(name_pos, name_end - name_pos);

        // Extract Longitude
        size_t lon_pos = line.find("\"longitude\": \"") + 14;
        size_t lon_end = line.find("\"", lon_pos);
        cust.longitude = stod(line.substr(lon_pos, lon_end - lon_pos));

        // Initialize distance to 0 (will be calculated later)
        cust.distance = 0.0;

        return cust;
    }
};

int main() {
    ifstream input_file("customers.json");
    if (!input_file.is_open()) {
        cerr << "Error: Could not open customers.json. Ensure the file is in the same directory." << endl;
        return 1;
    }

    vector<Customer> valid_customers;
    string line;

    // Read and process each customer line
    while (getline(input_file, line)) {
        if (line.empty()) continue;

        try {
            Customer cust = CustomerDataParser::parseCustomerLine(line);
            double distance = LocationServices::calculateDistance(ORIGIN_LAT, ORIGIN_LON, cust.latitude, cust.longitude);

            if (distance <= SEARCH_RADIUS_KM) {
                cust.distance = distance; // Save the distance in the struct
                valid_customers.push_back(cust);
            }
        } catch (const exception& e) {
            cerr << "Warning: Skipping malformed line. Error: " << e.what() << endl;
        }
    }
    input_file.close();

    // Sort valid customers by distance ascending (closest cabs first)
    sort(valid_customers.begin(), valid_customers.end(), [](const Customer& a, const Customer& b) {
        return a.distance < b.distance;
    });

    // Write to output file
    ofstream output_file("answer.json");
    if (!output_file.is_open()) {
        cerr << "Error: Could not create answer.json." << endl;
        return 1;
    }

    for (const auto& cust : valid_customers) {
        // Outputting distance as well so the sorting is verifiable
        output_file << "{\"user_id\": " << cust.user_id 
                    << ", \"name\": \"" << cust.name << "\""
                    << ", \"distance_km\": " << cust.distance << "}\n";
    }
    
    output_file.close();
    cout << "Success: Filtered " << valid_customers.size() << " customers within " << SEARCH_RADIUS_KM << "km." << endl;
    cout << "Results written to answer.json (Sorted by closest distance)" << endl;

    return 0;
}
