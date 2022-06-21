#include "MBR.h"

MBR::MBR(const Bound& b): MBR(b.getTopLeft(), b.getBottomRight()){

}

MBR::MBR(const Point& a, const Point& b): Bound(a,b){

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
    return perimeter();
}

void MBR::merge(Bound* m){
    MBR* aux = static_cast<MBR*>(m); // estamos seguros de que m es MBR xq el arbol solo trabaja con un tipo de bound
    topLeft = Point::createMinPoint(topLeft, aux->topLeft );
    bottomRight = Point::createMaxPoint(bottomRight, aux->bottomRight );
    aux = nullptr;
}

void  MBR::merge(const Point& p){
    topLeft = Point::createMinPoint(topLeft,p);
    bottomRight = Point::createMaxPoint(bottomRight,p);
}

void MBR::draw(SDL_Renderer* renderer, Color color) const {

    if(topLeft.x == INF && topLeft.y == INF)
        return;
    const int &lx = topLeft.x, &ty = topLeft.y, &rx = bottomRight.x, &by = bottomRight.y;

    SDL_SetRenderDrawColor(renderer, color.RGB[0], color.RGB[1],color.RGB[2], 255);
    SDL_RenderDrawLine(renderer, lx, ty, rx , ty);
    SDL_RenderDrawLine(renderer, lx, ty, lx , by);
    SDL_RenderDrawLine(renderer, lx, by, rx , by);
    SDL_RenderDrawLine(renderer, rx, ty, rx , by);
}


bool MBR::inArea(Point p){

    if(topLeft == bottomRight)
        return p.closeEnough(topLeft,3);
    else
        return(p.x > topLeft.x && p.x < bottomRight.x) &&
            (p.y > topLeft.y && p.y < bottomRight.y);
}

void MBR::merge(const MBR& mbr){
    topLeft = Point::createMinPoint(topLeft, mbr.topLeft);
    bottomRight = Point::createMaxPoint(bottomRight, mbr.bottomRight);
}
