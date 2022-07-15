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
        MBC getCopyBound() const {return myBound;}
        const Point& getCentroid() const{return myBound.centroid;}
        int lhv() const{
            return myBound.centroid[Z];
        }
        
        bool addPoint(const Point& p);
        void draw(SDL_Renderer* renderer, Color color = Color(0,250,0)) const;
        void clear();
    private:
        MBC myBound;
        std::vector<Point> points;
};


#endif
