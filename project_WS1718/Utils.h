#ifndef PROJECT_WS1718_UTILS_H
#define PROJECT_WS1718_UTILS_H

#include <cmath>
#include <cassert>
#include <fstream>
#include <vector>
#include "Calculation.h"

namespace utils {
    // Returns the smallest difference between two angles.
    // Handles rollover at 0°/360° border.
    double headingAngleDelta(double angle1, double angle2);

    // Pitch is in range -180..180
    double pitchAngleDelta(double angle1, double angle2);

    // Roll is in range -90..90
    // If we need it we have to add an extra function for it

    // ---------------
    // PLY file output

    // Write measurements to file as human-readable text.
    // The extension ".ply" is appended to the filename.
    void writeAsciiPLY(const std::string &filename, const std::vector<Measurement_3D> &measurements_3D);

    // Write measurements to file as compact binary data.
    // The extension ".ply" is appended to the filename.
    void writeBinaryPLY(const std::string &filename, const std::vector<Measurement_3D> &measurements_3D);

    // Functions used internally by PLY output
    void writeAsBinary(std::ofstream &fileout, const float value);
    void writePLYHeader(std::ofstream &fileout, const std::string &format, size_t vertexCount);

} // namespace utils

#endif //PROJECT_WS1718_UTILS_H
