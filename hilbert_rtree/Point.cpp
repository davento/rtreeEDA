#include "Point.h"

#include "Gilbert.h"
#include <algorithm>
#include <iostream>

Point::Point(double posX, double posY): x(posX), y(posY){

}

bool operator ==(const Point& Left, const Point& right){
    return (Left.x == right.x) && (Left.y == right.y);
}

bool operator !=(const Point& left, const Point& right){
    return  !(left==right);
}

Point operator-(const Point& left, const Point& right){
    return Point(left.x-right.x, left.y-right.y);
}

int Point::operator[] (int axis){
    if(axis == X) return x;
    else if(axis == Y) return y;
    else return xy2d(*this);
}

int Point::operator[] (int axis) const{
    if(axis == X) return x;
    else if(axis == Y) return y;
    else return xy2d(*this);
}


double Point::distance(const Point& from, const Point& to){
    return std::sqrt(std::pow(from.x - to.x, 2) + std::pow(from.y - to.y, 2));
}

double Point::length(){
    return std::sqrt(x*x + y*y);
}



bool Point::closeEnough(const Point& p1, const Point& p2, const int RADIUS){
    return Point::distance(p1, p2) < RADIUS * 2;
}


void Point::draw(SDL_Renderer* renderer) const{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    int dim = 1;
    SDL_Rect dot{
        static_cast<int>(x - dim),
        static_cast<int>(y - dim),
        3*dim,
        3*dim
    };
    SDL_RenderFillRect(renderer, &dot);
}

Point Point::createMinPoint(const Point& p1, const Point& p2){
    return Point(
            std::min(p1.x, p2.x),
            std::min(p1.y, p2.y)
    );
}


Point Point::createMaxPoint(const Point& p1, const Point& p2){
    return Point(
            std::max(p1.x, p2.x),
            std::max(p1.y, p2.y)
    );
}

void Point::print() const {
    std::cout << "( " << x << " " << y <<" )" << std::endl;
}
