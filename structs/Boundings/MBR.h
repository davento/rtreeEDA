#ifndef MBR_H
#define MBR_H

#include "Bound.h"

class MBR final: public Bound{

    public:
        MBR() = default;
        MBR(const Point&, const Point&);
        MBR(const MBR&) = default;
        MBR(const Bound&);

        virtual double perimeter() const override;
        virtual double area() const override;
        virtual double metric() const override;
        virtual void merge(Bound*)  override;
        virtual void merge(const Point&) override;
        void merge(const MBR&);
        virtual void draw(SDL_Renderer* renderer, Color color = Color(0,0,255)) const override;
        virtual bool inArea(Point p) override;
};


#endif

