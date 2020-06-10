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

void alignSlopes(Slope** sideSlopes)
{
    Point* s1UniquePoint = NULL;
    Point* s2UniquePoint = NULL;
    if (sideSlopes[0]->p1 != sideSlopes[1]->p1)
    {

        // Checks what slope has a point that is further left
        if (sideSlopes[0]->p1->x > sideSlopes[1]->p1->x) {
            // If slope sideSlope[1] has a point that is further left then the slopes in sideSlope will be swapped
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

        bool isAbove = false;
        
        // Determines if shared point is above or below horizontal slope
        if (sharedPoint->y < horizontalSlope->p1->y)
        {
            isAbove = true;
        }

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
        
        
        
    } 
    else
    {

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
    Point p1(500, 100);
    Point p2(1000, 700);
    Point p3(0, 700);
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





