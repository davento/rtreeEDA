#include "rtree.h"



Rtree::Rtree(Figure* f):myFigure(f){
    
}


Rtree* Rtree::search(const Figure& f){
    //TODO: search
    return {};
}


inline bool Rtree::isLeaf(){
    return this->regions[0]->myFigure != nullptr;
}

//gets the node that
Rtree *Rtree::chooseSubtree(const Figure& f){

    //if node is a figure node
    //return to region node (figure->father)
    if(this->myFigure != nullptr) return this->father;

    MBB res ;

    int minP = INF;
    int minpos = 0;
    int pos = 0;
    //choose region with minimum perimeter
    for(auto region : regions) {
        MBB aux = MBB::merge(f.bound, region->bound);
        int l = aux.bottomRight.x-aux.topLeft.x;
        int w = aux.bottomRight.y-aux.topLeft.y;
        if(2*(l+w) <= minP) {
            minP = 2*(l+w);
            res = aux;
            minpos = pos;
        }
        pos++;
    }

    return regions[pos];
}

void Rtree::split(){
    
}


bool Rtree::insert(Figure *f){

    if(!this->isLeaf()){
        
        return this->chooseSubtree(f)->insert(f);
    }

    Rtree* cur = this;
    
    //if overflow
    if(cur->cur_figs >= ORDER){
        // choose subtree
        MBB res ;

        int minP = INF;
        for(auto region : regions) {
            MBB aux = MBB::merge(f->bound, region->bound);
            int l = aux.bottomRight.x-aux.topLeft.x;
            int w = aux.bottomRight.y-aux.topLeft.y;
            if(2*(l+w) <= minP) {
                minP = 2*(l+w);
                res = aux;
            }
        }

        return true;
    }

    //else insert
    this->regions[cur_figs++] = new Rtree{f};
    this->bound = MBB::merge(this->bound, f->bound);
    return true;
}

