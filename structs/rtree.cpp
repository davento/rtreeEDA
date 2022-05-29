#include "rtree.h"

Rtree::Rtree(Figure f):myFigure(f){
    
}


Rtree* Rtree::search(const Figure& f){
    //TODO
    return {};
}

void Rtree::split(){

}


bool Rtree::insert(Figure f){
    Rtree* cur = search(f);
    
    if(cur->cur_figs >= ORDER){
        //split()
        return true;
    }

    this->node[cur_figs++] = &Rtree{f};
    MBB::merge(this->minimum, f.bound);
}

