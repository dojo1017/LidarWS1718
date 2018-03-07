#ifndef PROJECT_WS1718_UTILS_H
#define PROJECT_WS1718_UTILS_H

#include <cmath>
#include <cassert>

// Returns the smallest difference between two angles.
// Handles rollover at 0°/360° border.
inline double angleDelta(double angle1, double angle2) {
    assert(angle1 >= 0.0 && angle1 <= 360.0);
    assert(angle2 >= 0.0 && angle2 <= 360.0);
    return 180.0 - fabs(fabs(angle1 - angle2) - 180.0);
}

#endif //PROJECT_WS1718_UTILS_H
