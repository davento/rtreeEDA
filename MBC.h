#ifndef MBC_H
#define MBC_H

#define _USE_MATH_DEFINES
#include "Point.h"
#include <cmath>
#include <SDL2/SDL.h>

class MBC{
    //TODO: MBC
    public:
        MBC() = default;
        MBC(const MBC&) = default;
        MBC(const Point& inCentroid, double inRadius);
        MBC(const Point&, const Point&);

        double perimeter() const {return 2*M_PI*radius;}
        double area() const {return M_PI*radius*radius;}
        double metric() const {return perimeter();}
        bool inArea(const Point& p) const {return Point::distance(p,centroid) < radius;}
        double& getRadius(){return radius;}
        Point& getCentroid(){return centroid;}

        void draw(SDL_Renderer* renderer) const;
        void merge(const Point&);
        void merge(const MBC&);

    private:
        Point centroid;
        double radius;
        Point topLeft, bottomRight;
        void updateCircle();
};

#endif

