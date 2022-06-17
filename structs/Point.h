#ifndef POINT_H
#define POINT_H

#include <SDL2/SDL.h>
#include <math.h>


struct Point final{

    int x, y;  

    Point() = default;
    explicit Point(int posX, int posY);
    Point(const Point&) = default;
    Point& operator=(const Point&) = default;
    ~Point() = default;

    friend inline bool operator==(const Point& left, const Point& right);
    friend inline bool operator!=(const Point& left, const Point& right);

    inline double distance(const Point& other) const;
    inline bool insideRadious(const Point& other, const int RADIUS) const;
    //TODO: AGREGAR PARAMETRO COLOR
    void draw(SDL_Renderer* renderer) const;
    
    inline static Point createMinPoint(const Point&, const Point&);
    inline static Point createMaxPoint(const Point&, const Point&);
};

#endif
