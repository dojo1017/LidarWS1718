#include <iostream>
#include <vector>
using std::vector;
#include "Lidar.h"
#include "Merlin.h"
#include "Calculation.h"
#include "Utils.h"

// Take measurements while waiting for the motor to stop
void measureLoop(Merlin &merlin, Lidar &lidar, vector<Measurement> &measurements, int MEASURE_INTERVAL_MS) {
    int measureCount = 0;

    while(!merlin.hasMotorStopped(merlin.MOTOR_HEADING)) {
        // Take measurement with Lidar
        const unsigned int distanceCentimeters = lidar.measureDistance();
        const float heading = merlin.getGyro().getHeading();
        const float pitch = merlin.getGyro().getPitch();
        measurements.emplace_back(Measurement(heading, pitch, distanceCentimeters));
        // Sleep some time between measurements (time in nanoseconds)
        usleep(MEASURE_INTERVAL_MS * 1000);
        measureCount++;
    }

    cout << "[LIDAR] " << measureCount << " measurements done" << endl;
}

int main(int argc, char **argv) {
    Lidar lidar;
    Merlin merlin;

    vector<Measurement> measurements;
    vector<Measurement_3D> measurements_3D;

    // For now, just one circle
    // TODO: currently does not work (checkCircleFull immediately returns true)
//    merlin.startHorizontalCircle(Merlin::CLOCKWISE);
//	  usleep(1000000);
//    while(!merlin.checkHorizontalCircleFull()){
//        // Take measurement with Lidar
//        const unsigned int distance = lidar.measureDistance();
//        const float heading = merlin.getGyro().getHeading();
//        const float pitch = merlin.getGyro().getPitch();
//        cout << "distance: " << distance << " cm, "
//             << "Heading: " << heading << ", "
//             << "Pitch: " << pitch
//             << endl;
//        measurements.emplace_back(Measurement(heading, pitch, distance));
//
//        usleep(100000);
//    }

    // Winkel um den Motor 2 nach jedem Durchlauf erhöht werden soll
    const int VERTICAL_STEP = 5;
    // How much time to sleep between taking measurements, in milliseconds
    const int MEASURE_INTERVAL_MS = 50;

    // First, drive to 0 degrees
    // TODO: doesn't work
//    merlin.doSequenceStep(0, merlin.MOTOR_HEADING);
//    merlin.waitForStop(merlin.MOTOR_HEADING);
//
//    merlin.doSequenceStep(0, merlin.MOTOR_PITCH);
//    merlin.waitForStop(merlin.MOTOR_PITCH);

    const string date = utils::getTimeAsString();
    cout << "Scan started at " << date << endl;
    cout << "Vertical step " << VERTICAL_STEP
         << ", measure interval " << MEASURE_INTERVAL_MS << " ms" << endl;

    int totalCircles = 90 / VERTICAL_STEP;

    // TODO: Scan more than 90 degrees vertical
    for (int i = 0; i < totalCircles; i++) {
        const time_t start = time(nullptr);

        cout << "------------" << endl;
        cout << "Start Circle " << i + 1 << "/" << totalCircles << endl;

        if (i % 2 == 0) {
            // Drive to 360 degrees, taking measurements along the way
            cout << "[Heading] Driving to 360 degrees" << endl;
            merlin.doSequenceStep(360, merlin.MOTOR_HEADING);
            measureLoop(merlin, lidar, measurements, MEASURE_INTERVAL_MS);
        } else {
            // Drive back to 0 degrees, taking measurements along the way
            cout << "[Heading] Driving to 0 degrees" << endl;
            merlin.doSequenceStep(0, merlin.MOTOR_HEADING);
            measureLoop(merlin, lidar, measurements, MEASURE_INTERVAL_MS);
        }

        // Drive the upper motor up a bit
        const int pitchAngle = VERTICAL_STEP * (i + 1);
        cout << "[Pitch] Driving to " << pitchAngle << " degrees" << endl;
        merlin.doSequenceStep(pitchAngle, merlin.MOTOR_PITCH);

        cout << "Circle done, writing 3D data" << endl;
        // For debugging: write a file after each circle
        for(const Measurement &m : measurements){
            measurements_3D.emplace_back(Calculation::get3DCoordinates(m));
        }
        utils::writeBinaryPLY("3dPoints_" + date + "_" + std::to_string(i), measurements_3D);
        measurements.clear();
        measurements_3D.clear();
        cout << "3D data written" << endl;

        // Show information about the scan time
        time_t elapsed = time(nullptr) - start;
        cout << "Circle done in " << elapsed << " seconds" << endl;
        time_t remaining = (totalCircles - (i + 1)) * elapsed;
        cout << "Remaining time estimate: " << remaining << " seconds ";
        if(remaining > 60) {
            cout << "(" << remaining / 60 << " minutes" << endl;
        } else {
            cout << endl;
        }
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