#ifndef BOUND_H
#define BOUND_H

#include "../Point.h"

/*
 *  BOUND ES UNA CLASE INTERFAZ POR LO CUAL
 *  PODEMOS USAR Bound* para acceder a instancias de sus hijos
 *  con 100% de compatibilidad ya que los hijos solo tienen que 
 *  implementar los metodos del Padre y nada más
 * */
class Bound{

    public:
        static constexpr int INF = 40000;

        Bound();
        Bound(const Point&, const Point&);
        Bound(const Bound&) = default;
        virtual ~Bound(){}

        virtual double perimeter() const = 0;
        virtual double area() const = 0;
        virtual double metric() const = 0;
        virtual void merge(Bound*) = 0;
        virtual void merge(const Point&) = 0;
        virtual void draw(SDL_Renderer* renderer, Color color = Color(0,0,255)) const = 0;
        virtual bool inArea(Point p) = 0;

        Point getTopLeft() const { return topLeft; }
        Point getBottomRight() const { return bottomRight; }
        void clear();

    protected:
        Point topLeft, bottomRight;

};


#endif

