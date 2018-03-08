//
// Created by Sonja on 07.03.2018.
//

#ifndef PROJECT_WS1718_CALCULATION_H
#define PROJECT_WS1718_CALCULATION_H




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
    Calculation(){};
    Measurement_3D get3DCoordinates(float pitch_deg, float yaw_deg, unsigned int distance);
};


#endif //PROJECT_WS1718_CALCULATION_H