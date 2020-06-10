#ifndef TRIANGLE
#define TRIANGLE

#include "Slope.hpp"

class Triangle
{
    public:
        // Three slopes make a triangle.  
        Slope* allSlopes[3];
        Triangle(Slope*, Slope*, Slope*);

    private:
        double getSlopeLength(Slope*);
};

// Gets the length of a slope
double Triangle::getSlopeLength(Slope* slope)
{
    double length = pow(pow((slope->p1->x - slope->p2->x), 2) + pow((slope->p1->y - slope->p2->y), 2), 0.5);
    return length;
}

Triangle::Triangle(Slope* slope1, Slope* slope2, Slope* slope3)
{
    allSlopes[0] = slope1;
    allSlopes[1] = slope2;
    allSlopes[2] = slope3;
}

#endif
