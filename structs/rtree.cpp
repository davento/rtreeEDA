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

std::pair<RNode*, RNode*> RNode::split(RNode* u){
    int m = u->myFigures.size();
    
    //sort by x
    sort(u->myFigures.begin(), u->myFigures.end(),
    [](const Figure* m1, const Figure* m2){
        return m1->getBound().topLeft.x < m2->getBound().topLeft.x;
    });


    //u anb u' resulting of the split
    RNode* v = new RNode;
    RNode* p = new RNode;

    std::vector<Figure*> s1;
    std::vector<Figure*> s2;

    MBB m1;
    MBB m2;
    
    for( int i = ceil(m * 0.4); i <= m - ceil(m * 0.4); i++){
        // s1 = first i regions (points)
        // s2 = the other i regions (points)
        std::cout << "jean paul\n";
        // f1 f2     f3 f4
        s1 = {u->myFigures.begin(), u->myFigures.begin() + i};
        s2 = {u->myFigures.begin()+i , u->myFigures.end() };
        std::cout << "huby tuesta\n";
        //get mbb and choose minimum
        auto t1 = RNode::regionsMbb(s1);
        auto t2 = RNode::regionsMbb(s2);
        
        
        std::cout << t1.Perimeter() << " " << t2.Perimeter() << "\n";
        if(t1.Perimeter() < m1.Perimeter()) {m1 = t1; v->update(s1,m1);}
        if(t2.Perimeter() < m2.Perimeter()) {m2 = t2; p->update(s2,m2);}
    }

    //same thing but with y
    sort(u->myFigures.begin(), u->myFigures.end(),
    [](const Figure* m1, const Figure* m2){
        return m1->getBound().topLeft.y < m2->getBound().topLeft.y;
    });


   for( int i = ceil(m * 0.4); i <= m - ceil(m * 0.4); i++){
        // s1 = first i regions (points)
        // s2 = the other i regions (points)
        s1 = {u->myFigures.begin(), u->myFigures.begin() + i};
        s2 = {u->myFigures.begin()+i , u->myFigures.end() };
        
        //get mbb and choose minimum
        //
        MBB t1 = RNode::regionsMbb(s1);
        MBB t2 = RNode::regionsMbb(s2);

        if(t1.Perimeter() < m1.Perimeter()){ 
            m1 = t1; 
            v->update(s1,m1);
        }

        if(t2.Perimeter() < m2.Perimeter()){ 
            m2 = t2; 
            p->update(s2,m2);
        }
    }
    std::cout << m1.Perimeter() << " " << m2.Perimeter() << "\n";
    return {v,p};
}


MBB RNode::regionsMbb( std::vector<RNode*> regions){
    MBB res = regions[0]->bound;
    for(auto region: regions){
        res = MBB::merge(res, region->bound);
    }
    return res;
}
MBB RNode::regionsMbb( std::vector<Figure*> figures){
    MBB res = figures[0]->getBound();
    for(auto figure: figures){
        res = MBB::merge(res, figure->getBound());
    }
    return res;
}

void RNode::handleOverflow(Figure *f){

    RNode* u, *v;
    auto pr = RNode::split(this);
    u = pr.first;
    v = pr.second;
    
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
