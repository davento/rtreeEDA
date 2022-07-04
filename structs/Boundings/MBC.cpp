#include "MBC.h"
#include <SDL2/SDL_render.h>
#include <cmath>



void MBC::updateCircle(){
    double dist_x  = this->bottomRight.x  - this->topLeft.x;
    double dist_y  = this->bottomRight.y  - this->topLeft.y;

    ce = Point(
        this->topLeft.x + dist_x/2,
        this->topLeft.y + dist_y/2
    );
    
    rad = ce.distance(this->topLeft);
}

MBC::MBC(const Point& a, const Point& b):Bound(a,b){
    updateCircle();
}


MBC::MBC(const Bound& b):Bound(b.getTopLeft(), b.getBottomRight()){
   updateCircle();
}

double MBC::perimeter() const{
    return 2.0 * M_PI * rad;
}


double MBC::area() const{
    return M_PI * rad * rad;
}

double MBC::metric() const{
    return area();
}

void MBC::merge(Bound* m){
    //again, we assume that m is the same type
    MBC* aux = static_cast<MBC*> (m); 
    //logic of merge

    Point temp = this->ce;
    temp.x = (this->ce.x * this->area() +  aux->area() *
                aux->ce.x) / (this->area() + aux->area());
   
    temp.y = (this->ce.y * this->area() +  aux->area() *
                aux->ce.y) / (this->area() + aux->area());

    //double rtemp = 0;
    
   // rtemp = std::max(
   //     temp.distance(this->ce) + this->rad,
   //     temp.distance(aux->ce) + aux->rad);
}

//this merge is only use to expand the MB
//of figure node, so it affects the TL and BR
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
    std::cout<<"mbc1\n";
    // el angulo va en radianes
    double changeRatio = 2*M_PI/perim;
    SDL_SetRenderDrawColor(renderer , color.RGB[0], color.RGB[1], color.RGB[2], 255);
    for(double angle = 0; angle*rad < perim && angle < 2*M_PI; angle += changeRatio){
        SDL_RenderDrawPoint(renderer, ce.x + rad*std::cos(angle),ce.y + rad*std::sin(angle));
    }
    ce.draw(renderer);
    
    if(topLeft.x == INF && topLeft.y == INF)
        return;
    const int &lx = topLeft.x, &ty = topLeft.y, &rx = bottomRight.x, &by = bottomRight.y;

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, lx, ty, rx , ty);
    SDL_RenderDrawLine(renderer, lx, ty, lx , by);
    SDL_RenderDrawLine(renderer, lx, by, rx , by);
    SDL_RenderDrawLine(renderer, rx, ty, rx , by);

}

bool MBC::inArea(Point p) {
    return this->ce.distance(p) < rad;
}

Point& MBC::getCentroid(){
    return this->ce;
}

double& MBC::getRadious(){
    return this->rad;
}
