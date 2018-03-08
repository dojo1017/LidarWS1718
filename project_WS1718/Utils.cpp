//
// Created by simon on 08.03.18.
//

#include "Utils.h"
using std::ofstream;
using std::string;
using std::vector;

// Returns the smallest difference between two angles.
// Handles rollover at 0°/360° border.
double utils::headingAngleDelta(double angle1, double angle2) {
    assert(angle1 >= 0.0 && angle1 <= 360.0);
    assert(angle2 >= 0.0 && angle2 <= 360.0);
    return 180.0 - fabs(fabs(angle1 - angle2) - 180.0);
}

// Pitch is in range -180..180
double utils::pitchAngleDelta(double angle1, double angle2) {
    // Bring in range 0..360
    return headingAngleDelta(angle1 + 180.0, angle2 + 180.0);
}

// Roll is in range -90..90
// If we need it we have to add an extra function for it

// ---------------
// PLY file output

void utils::writeAsBinary(ofstream &fileout, const float value) {
    fileout.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

void utils::writePLYHeader(ofstream &fileout, const string &format, size_t vertexCount) {
    fileout << "ply\n";
    fileout << "format " << format << " 1.0\n";
    fileout << "element vertex " << vertexCount << "\n";
    fileout << "property float x\n";
    fileout << "property float y\n";
    fileout << "property float z\n";
    fileout << "end_header\n";
}

void utils::writeAsciiPLY(const string &filename, const vector<Measurement_3D> &measurements_3D) {
    ofstream fileout;
    fileout.open(filename + ".ply");
    writePLYHeader(fileout, "ascii", measurements_3D.size());

    for(const Measurement_3D &m : measurements_3D) {
        fileout << m.x << " " << m.y << " " << m.z << "\n";
    }
}

void utils::writeBinaryPLY(const string &filename, const vector<Measurement_3D> &measurements_3D) {
    ofstream fileout;
    fileout.open(filename + ".ply");
    writePLYHeader(fileout, "binary_little_endian", measurements_3D.size());

    for(const Measurement_3D &m : measurements_3D) {
        writeAsBinary(fileout, m.x);
        writeAsBinary(fileout, m.y);
        writeAsBinary(fileout, m.z);
    }
}