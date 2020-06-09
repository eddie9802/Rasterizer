#ifndef SLOPE
#define SLOPE

#include "Point.hpp"

// Defines a slope that is to be drawn on the grid
class Slope
{
    public:
        Point* p1;
        Point* p2;
        Slope(Point*, Point*);
        Slope();
};

// A line is drawn from point1 to point2
Slope::Slope(Point* point1, Point* point2)
{
    p1 = point1;
    p2 = point2;
}

Slope::Slope()
{

}

#endif