#include "Point.h"
#include <algorithm>

Point::Point(int posX, int posY): x(posX), y(posY){

}

inline bool operator ==(const Point& Left, const Point& right){
    return (Left.x == right.x) && (Left.y == right.y);
}

inline bool operator !=(const Point& Left, const Point& right){
    return  (Left.x != right.x) || (Left.y != right.y);
}


inline double Point::distance(const Point& other) const{
    return std::sqrt(std::pow(other.x - x, 2) + std::pow(other.y - y, 2));
}

inline bool Point::insideRadious(const Point& other, const int RADIUS) const{
    return this->distance(other) < RADIUS * 2;
}


void Point::draw(SDL_Renderer* renderer) const{
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderDrawPoint(renderer, x, y);
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


inline static Point createMinPoint(const Point& p1, const Point& p2){
    return Point(
            std::min(p1.x, p2.x),
            std::min(p1.y, p2.y)
    );
} 


inline static Point createMaxPoint(const Point& p1, const Point& p2){
    return Point(
            std::max(p1.x, p2.x),
            std::max(p1.y, p2.y)
    );
} 
