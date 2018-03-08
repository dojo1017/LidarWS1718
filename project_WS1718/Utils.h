#ifndef PROJECT_WS1718_UTILS_H
#define PROJECT_WS1718_UTILS_H

#include <cmath>
#include <cassert>

namespace utils {
    // Returns the smallest difference between two angles.
    // Handles rollover at 0°/360° border.
    inline double headingAngleDelta(double angle1, double angle2) {
        assert(angle1 >= 0.0 && angle1 <= 360.0);
        assert(angle2 >= 0.0 && angle2 <= 360.0);
        return 180.0 - fabs(fabs(angle1 - angle2) - 180.0);
    }

    // Pitch is in range -180..180
    double pitchAngleDelta(double angle1, double angle2) {
        // Bring in range 0..360
        const double a1 = angle1 + 180.0;
        const double a2 = angle2 + 180.0;
        return headingAngleDelta(a1, a2);
    }

    // Roll is in range -90..90
    // If we need it we have to add an extra function for it

} // namespace utils

#endif //PROJECT_WS1718_UTILS_H
