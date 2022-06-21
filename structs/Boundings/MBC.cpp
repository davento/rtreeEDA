#include "MBC.h"



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

    double rtemp = 0;
    
    rtemp = std::max(
        temp.distance(this->ce) + this->rad,
        temp.distance(aux->ce) + aux->rad);
}

//this merge is only use to expand the MB
//of figure node, so it affects the TL and BR
void MBC::merge(const Point &p){
    
    topLeft = Point::createMinPoint(topLeft,p);
    bottomRight = Point::createMaxPoint(bottomRight,p);
    updateCircle();
}

void MBC::draw(SDL_Renderer* renderer, Color color = Color(0,0,255)) const {
    //draw circle
}

bool MBC::inArea(Point p) {
    return this->ce.distance(p) < rad;
}
