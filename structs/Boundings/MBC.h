#ifndef _MBC_H_
#define _MBC_H_

#include "Bound.h"

class MBC final: public Bound{

    private:
        Point ce;//centroide
        double rad; //radious

        void updateCircle();

    public:
        MBC() = default;
        MBC(const Point&, const Point&);
        MBC(const MBC&) = default;
        MBC(const Bound&);

        virtual double perimeter() const override;
        virtual double area() const override;
        virtual double metric() const override;
        virtual void merge(Bound*)  override;
        virtual void merge(const Point&) override;
        virtual void draw(SDL_Renderer* renderer, Color color = Color(0,0,255)) const override;
        virtual bool inArea(Point p) override;
        void merge(const MBC&);
        Point& getCentroid();
        double& getRadious();

};


#endif

