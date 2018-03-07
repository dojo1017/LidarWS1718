//
// Created by Sonja on 07.03.2018.
//

#include "Calculation.h"
#include <math.h>

Measurement_3D Calculation::get3DCoordinates(float pitch_deg, float yaw_deg, unsigned int distance){

    Measurement_3D measurement_3D = Measurement_3D(0,0,0);
    double pitch_rad = (2*M_PI / 360.0) * pitch_deg;
    double yaw_rad = (2*M_PI / 360.0) * yaw_deg;

    measurement_3D.x = cos(yaw_rad) * cos(pitch_rad) * distance; //x-Koordinate
    measurement_3D.y = sin(yaw_rad) * cos(pitch_rad); //y-Koordinate
    measurement_3D.z = sin(pitch_rad); //z-Koordinate

    return measurement_3D;
}

