#include "rtree.h"





//gets the node that
RNode* RNode::chooseSubtree(Figure* f){

    //if node is a figure node
    //return to region node (figure->father)
    if(this->isLeaf()) return this;

    MBB res;
    int minP = INF;
    int minpos = 0;
    int pos = 0;
    //choose region with minimum perimeter
    for(auto region : regions) {
        MBB aux = MBB::merge(f->getBound(), region->bound);
        
        int p = aux.Perimeter();

        if(p <= minP) {
            minP = p;
            res = aux;
            minpos = pos;
        }
        pos++;
    }

    return regions[pos];
}

template<class cnt>
void  RNode::minimumPerimeter(cnt &u, RNode* v, RNode* p){
    

    int m = u.size();
    using T = typename cnt::value_type();

    cnt<T> s1;
    cnt<T> s2;

    MBB m1 = v->bound;
    MBB m2 = p->bound;
    
    for( int i = ceil(m * 0.4); i <= m - ceil(m * 0.4); i++){
        // s1 = first i regions (points)
        // s2 = the other i regions (points)
        s1 = {u.begin(), u.begin() + i};
        s2 = {u.begin()+i , u.end() };
        
        //get mbb and choose minimum
        auto t1 = RNode::regionsMbb(s1);
        auto t2 = RNode::regionsMbb(s2);        
        
        std::cout << t1.Perimeter() << " " << t2.Perimeter() << "\n";
        if(t1.Perimeter() + t2.Perimeter() < m1.Perimeter() + m2.Perimeter()){
            m1 = t1; m2 = t2;
            v->update(s1,m1); p->update(s2,m2);
        }
    }
}

template<class cnt>
std::pair<RNode*, RNode*> RNode::split(cnt &u){
    

    //u anb u' resulting of the split
    RNode* v = new RNode;
    RNode* p = new RNode;

    using T = typename cnt::value_type();
    
    //sort by x left
    sort(u.begin(), u.end(),
    [](const T m1, const T m2){
        return m1->getBound().topLeft.x < m2->getBound().topLeft.x;
    });

    minimumPerimeter(u,v,p);
    //sort by x right
    sort(u.begin(), u.end(),
    [](const T m1, const T m2){
        return m1->getBound().bottomRight.x < m2->getBound().bottomRight.x;
    });
    minimumPerimeter(u,v,p);

    //same thing but with y left
    sort(u.begin(), u.end(),
    [](const T m1, const T m2){
        return m1->getBound().topLeft.y < m2->getBound().topLeft.y;
    });
    minimumPerimeter(u,v,p);

    //same thing but with y right
    sort(u.begin(), u.end(),
    [](const T m1, const T m2){
        return m1->getBound().bottomRight.y < m2->getBound().bottomRight.y;
    });
    minimumPerimeter(u,v,p);

    return {v,p};
}

template<class cnt>
MBB RNode::regionsMbb( cnt c){

    MBB res = c.front()->bound;
    for(auto region: c){
        res = MBB::merge(res, region->bound);
    }
    return res;
}


void RNode::handleOverflow(Figure *f){

    RNode* u, *v;
    if(this->isLeaf()){
        auto p = split(this->myFigures);
        u = p.first;
        v = p.second;
    }
    else{
        auto p = split(this->regions);
        u = p.first;
        v = p.second;
    }
    
    if(!u->father){
        RNode* root = new RNode;
        root->regions.push_back(u);
        root->regions.push_back(v);
        root->bound = MBB::merge(u->bound, v->bound);
        father = root;
    }
    else{
        RNode* w = u->father;
        w->bound = MBB::merge(w->bound, v->bound);
        w->regions.push_back(v);
        if(w->regions.size() > ORDER){
            w->handleOverflow(f);
        }
    }
}

bool RNode::insert(Figure *f){

    if(!this->isLeaf()){
        return this->chooseSubtree(f)->insert(f);
    }

    
    //if overflow
    if(myFigures.size() == ORDER){
        
        myFigures.push_back(new Figure(*f));
        ++cur_figs;        
        this->handleOverflow(f);
        
        return true;
    }

    //else insert
    //this->regions.push_back(new Rtree{f})
    myFigures.push_back(new Figure(*f));
    ++cur_figs;
    this->bound = MBB::merge(this->bound, f->getBound());
    //this->myFigure = f;
    return true;

}






RNode* Rtree::search(Figure* f){
    //TODO: search
    return root->search(f);
}

bool Rtree::insert(Figure *f){

    //insert
    root->insert(f);

    if(root->father != nullptr) root = root->father;

    return true;
}
