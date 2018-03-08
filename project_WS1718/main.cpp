#include <iostream>
#include <vector>
using std::vector;
#include "Lidar.h"
#include "Merlin.h"
#include "Calculation.h"
#include "Utils.h"

int main(int argc, char **argv) {
    Lidar lidar;
    Merlin merlin;

    vector<Measurement> measurements;
    vector<Measurement_3D> measurements_3D;
//    const float step = 5.f;

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
//    merlin.startHorizontalCircle(Merlin::CLOCKWISE);

    //while(!merlin.checkHorizontalCircleFull()) {
//    for(int i = 0; i < 10; ++i) {
//        Merlin::Direction dir = (i % 2 == 0) ? Merlin::CLOCKWISE : Merlin::COUNTERCLOCKWISE;
//        merlin.moveMotor(merlin.motorHeading, dir, Merlin::FAST);
//
//        for(int j = 0; j < 10; ++j) {
//            Merlin::Direction dir2 = (j % 2 == 0) ? Merlin::CLOCKWISE : Merlin::COUNTERCLOCKWISE;
//            merlin.moveMotor(merlin.motorPitch, dir2, Merlin::FAST);
//
//            for(int k = 0; k < 100; ++k) {
//                // Take measurement with Lidar
//                const unsigned int distance = lidar.measureDistance();
//                const float heading = merlin.gyro.getHeading();
//                const float pitch = merlin.gyro.getPitch();
//                cout << "distance: " << distance << " cm, "
//                     << "Heading: " << heading << ", "
//                     << "Pitch: " << pitch
//                     << endl;
//                measurements.emplace_back(Measurement(heading, pitch, distance));
//
//                usleep(500000);
//            }
//
//            merlin.stopMotor(merlin.motorPitch);
//            merlin.waitForStop(merlin.motorPitch);
//        }
//
//        merlin.stopMotor(merlin.motorHeading);
//        merlin.waitForStop(merlin.motorHeading);
//    }
//    // Test
//    merlin.stopMotor(merlin.motorHeading);
//    merlin.waitForStop(merlin.motorHeading);

    // Measurement test code
//    for(int pitch = 0; pitch < 90; pitch += 5) {
//        // Describe a horizontal circle
//        for (int heading = 0; heading < 360; heading += 10) {
//            measurements.emplace_back(Measurement(heading, pitch, 2));
//        }
//    }


    // Winkel um den Motor 2 nach jedem Durchlauf erhöht werden soll
    const int ANGLE_UP = 10;

    // Winkel um den sich Motor 1 drehen soll, bevor sich Motor 2 bewegt - Für den Produkttivbetrieb 360
    const int ANGLE_SIDE = 360;

    // Basically a direct port from MerlinHalfSphere
    const int step = 5;
    for (int i = 0; i < 90 / step; i++) {
        if (i % 2 == 0) {
            merlin.doSequenceStep(ANGLE_SIDE, merlin.motorHeading);
        } else {
            merlin.doSequenceStep(0, merlin.motorHeading);
        }

        merlin.doSequenceStep(ANGLE_UP * (i + 1), merlin.motorPitch);
    }

    // Convert measurements into 3D coordinates
    for(const Measurement &m : measurements){
        // Note that we need to switch heading and pitch here - TODO make use of heading/pitch consistent
        measurements_3D.emplace_back(Calculation::get3DCoordinates(m));
    }

    // Write to file
    utils::writeBinaryPLY("3dPoints", measurements_3D);

    cout << "Done." << endl;
    return 0;
}