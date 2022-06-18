#include "Bound.h"


Bound::Bound(): topLeft(INF,INF), bottomRight(-INF,-INF){
}

Bound::Bound(const Point& p1, const Point& p2): topLeft(p1), bottomRight(p2){

} 


double MBR::perimeter() const {
    return (abs(topLeft.x - bottomRight.x) +
        abs(topLeft.y - bottomRight.y ));
}

double MBR::area() const {
    return (abs(topLeft.x - bottomRight.x) *
        abs(topLeft.y - bottomRight.y ));
}

double MBR::metric() const{
    return this->perimeter();
}

void MBR::merge(MBR* m){

    this->topLeft = Point::createMinPoint(this->topLeft,
    m->topLeft );
    this->bottomRight = Point::createMaxPoint(this->bottomRight, 
    m->bottomRight );   
}

void  MBR::merge(const Point& p){
    this->topLeft = Point::createMinPoint(this->topLeft,
                    p);
    this->bottomRight = Point::createMaxPoint(this->bottomRight,
                        p);
}

void MBR::draw(SDL_Renderer* renderer) const {

    if(topLeft.x == INF && topLeft.y == INF)
        return;
    const int &lx = topLeft.x, &ty = topLeft.y, &rx = bottomRight.x, &by = bottomRight.y;
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawLine(renderer, lx, ty, rx , ty);
    SDL_RenderDrawLine(renderer, lx, ty, lx , by);
    SDL_RenderDrawLine(renderer, lx, by, rx , by);
    SDL_RenderDrawLine(renderer, rx, ty, rx , by);
}
