#include "MBC.h"
#define INF 20000000000

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

void MBC::merge(const Point &p){

    topLeft = Point::createMinPoint(topLeft,p);
    bottomRight = Point::createMaxPoint(bottomRight,p);
    updateCircle();
}

void MBC::merge(const MBC& other){
}

void MBC::draw(SDL_Renderer* renderer, Color color) const {
    //draw circle
    double perim = perimeter();
    // el angulo va en radianes
    double changeRatio = 2*M_PI/perim;
    SDL_SetRenderDrawColor(renderer , color.RGB[0], color.RGB[1], color.RGB[2], 255);
    for(double angle = 0; angle*radius < perim && angle < 2*M_PI; angle += changeRatio){
        SDL_RenderDrawPoint(renderer, centroid.x + radius*std::cos(angle),centroid.y + radius*std::sin(angle));
    }
    centroid.draw(renderer);

    if(topLeft.x == INF && topLeft.y == INF)
        return;
    const int &lx = topLeft.x, &ty = topLeft.y, &rx = bottomRight.x, &by = bottomRight.y;

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, lx, ty, rx , ty);
    SDL_RenderDrawLine(renderer, lx, ty, lx , by);
    SDL_RenderDrawLine(renderer, lx, by, rx , by);
    SDL_RenderDrawLine(renderer, rx, ty, rx , by);

}

