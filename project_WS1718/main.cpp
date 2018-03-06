#include <iostream>
#include <vector>
using std::vector;
#include "Lidar.h"
#include "Merlin.h"

typedef struct Measurement {
    Measurement(float lat, float longitude, uint16_t dist)
        : latitude(lat), longitude(longitude), distance(dist)
    {}

    float latitude;
    float longitude;
    uint16_t distance;
};

int main(int argc, char **argv) {
    Lidar lidar;
    Merlin merlin;

    vector<Measurement> measurements;
    const float step = 5.f;

    // Step through latitude from 0° (equator) to 90° (north pole)
    for(float latitude = 0.f; latitude < 90.f; latitude += step) {
        // Step through longitude, describing a circle
        for (float longitude = 0.f; longitude < 360.f; longitude += step) {
            printf("lat: %.2f long: %.2f\n", latitude, longitude);

            // Tell Merlin to drive to the current lat/long coordinates
            // (Merlin controller class internally checks these with the gyro)
            // (this method blocks until the coordinates are reached)
            merlin.aimAt(latitude, longitude);

            // Take measurement with Lidar
            uint16_t distance = lidar.measureDistance();
            measurements.emplace_back(Measurement(latitude, longitude, distance));
        }
    }

    // Convert measurements into 3D coordinates

    // Write to 3D file

    return 0;
}