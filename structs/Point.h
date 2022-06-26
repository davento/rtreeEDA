#ifndef POINT_H
#define POINT_H

#include "Color.h"
#include <SDL2/SDL.h>
#include <math.h>


enum axis{
    X,
    Y
};


struct Point final{

    int x, y;  

    Point() = default;
    explicit Point(int posX, int posY);
    Point(const Point&) = default;
    Point& operator=(const Point&) = default;
    ~Point() = default;

    friend bool operator==(const Point& left, const Point& right);
    friend bool operator!=(const Point& left, const Point& right);
    int operator[] (int axis) {return (axis == X) ? x : y; }
    const int operator[] (int axis) const {return (axis == X) ? x : y; }
    

    double distance(const Point& other) const;
    bool closeEnough(const Point& other, const int RADIUS) const;
    //TODO: AGREGAR PARAMETRO COLOR
    void draw(SDL_Renderer* renderer, Color color = Color(0,0,0)) const;
    
    static Point createMinPoint(const Point&, const Point&);
    static Point createMaxPoint(const Point&, const Point&);
};

#endif

