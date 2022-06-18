#ifndef BOUND_H
#define BOUND_H

#include "Point.h"

class Bound{

    static constexpr int INF = 40000;

    public:
        Bound();
        Bound(const Point&, const Point&);
        Bound(const Bound&) = default;
        virtual double perimeter() const = 0;
        virtual double area() const = 0;
        virtual double metric() const = 0;
        virtual void merge(Bound*) = 0;
        virtual void merge(const Point&) = 0;
        //TODO: Agregar color luego
        virtual void draw(SDL_Renderer* renderer) const = 0;

    protected:
        Point topLeft, bottomRight;

};

#endif
