#include "rtree.h"



Rtree::Rtree(Figure* f):myFigure(f){
    
}


Rtree* Rtree::search(Figure* f){
    //TODO: search
    return {};
}


inline bool Rtree::isLeaf(){
    return this->regions[0]->myFigure != nullptr;
}

//gets the node that
Rtree *Rtree::chooseSubtree(Figure* f){

    //if node is a figure node
    //return to region node (figure->father)
    if(this->isLeaf()) return this;

    MBB res ;

    int minP = INF;
    int minpos = 0;
    int pos = 0;
    //choose region with minimum perimeter
    for(auto region : regions) {
        MBB aux = MBB::merge(f->bound, region->bound);
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

std::pair<Rtree*, Rtree*> Rtree::split(Rtree* u){
    int m = u->cur_figs;
    
    //sort by x
    sort(u->regions.begin(), u->regions.end(),
    [](const Rtree* m1, const Rtree* m2){
        return m1->myFigure->bound.topLeft.x < m2->myFigure->bound.topLeft.x;
    });


    //u anb u' resulting of the split
    Rtree* v = new Rtree;
    Rtree* p = new Rtree;

    std::vector<Rtree*> s1;
    std::vector<Rtree*> s2;

    MBB m1;
    MBB m2;
    
    for( int i = ceil(m * 0.4); i < m - ceil(m * 0.4); i++){
        // s1 = first i regions (points)
        // s2 = the other i regions (points)
        s1 = {u->regions.begin(), u->regions.begin() + i};
        s2 = {u->regions.begin()+i , u->regions.end() };

        //get mbb and choose minimum
        auto t1 = MBB::regionsMbb(s1);
        auto t2 = MBB::regionsMbb(s2);

        if(t1.Perimeter() < m1.Perimeter()) {m1 = t1; v->update(s1,m1);}
        if(t2.Perimeter() < m2.Perimeter()) {m1 = t1; p->update(s2,m2);}
    }

    //same thing but with y
    sort(u->regions.begin(), u->regions.end(),
    [](const Rtree* m1, const Rtree* m2){
        return m1->myFigure->bound.topLeft.y < m2->myFigure->bound.topLeft.y;
    });


   for( int i = ceil(m * 0.4); i < m - ceil(m * 0.4); i++){
        // s1 = first i regions (points)
        // s2 = the other i regions (points)
        s1 = {u->regions.begin(), u->regions.begin() + i};
        s2 = {u->regions.begin()+i , u->regions.end() };

        //get mbb and choose minimum
        auto t1 = MBB::regionsMbb(s1);
        auto t2 = MBB::regionsMbb(s2);

        if(t1.Perimeter() < m1.Perimeter()) {m1 = t1; v->update(s1,m1);}
        if(t2.Perimeter() < m2.Perimeter()) {m1 = t1; p->update(s2,m2);}
    }

    return {v,p};
}


void Rtree::handleOverflow(Figure *f){

    Rtree* u, *v;
    auto pr = Rtree::split(this);
    u = pr.first;
    v = pr.second;

}

bool Rtree::insert(Figure *f){

    if(!this->isLeaf()){
        return this->chooseSubtree(f)->insert(f);
    }

    Rtree* cur = this;
    
    //if overflow
    if(cur->cur_figs >= ORDER){
        
        this->regions.push_back(new Rtree{f});
        this->cur_figs++;        
        this->handleOverflow(f);
        
        return true;
    }

    //else insert
    this->regions.push_back(new Rtree{f});
    this->cur_figs++;
    this->bound = MBB::merge(this->bound, f->bound);
    return true;
}

