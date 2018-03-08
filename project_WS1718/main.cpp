#include <iostream>
#include <vector>
using std::vector;
#include "Lidar.h"
#include "Merlin.h"
//#include "libs/merlin/MerlinHalfSqhere.h"
#include "Calculation.h"
#include <fstream>

void writeMeasurementsToFile(const string &filename, const vector<Measurement_3D> &measurements_3D)
{
    ofstream fileout;
    fileout.open(filename + ".ply");
    fileout << "ply\n";
    fileout << "format ascii 1.0\n";
    fileout << "element vertex " << measurements_3D.size() << "\n";
    fileout << "property float x\n";
    fileout << "property float y\n";
    fileout << "property float z\n";
    fileout << "end_header\n";

    for(const Measurement_3D &m : measurements_3D)
    {
        fileout << m.x << " " << m.y << " " << m.z << "\n";
    }
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
    merlin.startHorizontalCircle(Merlin::CLOCKWISE);

    //while(!merlin.checkHorizontalCircleFull()) {
    for(int i = 0; i < 40; ++i) {
        // Take measurement with Lidar
        const unsigned int distance = lidar.measureDistance();
        const float heading = merlin.gyro.getHeading();
        const float pitch = merlin.gyro.getPitch();
        cout << "distance: " << distance << " cm, "
             << "Heading: " << heading << ", "
             << "Pitch: " << pitch
             << endl;
        measurements.emplace_back(Measurement(heading, pitch, distance));

        usleep(100000);
    }
    // Test
    merlin.stopMotor(merlin.motorHeading);
    merlin.waitForStop(merlin.motorHeading);

    // Measurement test code
//    for(int pitch = 0; pitch < 90; pitch += 5) {
//        // Describe a horizontal circle
//        for (int heading = 0; heading < 360; heading += 10) {
//            measurements.emplace_back(Measurement(heading, pitch, 2));
//        }
//    }

    // Convert measurements into 3D coordinates
    for(const Measurement &m : measurements){
        // Note that we need to switch heading and pitch here - TODO make use of heading/pitch consistent
        measurements_3D.emplace_back(Calculation::get3DCoordinates(m.pitch, m.heading, m.distance));
    }

    // Write to file
    writeMeasurementsToFile("3dPoints", measurements_3D);
    cout << "test" << endl;

    cout << "Done." << endl;
    return 0;
}