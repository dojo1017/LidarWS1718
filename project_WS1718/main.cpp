#include <iostream>
#include <vector>
using std::vector;
#include "Lidar.h"
#include "Merlin.h"
#include "Calculation.h"
#include "Utils.h"

// Take measurements while waiting for the motor to stop
void measureLoop(Merlin &merlin, Lidar &lidar, vector<Measurement> &measurements, int MEASURE_INTERVAL_MS) {
    while(!merlin.hasMotorStopped(merlin.MOTOR_HEADING)) {
        // Take measurement with Lidar
        const unsigned int distanceCentimeters = lidar.measureDistance();
        const float heading = merlin.getGyro().getHeading();
        const float pitch = merlin.getGyro().getPitch();
        measurements.emplace_back(Measurement(heading, pitch, distanceCentimeters));
        // Sleep some time between measurements (time in nanoseconds)
        usleep(MEASURE_INTERVAL_MS * 1000);
    }
}

int main(int argc, char **argv) {
    Lidar lidar;
    Merlin merlin;

    vector<Measurement> measurements;
    vector<Measurement_3D> measurements_3D;
//    const float HORIZONTAL_STEP = 5.f;

//    merlin.aimAt(0,0); //an Start
//
//    // Step through pitch from 0° (equator) to 90° (north pole)
//    for(float pitch = 0.f; pitch < 90.f; pitch += HORIZONTAL_STEP) {
//        // Step through heading, describing a circle
//        for (float heading = 0.f; heading < 360.f; heading += HORIZONTAL_STEP) {
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

    while(!merlin.checkHorizontalCircleFull()){
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
    };

    /*
    while(!merlin.checkHorizontalCircleFull()) {
    for(int i = 0; i < 10; ++i) {
        Merlin::Direction dir = (i % 2 == 0) ? Merlin::CLOCKWISE : Merlin::COUNTERCLOCKWISE;
        merlin.moveMotor(merlin.motorHeading, dir, Merlin::FAST);

        for(int j = 0; j < 10; ++j) {
            Merlin::Direction dir2 = (j % 2 == 0) ? Merlin::CLOCKWISE : Merlin::COUNTERCLOCKWISE;
            merlin.moveMotor(merlin.motorPitch, dir2, Merlin::FAST);

            for(int k = 0; k < 100; ++k) {
                // Take measurement with Lidar
                const unsigned int distance = lidar.measureDistance();
                const float heading = merlin.gyro.getHeading();
                const float pitch = merlin.gyro.getPitch();
                cout << "distance: " << distance << " cm, "
                     << "Heading: " << heading << ", "
                     << "Pitch: " << pitch
                     << endl;
                measurements.emplace_back(Measurement(heading, pitch, distance));

                usleep(500000);
            }

            merlin.stopMotor(merlin.motorPitch);
            merlin.waitForStop(merlin.motorPitch);
        }
    //while(!merlin.checkHorizontalCircleFull()) {
//    for(int i = 0; i < 10; ++i) {
//        Merlin::Direction dir = (i % 2 == 0) ? Merlin::CLOCKWISE : Merlin::COUNTERCLOCKWISE;
//        merlin.moveMotor(merlin.MOTOR_HEADING, dir, Merlin::FAST);
//
//        for(int j = 0; j < 10; ++j) {
//            Merlin::Direction dir2 = (j % 2 == 0) ? Merlin::CLOCKWISE : Merlin::COUNTERCLOCKWISE;
//            merlin.moveMotor(merlin.MOTOR_PITCH, dir2, Merlin::FAST);
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
//            merlin.stopMotor(merlin.MOTOR_PITCH);
//            merlin.waitForStop(merlin.MOTOR_PITCH);
//        }
//
//        merlin.stopMotor(merlin.MOTOR_HEADING);
//        merlin.waitForStop(merlin.MOTOR_HEADING);
//    }
//    // Test
//    merlin.stopMotor(merlin.MOTOR_HEADING);
//    merlin.waitForStop(merlin.MOTOR_HEADING);

    // Measurement test code
//    for(int pitch = 0; pitch < 90; pitch += 5) {
//        // Describe a horizontal circle
//        for (int heading = 0; heading < 360; heading += 10) {
//            measurements.emplace_back(Measurement(heading, pitch, 2));
//        }
//    }

    // Winkel um den Motor 2 nach jedem Durchlauf erhöht werden soll
    const int VERTICAL_STEP = 5;
    // How much time to sleep between taking measurements, in milliseconds
    const int MEASURE_INTERVAL_MS = 50;

    // First, drive to 0 degrees
    merlin.doSequenceStep(0, merlin.MOTOR_HEADING);
    merlin.waitForStop(merlin.MOTOR_HEADING);

    merlin.doSequenceStep(0, merlin.MOTOR_PITCH);
    merlin.waitForStop(merlin.MOTOR_PITCH);

    // TODO: Scan more than 90 degrees vertical
    for (int i = 0; i < 90 / VERTICAL_STEP; i++) {
        if (i % 2 == 0) {
            // Drive to 360 degrees, taking measurements along the way
            merlin.doSequenceStep(360, merlin.MOTOR_HEADING);
            measureLoop(merlin, lidar, measurements, MEASURE_INTERVAL_MS);
        } else {
            // Drive back to 0 degrees, taking measurements along the way
            merlin.doSequenceStep(0, merlin.MOTOR_HEADING);
            measureLoop(merlin, lidar, measurements, MEASURE_INTERVAL_MS);
        }

        // Drive the upper motor up a bit
        merlin.doSequenceStep(VERTICAL_STEP * (i + 1), merlin.MOTOR_PITCH);


        // For debugging: write a file after each circle
        for(const Measurement &m : measurements){
            measurements_3D.emplace_back(Calculation::get3DCoordinates(m));
        }
        utils::writeBinaryPLY("3dPoints" + std::to_string(i), measurements_3D);
        measurements.clear();
        measurements_3D.clear();
    }

//    // Convert measurements into 3D coordinates
//    for(const Measurement &m : measurements){
//        measurements_3D.emplace_back(Calculation::get3DCoordinates(m));
//    }
//
//    // Write to file
//    utils::writeBinaryPLY("3dPoints", measurements_3D);

    cout << "Done." << endl;
    return 0;
}