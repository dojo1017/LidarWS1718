#include "../compass/compassController.h"

class calculation {
public:
        calculation(point* points, face* faces);
        ~calculation();

        void addPoint(position pos, unsigned int distance);
};