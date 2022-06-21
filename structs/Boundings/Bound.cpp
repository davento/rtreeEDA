#include "Bound.h"

Bound::Bound(): topLeft(INF,INF), bottomRight(-INF,-INF){

}

Bound::Bound(const Point& p1, const Point& p2): topLeft(p1), bottomRight(p2){

} 

void Bound::clear(){
    topLeft = Point(INF,INF);
    bottomRight = Point(-INF, -INF);
}
