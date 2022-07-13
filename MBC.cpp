#include "MBC.h"
#define INF 20000000000

MBC::MBC(const Point& p, int length): topLeft(p), bottomRight(p.x+length, p.y+length){

}

MBC::MBC(): topLeft(INF,INF), bottomRight(-INF,-INF){
    updateCircle();
}

MBC::MBC(const Point& inCentroid, double inRadius): centroid(inCentroid), radius(inRadius){

}

MBC::MBC(const Point& tp, const Point& br){
    topLeft = tp;
    bottomRight = br;
    updateCircle();
}

void MBC::updateCircle(){
    double dist_x  = bottomRight.x - topLeft.x;
    double dist_y  = bottomRight.y - topLeft.y;

    centroid = Point(
        topLeft.x + dist_x/2,
        topLeft.y + dist_y/2
    );

    radius = Point::distance(centroid, topLeft);
}


double MBC::perimeter() const{

    int l = this->bottomRight.x - this->topLeft.x;
    int w = this->bottomRight.y - this->topLeft.y;

    return std::abs(2 * (l + w));
}


double MBC::area() const{

    int l = this->bottomRight.x - this->topLeft.x;
    int w = this->bottomRight.y - this->topLeft.y;

    return std::abs(l * w);
}


double MBC::metric() const{

    return perimeter();
}

bool MBC::inArea(const Point& p) const{
    
    if(this->topLeft == this->bottomRight){
        if(Point::closeEnough(centroid, p,3)){
            return true;
        }
        return false;
    }
    if(
        (p.x > this->topLeft.x && p.x < this->bottomRight.x) &&
        (p.y > this->topLeft.y && p.y < this->bottomRight.y)
    )
        return true;
    return false;
}


void MBC::merge(const Point &p){

    topLeft = Point::createMinPoint(topLeft,p);
    bottomRight = Point::createMaxPoint(bottomRight,p);
    updateCircle();
}

void MBC::merge(const MBC& other){
    
    topLeft = Point::createMinPoint(topLeft,other.topLeft);
    bottomRight = Point::createMaxPoint(bottomRight,other.bottomRight);
    updateCircle();
}

void MBC::draw(SDL_Renderer* renderer, Color color) const {
    
    // std::cout<<"c\n";
    
    // centroid.draw(renderer);

    if(topLeft.x == INF && topLeft.y == INF)
        return;
    const int &lx = topLeft.x, &ty = topLeft.y, &rx = bottomRight.x, &by = bottomRight.y;

    SDL_SetRenderDrawColor(renderer , color.RGB[0], color.RGB[1], color.RGB[2], 255);
    SDL_RenderDrawLine(renderer, lx, ty, rx , ty);
    SDL_RenderDrawLine(renderer, lx, ty, lx , by);
    SDL_RenderDrawLine(renderer, lx, by, rx , by);
    SDL_RenderDrawLine(renderer, rx, ty, rx , by);

}



double MBC::distance(const Point& p, const MBC& mb){
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
