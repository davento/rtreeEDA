#ifndef BOUND_H
#define BOUND_H

#include "Point.h"



class Bound{


    public:
        static constexpr int INF = 40000;

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
        //used un Rtree remove
        virtual bool inArea(Point p) = 0;
        
        Point getTopLeft() const { return topLeft; }
        Point getBottomRight() const { return bottomRight; }

    protected:
        Point topLeft, bottomRight;

};

class MBR final: public Bound{

    public:
        MBR();
        MBR(const Point&, const Point&);
        MBR(const Bound&);

        virtual double perimeter() const override;
        virtual double area() const override;
        virtual double metric() const override;
        virtual void merge(Bound*)  override;

        virtual void merge(const Point&) override;
    
        //TODO: Agregar color luego
        virtual void draw(SDL_Renderer* renderer) const override;

        virtual bool inArea(Point p) override;
};

#endif
