#ifndef POINT
#define POINT

// Points defines a point within the rasterizer grid as a point with an x and y coordinate
class Point
{
    public:
        int x, y;
        Point(int, int);
};

Point::Point(int xCoord, int yCoord)
{
    x = xCoord;
    y = yCoord;
}

#endif