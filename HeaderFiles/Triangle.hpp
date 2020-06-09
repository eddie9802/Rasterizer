#ifndef TRIANGLE
#define TRIANGLE

#include "Slope.hpp"

class Triangle
{
    public:
        // Three slopes make a triangle.  
        Slope* allSlopes[3];
        Slope* longestSlope;
        Slope* bendSlopes[2];
        Triangle(Slope*, Slope*, Slope*);

    private:
        void setLongestSlope();
        double getSlopeLength(Slope*);
};

// Gets the length of a slope
double Triangle::getSlopeLength(Slope* slope)
{
    double length = pow(pow((slope->p1->x - slope->p2->x), 2) + pow((slope->p1->y - slope->p2->y), 2), 0.5);
    return length;
}

// Determines the longest slope of the triangle
void Triangle::setLongestSlope()
{
    if (getSlopeLength(allSlopes[0]) > getSlopeLength(allSlopes[1]))
    {
        longestSlope = allSlopes[0];
        bendSlopes[0] = allSlopes[1];
        bendSlopes[1] = allSlopes[2];
    } else {
        longestSlope = allSlopes[1];
        bendSlopes[0] = allSlopes[0];
        bendSlopes[1] = allSlopes[2];
    }

    if (getSlopeLength(allSlopes[2]) > getSlopeLength(longestSlope))
    {
        longestSlope = allSlopes[2];
        bendSlopes[0] = allSlopes[0];
        bendSlopes[1] = allSlopes[1];
    }

}

Triangle::Triangle(Slope* slope1, Slope* slope2, Slope* slope3)
{
    allSlopes[0] = slope1;
    allSlopes[1] = slope2;
    allSlopes[2] = slope3;
    setLongestSlope();
}

#endif
