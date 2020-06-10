#include <SDL2/SDL.h>
#include <stdio.h>
#include <cmath>
#include <exception> 
#include <iostream>

#include "HeaderFiles/Point.hpp"
#include "HeaderFiles/sdl.hpp"
#include "HeaderFiles/Triangle.hpp"
#include "HeaderFiles/Slope.hpp"


// Takes a slope object and draws it
void drawSlope(Slope* slope, SDL_Renderer* renderer)
{
    Point* p1 = NULL;
    Point* p2 = NULL;
    if (slope->p1->y < slope->p2->y)
    {
        p1 = slope->p1;
        p2 = slope->p2;
    }
    else 
    {
        p1 = slope->p2;
        p2 = slope->p1;
    }
    if (p1->y != p2->y)
    {
        for (int y = p1->y; y <= p2->y; y++)
        {
            int x = p1->x + (p2->x - p1->x)*(y - p1->y) / (p2->y - p1->y);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    } else {
        int x = 0;
        int maxX = 0;
        if (p1->x < p2->x)
        {
            x = p1->x;
            maxX = p2->x;
        }
        else
        {
            x = p2->x;
            maxX = p1->x;
        }
        for (x ; x < maxX; x++) {
            SDL_RenderDrawPoint(renderer, x, p1->y);
        }
    }
}

// Takes a triangle object and draws it
void drawTriangle(Triangle* tri, SDL_Renderer* renderer)
{
    for (Slope* slope : tri->allSlopes)
    {
        drawSlope(slope, renderer);
    }
}

// Determines if the triangle has a slope that is horizontal
Slope* hasHorizontalSlope(Triangle* tri)
{
    for (Slope* slope : tri->allSlopes)
    {
        if (slope->p1->y == slope->p2->y) {
            return slope;
        }
    }
    return NULL;
}

// makes the first element of sideSlopes, point to the slope that appears on the left side of the horizontal triangle
void alignSlopes(Slope** sideSlopes)
{
    Point* s1UniquePoint = NULL;
    Point* s2UniquePoint = NULL;
    if (sideSlopes[0]->p1 != sideSlopes[1]->p1)
    {

        // Checks what slope has a point that is furtherest left
        if (sideSlopes[0]->p1->x > sideSlopes[1]->p1->x) {
            // If slope sideSlope[1] has a point that is furtherest left then allSlopes[0] and allSlopes[1] will be swapped
            Slope* temp = sideSlopes[0];
            sideSlopes[0] = sideSlopes[1];
            sideSlopes[1] = temp;   
        }
    } else {
        // Checks what slope has a point that is further left
        if (sideSlopes[0]->p1->x > sideSlopes[1]->p1->x) {
            // If slope sideSlope[1] has a point that is further left then the slopes in sideSlope will be swapped
            Slope* temp = sideSlopes[0];
            sideSlopes[0] = sideSlopes[1];
            sideSlopes[1] = temp;   
        }
    }  
}

Slope** getSideSlopes(Triangle* tri, Slope* horizontalSlope)
{
    Slope** sideSlopes = new Slope*[2];
    int count = 0;
    for (Slope* slope : tri->allSlopes)
    {
        if (slope != horizontalSlope) 
        {
            sideSlopes[count] = slope;
            count += 1;
        }
    }
    alignSlopes(sideSlopes);
    return sideSlopes;
}

void fillTriangle(Triangle* tri, SDL_Renderer* renderer)
{
    Slope* horizontalSlope = hasHorizontalSlope(tri);
    if (horizontalSlope != NULL) {
        Slope** sideSlopes = getSideSlopes(tri, horizontalSlope);
        Point* sharedPoint = NULL;

        // Finds the shared point of the side slopes
        if (sideSlopes[0]->p1->y != horizontalSlope->p1->y)
        {
            sharedPoint = sideSlopes[0]->p1;
        }
        else
        {
            sharedPoint = sideSlopes[0]->p2;
        }

        // Gets the unique points of the sideSlopes
        Point* s1uniquePoint = NULL;
        if (sideSlopes[0]->p1 != sharedPoint)
        {
            s1uniquePoint = sideSlopes[0]->p1;
        }
        else
        {
            s1uniquePoint = sideSlopes[0]->p2;
        }

        Point* s2uniquePoint = NULL;
        if (sideSlopes[1]->p1 != sharedPoint)
        {
            s2uniquePoint = sideSlopes[1]->p1;
        }
        else
        {
            s2uniquePoint = sideSlopes[1]->p2;
        }

        bool isAbove = false;
        
        // Determines if shared point is above or below horizontal slope
        if (sharedPoint->y < horizontalSlope->p1->y)
        {
            isAbove = true;
        }

        // Draws triangle if shared point is above horizontal slope
        if (isAbove)
        {
            for (int y = sharedPoint->y; y < s2uniquePoint->y; y++)
            {
                int x1 = sharedPoint->x + (s1uniquePoint->x - sharedPoint->x)*(y - sharedPoint->y) / (s1uniquePoint->y - sharedPoint->y);
                int x2 = sharedPoint->x + (s2uniquePoint->x - sharedPoint->x)*(y - sharedPoint->y) / (s2uniquePoint->y - sharedPoint->y);
                for (int x = x1 + 1; x < x2; x++)
                {
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }
        // Draws triangle if shared point is below horizontal slope
        else
        {
            for (int y = s1uniquePoint->y; y < sharedPoint->y; y++)
            {
                int x1 = s1uniquePoint->x + (sharedPoint->x - s1uniquePoint->x)*(y - s1uniquePoint->y) / (sharedPoint->y - s1uniquePoint->y);
                int x2 = s2uniquePoint->x + (sharedPoint->x - s2uniquePoint->x)*(y - s2uniquePoint->y) / (sharedPoint->y - s2uniquePoint->y);
                for (int x = x1 + 1; x < x2; x++)
                {
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }
    } 
    // Draws triangles with no horizontal slope
    else
    {
        // Finds the point that has a y coordinate that lies between the other points
        Point* allPoints[3] = {tri->allSlopes[0]->p1, tri->allSlopes[1]->p1, tri->allSlopes[2]->p1};
        Point* middlePoint = NULL;
        if (allPoints[0]->y < allPoints[1]->y && allPoints[0]->y > allPoints[2]->y || allPoints[0]->y > allPoints[1]->y && allPoints[0]->y < allPoints[2]->y)
        {
            middlePoint = allPoints[0];
        }
        else if (allPoints[1]->y < allPoints[0]->y && allPoints[1]->y > allPoints[2]->y || allPoints[1]->y > allPoints[0]->y && allPoints[1]->y < allPoints[2]->y)
        {
            middlePoint = allPoints[1];
        }
        else if (allPoints[2]->y < allPoints[0]->y && allPoints[2]->y > allPoints[1]->y || allPoints[2]->y > allPoints[0]->y && allPoints[2]->y < allPoints[1]->y)
        {
            middlePoint = allPoints[2];
        }

        Point* lowestPoint = NULL;
        for (Point* point : allPoints)
        {
            if (point->y > middlePoint->y)
            {
                lowestPoint = point;
            }
        }

        Point* highestPoint = NULL;
        for (Point* point : allPoints)
        {
            if (point->y < middlePoint->y)
            {
                highestPoint = point;
            }
        }

        // Gets the bend slopes and the non bend slope of the triangle
        Slope* bendSlopes[2];
        Slope* nonBendSlope = NULL;
        int count = 0;
        for (Slope* slope : tri->allSlopes)
        {
            if (slope->p1 == middlePoint || slope->p2 == middlePoint)
            {
                bendSlopes[count] = slope;
                count++;
            }
            else
            {
                nonBendSlope = slope;
            }
        }

        // Arranges slopes in order of what slopes are highest
        if (bendSlopes[1]->p1 == highestPoint || bendSlopes[1]->p2 == highestPoint)
        {
            Slope* tempSlope = bendSlopes[0];
            bendSlopes[0] = bendSlopes[1];
            bendSlopes[1] = tempSlope;
        }

        // Determines where the bend is on the triangle
        bool isRight = false;
        if (middlePoint->x == lowestPoint->x)
        {
            if (highestPoint->x > lowestPoint->x)
            {
                isRight = true;
            }
        }
        else if (middlePoint->x > lowestPoint->x)
        {
            isRight = true;
        }


        // Draws triangle if bend is on the right
        if (isRight)
        {
            int nonBendMidX = 0;
            for (int i = 0; i < 2; i++)
            {

                // Gets the slopes high and low points
                Point* slopeHighPoint = NULL;
                Point* slopeLowPoint = NULL;
                if (bendSlopes[i]->p1->y < bendSlopes[i]->p2->y)
                {
                    slopeHighPoint = bendSlopes[i]->p1;
                    slopeLowPoint = bendSlopes[i]->p2;
                }
                else
                {
                    slopeHighPoint = bendSlopes[i]->p2;
                    slopeLowPoint = bendSlopes[i]->p1;
                }

                if (i == 0)
                {
                    nonBendMidX = slopeHighPoint->x + (slopeLowPoint->x - slopeHighPoint->x) * (middlePoint->y - slopeHighPoint->y) / (slopeLowPoint->y - slopeHighPoint->y);
                }

                // for (int y = slopeHighPoint->y; y < slopeLowPoint->y; y++)
                // {
                //     int x1 = slopeHighPoint->x + (slopeLowPoint->x - slopeHighPoint->x)*(y - slopeHighPoint->y) / (slopeLowPoint->y - slopeHighPoint->y);
                //     int x2 = s2uniquePoint->x + (sharedPoint->x - s2uniquePoint->x)*(y - s2uniquePoint->y) / (sharedPoint->y - s2uniquePoint->y);
                //     for (int x = x1 + 1; x < x2; x++)
                //     {
                //         SDL_RenderDrawPoint(renderer, x, y);
                //     }
                // }

            }
        }

    }
}

  
int main() 
{
    int WINDOW_WIDTH = 1000;
    int WINDOW_HEIGHT = 1000;
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    int i;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL count not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
    
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    //SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // The two points which I will draw a line between
    Point p1(0, 0);
    Point p2(1000, 500);
    Point p3(100, 700);
    Slope s1(&p1, &p2);
    Slope s2(&p2, &p3);
    Slope s3(&p3, &p1);
    Triangle* tri = new Triangle(&s1, &s2, &s3);
    drawTriangle(tri, renderer);
    fillTriangle(tri, renderer);
        
    SDL_RenderPresent(renderer);
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    }
    return 0; 
}





