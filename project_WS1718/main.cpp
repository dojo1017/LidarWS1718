#include <iostream>
#include <vector>
using std::vector;
#include "Lidar.h"
#include "Merlin.h"
//#include "libs/merlin/MerlinHalfSqhere.h"
#include "Calculation.h"
#include <fstream>

struct Measurement {
    Measurement(float heading, float pitch, unsigned int dist)
        : heading(heading), pitch(pitch), distance(dist)
    {}

    float heading;
    float pitch;
    unsigned int distance;
};

void writeMeasurementsToFile(vector<Measurement_3D> measurements_3D)
{
    ofstream fileout;
    fileout.open("3dPoints.ply");
    fileout << "ply" << endl;
    fileout << "format ascii 1.0" << endl;
    fileout << "element vertex " << measurements_3D.size() << endl;
    fileout << "property float x" << endl;
    fileout << "property float y" << endl;
    fileout << "property float z" << endl;
    fileout << "end_header" << endl;

    for(const Measurement_3D &m : measurements_3D)
    {
        fileout << m.x << " " << m.y << " " << m.z << " " << endl;
    }

    fileout.close();
}



int main(int argc, char **argv) {

    Lidar lidar;
    Merlin merlin;
    Calculation calculation;

    vector<Measurement> measurements;
    vector<Measurement_3D> measurements_3D;
    const float step = 5.f;

//    merlin.aimAt(0,0); //an Start
//
//    // Step through pitch from 0° (equator) to 90° (north pole)
//    for(float pitch = 0.f; pitch < 90.f; pitch += step) {
//        // Step through heading, describing a circle
//        for (float heading = 0.f; heading < 360.f; heading += step) {
//            printf("heading: %.2f pitch: %.2f\n", heading, pitch);
//
//            // Tell Merlin to drive to the current lat/long coordinates
//            // (Merlin controller class internally checks these with the gyro)
//            // (this method blocks until the coordinates are reached)
//            merlin.aimAt(heading, pitch);
//
//            // Take measurement with Lidar
//            unsigned int distance = lidar.measureDistance();
//            cout << "distance: " << distance << " cm" << endl;
//            measurements.push_back(Measurement(heading, pitch, distance));
//        }
//    }

    // For now, just one circle
//    for(int i = 0; i < 5; ++i) {
        merlin.startHorizontalCircle();

        while(!merlin.checkHorizontalCircleFull()) {
            // Take measurement with Lidar
            const unsigned int distance = lidar.measureDistance();
            const float heading = merlin.gyro.getHeading();
            const float pitch = merlin.gyro.getPitch();
            cout << "distance: " << distance << " cm, "
                 << "Heading: " << heading << ", "
                 << "Pitch: " << pitch
                 << endl;
            measurements.emplace_back(Measurement(heading, pitch, distance));
        }
//    }

    // Convert measurements into 3D coordinates
    for(int m = 0; m <= measurements.size(); m++){
        Measurement_3D tempMeasurement_3D = calculation.get3DCoordinates(measurements[m].heading, measurements[m].pitch, measurements[m].distance);
        measurements_3D.push_back(tempMeasurement_3D);
    }


    // Write to file
    writeMeasurementsToFile(measurements_3D);

    cout << "Done." << endl;
    return 0;
}