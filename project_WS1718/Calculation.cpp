//
// Created by Sonja on 07.03.2018.
//

#include "Calculation.h"
#include <math.h>

Measurement_3D Calculation::get3DCoordinates(float pitch_deg, float yaw_deg, float distanceMeters) {
    float pitch_rad = (2*M_PI / 360.0) * pitch_deg;
    float yaw_rad = (2*M_PI / 360.0) * yaw_deg;

    const float x = cos(yaw_rad) * cos(pitch_rad) * distanceMeters;
    const float y = sin(yaw_rad) * cos(pitch_rad) * distanceMeters;
    const float z = sin(pitch_rad) * distanceMeters;

    return Measurement_3D(x, y, z);
}

Measurement_3D Calculation::get3DCoordinates(const Measurement &m) {
    // Note that we need to switch heading and pitch here - TODO make use of heading/pitch consistent
    return get3DCoordinates(m.pitch, m.heading, m.distanceMeters);
}