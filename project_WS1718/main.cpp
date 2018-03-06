#include <iostream>
#include <vector>
using std::vector;
#include "Lidar.h"
#include "Merlin.h"

typedef struct Measurement {
    Measurement(float heading, float pitch, unsigned int dist)
        : heading(heading), pitch(pitch), distance(dist)
    {}

    float heading;
    float pitch;
    unsigned int distance;
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
            printf("heading: %.2f pitch: %.2f\n", heading, pitch);

            // Tell Merlin to drive to the current lat/long coordinates
            // (Merlin controller class internally checks these with the gyro)
            // (this method blocks until the coordinates are reached)
            merlin.aimAt(heading, pitch);

            // Take measurement with Lidar
            unsigned int distance = lidar.measureDistance();
            cout << "distance: " << distance << " cm" << endl;
            measurements.push_back(Measurement(heading, pitch, distance));
        }
    }

    // Convert measurements into 3D coordinates

    // Write to file

    cout << "Done." << endl;
    return 0;
}