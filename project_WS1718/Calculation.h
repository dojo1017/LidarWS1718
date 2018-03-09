//
// Created by Sonja on 07.03.2018.
//

#ifndef PROJECT_WS1718_CALCULATION_H
#define PROJECT_WS1718_CALCULATION_H

// Raw measured data point (heading and pitch from gyro, dist in cm from lidar)
struct Measurement {
    Measurement(float heading, float pitch, unsigned int distCentimeters)
            : heading(heading), pitch(pitch), distanceMeters(distCentimeters / 100.f)
    {}

    float heading;
    float pitch;
    // distance in meters (converted from centimeters)
    float distanceMeters;
};

// Converted data point in XYZ object coordinates (unit: meters)
struct Measurement_3D {
    Measurement_3D(float x, float y, float z)
            : x(x), y(y), z(z)
    {}

    float x;
    float y;
    float z;
};

class Calculation{
public:
    static Measurement_3D get3DCoordinates(float pitch_deg, float yaw_deg, float distanceCentimeters);
    static Measurement_3D get3DCoordinates(const Measurement &m);
};


#endif //PROJECT_WS1718_CALCULATION_H