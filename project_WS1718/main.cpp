#include <iostream>
#include <vector>
using std::vector;
#include "Lidar.h"
#include "Merlin.h"

typedef struct Measurement {
    Measurement(float heading, float pitch, uint16_t dist)
        : heading(heading), pitch(pitch), distance(dist)
    {}

    float heading;
    float pitch;
    uint16_t distance;
};

int main(int argc, char **argv) {
    Lidar lidar;
    Merlin merlin;

    vector<Measurement> measurements;
    const float step = 5.f;

    // Step through pitch from 0° (equator) to 90° (north pole)
    for(float pitch = 0.f; pitch < 90.f; pitch += step) {
        // Step through heading, describing a circle
        for (float heading = 0.f; heading < 360.f; heading += step) {
            printf("lat: %.2f long: %.2f\n", pitch, heading);

            // Tell Merlin to drive to the current lat/long coordinates
            // (Merlin controller class internally checks these with the gyro)
            // (this method blocks until the coordinates are reached)
            merlin.aimAt(heading, pitch);

            // Take measurement with Lidar
            uint16_t distance = lidar.measureDistance();
            measurements.emplace_back(Measurement(heading, pitch, distance));
        }
    }

    // Convert measurements into 3D coordinates

    // Write to file

    return 0;
}