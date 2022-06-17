#ifndef FIGURE_H
#define FIGURE_H

#include "Bound.h"
#include <vector>

class Figure{
    public:
        Figure(Bound*);
        //TODO: chequear si poner const aquí
        Bound* getBound();
        bool addPoint(const Point& p);
        //TODO: agregar color más tarde
        void draw(SDL_Renderer* renderer) const;
    private:
        Bound* myBound;
        std::vector<Point> points;
};

#endif
