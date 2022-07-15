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
        MBC(const Point& p, int length);
        MBC(const MBC&) = default;
        MBC(const Point& inCentroid, double inRadius);
        MBC(const Point&, const Point&);

        double perimeter() const;
        double area() const;
        double metric() const;
        bool inArea(const Point& p) const;
        
        double& getRadius(){return radius;}
        const double& getRadius() const {return radius;}
        Point& getCentroid(){return centroid;}
        const Point& getCentroid() const{return centroid;}
        Point getTopLeft() const {return topLeft;}
        Point getBottomRight() const {return bottomRight;}


        static double distance(const Point& from, const MBC& to);

        void draw(SDL_Renderer* renderer, Color color = Color(0,0,255)) const;
        void merge(const Point&);
        void merge(const MBC&);

    public:
        Point centroid;
        double radius;
        Point topLeft, bottomRight;
        void updateCircle();
};

#endif

