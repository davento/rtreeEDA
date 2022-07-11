#ifndef FIGURE_H
#define FIGURE_H

#include "MBC.h"
#include <vector>

class FigureNode;

class Figure{
    

        friend class FigureNode;

    public:

        Figure() = default;
        Figure(const MBC&);
        Figure(const Figure&) = default;
        //TODO: chequear si poner const aquí
        const MBC& getBound() const {return myBound;}
        
        const MBC& getCentroid() const{return myBound;}
        
        bool addPoint(const Point& p);
        void draw(SDL_Renderer* renderer, Color color = Color(255,0,0)) const;
        void clear();
    private:
        MBC myBound;
        std::vector<Point> points;
};


#endif
