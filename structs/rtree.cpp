#include "rtree.h"

Rtree::Rtree(Figure f):myFigure(f){
    
}


Rtree* Rtree::search(const Figure& f){
    //TODO: search
    return {};

}

void Rtree::split(){
    
}


bool Rtree::insert(Figure f){
    Rtree* cur = search(f);
    
    if(cur->cur_figs >= ORDER){
        // choose subtree
        MBB res = {INF, INF};
        int minP = INF;
        for(auto region : regions) {
            MBB aux = MBB::merge(f.bound, region->minimum);
            int l = aux.bottomRight.x-aux.topLeft.x;
            int w = aux.bottomRight.y-aux.topLeft.y;
            if(2*(l+w) <= minP) {
                minP = 2*(l+w);
                res = aux;
            }
        }
        //split()
        return true;
    }

    this->regions[cur_figs++] = &Rtree{f};
    MBB::merge(this->minimum, f.bound);
}

