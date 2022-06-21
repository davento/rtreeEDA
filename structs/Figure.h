#ifndef FIGURE_H
#define FIGURE_H

#include "Boundings/Bound.h"
#include <vector>

template <typename, unsigned>
class FigureNode;

class Figure{
    
    template <typename T, unsigned ORDER>
        friend class FigureNode;

    public:
        Figure(Bound*);
        Figure(const Figure&) = default;
        //TODO: chequear si poner const aquí
        Bound* getBound();
        bool addPoint(const Point& p);
        void draw(SDL_Renderer* renderer, Color color = Color(255,0,0)) const;
        void clear();
    private:
        Bound* myBound;
        std::vector<Point> points;
};


#endif

