//
// Created by Sonja on 07.03.2018.
//

#include "Calculation.h"
#include <math.h>

Measurement_3D Calculation::get3DCoordinates(float pitch_deg, float yaw_deg, unsigned int distance) {
    double pitch_rad = (2*M_PI / 360.0) * pitch_deg;
    double yaw_rad = (2*M_PI / 360.0) * yaw_deg;

    const double x = cos(yaw_rad) * cos(pitch_rad) * distance;
    const double y = sin(yaw_rad) * cos(pitch_rad) * distance;
    const double z = sin(pitch_rad) * distance;

    return Measurement_3D(x, y, z);
}

Measurement_3D Calculation::get3DCoordinates(const Measurement &m) {
    // Note that we need to switch heading and pitch here - TODO make use of heading/pitch consistent
    return get3DCoordinates(m.pitch, m.heading, m.distance);
}