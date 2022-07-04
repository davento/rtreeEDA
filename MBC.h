#ifndef MBC_H
#define MBC_H

#define _USE_MATH_DEFINES
#include "Point.h"
#include "Color.h"
#include <cmath>
#include <SDL2/SDL.h>

class MBC{
    //TODO: MBC
    public:
        MBC();
        MBC(const MBC&) = default;
        MBC(const Point& inCentroid, double inRadius);
        MBC(const Point&, const Point&);

        double perimeter() const {return 2*M_PI*radius;}
        double area() const {return (radius == 1) ? 1 : M_PI*radius*radius;}
        double metric() const {return perimeter();}
        bool inArea(const Point& p) const {return Point::distance(p,centroid) < radius;}
        double& getRadius(){return radius;}
        Point& getCentroid(){return centroid;}
        const Point& getCentroid() const{return centroid;}


        void draw(SDL_Renderer* renderer, Color color = Color(0,0,255)) const;
        void merge(const Point&);
        void merge(const MBC&);

    private:
        Point centroid;
        double radius;
        Point topLeft, bottomRight;
        void updateCircle();
};

#endif

