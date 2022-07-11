#include "Point.h"

#include "Point.h"
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


double Point::distance(const Point& from, const Point& to){
    return std::sqrt(std::pow(from.x - to.x, 2) + std::pow(from.y - to.y, 2));
}

double Point::distance(const Point& p, const MBC& mb){
    double dist_y;
    double dist_x;
    if(mb.topLeft.y >= p.y && p.y >= mb.bottomRight.y) dist_y = 0;
    else{
        //calcular
        dist_y = std::min(
        abs(p.y - mb.topLeft.y),
        abs(p.y - mb.bottomRight.y)
        );
    }
    if(mb.topLeft.x >= p.x && p.x >= mb.bottomRight.x) dist_x = 0;
    else{
        //calcular
        dist_x = std::min(
        abs(p.x - mb.topLeft.x),
        abs(p.x - mb.bottomRight.x)
        );
    }

    return sqrt( dist_x * dist_x + dist_y * dist_y);
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
