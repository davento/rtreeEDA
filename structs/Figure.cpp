#include "Figure.h"
#include "Boundings/MBC.h"

Figure::Figure(Bound* mb): myBound(new MBC(*static_cast<MBC*>(mb))){

}

Bound* Figure::getBound(){
    return myBound;
}

bool Figure::addPoint(const Point&p){

    if (points.size() == 0 or
        !points.front().closeEnough(p,4))
    {
        points.push_back(p);
        myBound->merge(p);
        return true;
    }
    else if(points.front().closeEnough(p,4))
    {
        points.push_back(points.front());
        return false;
    }
    return true;
}

void Figure::draw(SDL_Renderer* renderer, Color color) const{

    if(points.size() == 0)
        return;
    for(const auto& point: points)     
        point.draw(renderer);
    if(points.size() <= 1)
            return;
    SDL_SetRenderDrawColor(renderer, color.RGB[0], color.RGB[1], color.RGB[2], 255);
    
    for(size_t i = 0; i != points.size() - 1; i++){
        SDL_RenderDrawLine(renderer, points[i].x, points[i].y, points[i+1].x, points[i+1].y);
    }
    //myBound->draw(renderer, color);
}

void Figure::clear(){
    points.clear();
    myBound->clear();
}

