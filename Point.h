#ifndef POINT_H
#define POINT_H

#include <SDL2/SDL.h>

struct Point final{
    double x, y;

    Point() = default;
    explicit Point(double posX, double posY);
    Point(const Point&) = default;
    Point& operator =(const Point&) = default;
    ~Point() = default;

    friend bool operator==(const Point& left, const Point& right);
    friend bool operator!=(const Point& left, const Point& right);
    
    static double distance(const Point& from, const Point& to);
    static bool closeEnough(const Point& p1, const Point& p2, const int RADIUS);
    static Point createMinPoint(const Point& left, const Point& right);
    static Point createMaxPoint(const Point& left, const Point& right);

    void draw(SDL_Renderer* renderer) const;
};

#endif

