#include "rtree.h"


void addChildrenToFather(RNode* root, RNode* nodeOverflowed, RNode* v){
    root->bound = nodeOverflowed->bound;
    root->bound = MBB::merge(root->bound, v->bound);
    nodeOverflowed->father = v->father = root;
    root->regions.push_back(nodeOverflowed);
    root->regions.push_back(v);
}
template<class cnt>
void FminimumPerimeter(cnt &u, RNode* v, RNode* p){
    

    int m = u.size();
    //using T = typename cnt::value_type;

    cnt s1;
    cnt s2;

    MBB m1 = v->bound;
    MBB m2 = p->bound;
    
    for( int i = ceil(m * 0.4); i <= m - ceil(m * 0.4); i++){
        // s1 = first i regions (points)
        // s2 = the other i regions (points)
        s1 = {u.begin(), u.begin() + i};
        s2 = {u.begin()+i , u.end() };
        
        std::cout <<"tamaño de divisiones: " << s1.size() << " "<< s2.size() <<"\n";
        //get mbb and choose minimum
        auto t1 = RNode::regionsMbb(s1);
        auto t2 = RNode::regionsMbb(s2);        
        
        std::cout << t1.Perimeter() << " " << t2.Perimeter() << " < " << m1.Perimeter() << " " << m2.Perimeter() << "\n";
        if(t1.Perimeter() + t2.Perimeter() < m1.Perimeter() + m2.Perimeter()){
            m1 = t1; m2 = t2;
            v->update(s1,m1); p->update(s2,m2);
        }
    }
}

void splitLeaf(RNode* originalAndFirstHalf, RNode* secondHalf){
      //u anb u' resulting of the split    
    std::vector<Figure*> figures = originalAndFirstHalf->myFigures;
    //sort by x left
    sort(figures.begin(), figures.end(),
    [](const Figure* m1, const Figure* m2){
        return m1->getBound().topLeft.x < m2->getBound().topLeft.x;
    });

    FminimumPerimeter(figures,originalAndFirstHalf,secondHalf);
    
    //sort by x right
    sort(figures.begin(), figures.end(),
    [](const Figure* m1, const Figure* m2){
        return m1->getBound().bottomRight.x < m2->getBound().bottomRight.x;
    });
    FminimumPerimeter(figures,originalAndFirstHalf,secondHalf);

    //same thing but with y left
    sort(figures.begin(), figures.end(),
    [](const Figure* m1, const Figure* m2){
        return m1->getBound().topLeft.y < m2->getBound().topLeft.y;
    });
    FminimumPerimeter(figures,originalAndFirstHalf,secondHalf);

    //same thing but with y right
    sort(figures.begin(), figures.end(),
    [](const Figure* m1, const Figure* m2){
        return m1->getBound().bottomRight.y < m2->getBound().bottomRight.y;
    });
    FminimumPerimeter(figures,originalAndFirstHalf,secondHalf);


}

void splitInternal(RNode* originalAndFirstHalf, RNode* secondHalf){
    //sort by x left
    std::vector<RNode*> regions = originalAndFirstHalf->regions;
    sort(regions.begin(), regions.end(),
    [](const RNode* m1, const RNode* m2){
        return m1->getBound().topLeft.x < m2->getBound().topLeft.x;
    });

    FminimumPerimeter(regions,originalAndFirstHalf,secondHalf);
    //sort by x right
    sort(regions.begin(), regions.end(),
    [](const RNode* m1, const RNode* m2){
        return m1->getBound().bottomRight.x < m2->getBound().bottomRight.x;
    });
    FminimumPerimeter(regions,originalAndFirstHalf,secondHalf);

    //same thing but with y left
    sort(regions.begin(), regions.end(),
    [](const RNode* m1, const RNode* m2){
        return m1->getBound().topLeft.y < m2->getBound().topLeft.y;
    });
    FminimumPerimeter(regions,originalAndFirstHalf,secondHalf);

    //same thing but with y right
    sort(regions.begin(), regions.end(),
    [](const RNode* m1, const RNode* m2){
        return m1->getBound().bottomRight.y < m2->getBound().bottomRight.y;
    });
    FminimumPerimeter(regions,originalAndFirstHalf,secondHalf);


}

void split(RNode* originalAndFirstHalf, RNode* secondHalf){
    if(originalAndFirstHalf->isLeaf())
        splitLeaf(originalAndFirstHalf, secondHalf);
    else
        splitInternal(originalAndFirstHalf, secondHalf);
}

void handleOverflow(RNode* nodeOverflowed){
    RNode* v = new RNode;
    split(nodeOverflowed, v);
    if(!nodeOverflowed->father){
        RNode* root = new RNode;
        addChildrenToFather(root, nodeOverflowed, v);
        if(nodeOverflowed->father)
            std::cout << "i have a father\n";
        std::cout << nodeOverflowed->myFigures.size() << " " << nodeOverflowed->regions.size() << "\n";
        std::cout << v->myFigures.size() << " " << v->regions.size() << "\n";
    }
    else{
        RNode* w = nodeOverflowed->father;
        // update MBR(u) in w or whatever that means
        v->father = w;
        w->regions.push_back(v);
        if(w->regions.size() == ORDER + 1)
            handleOverflow(w);
    }
}

RNode* chooseSubtree(RNode* root, Figure* figure){

    int minP = INF;
    RNode* result = root;
    //choose region with minimum perimeter
    for(auto region : root->regions) {
        MBB aux = MBB::merge(figure->getBound(), region->bound);
        
        int p = aux.Perimeter();

        if(p <= minP) {
            minP = p;
            result = region;
        }
        
    }
    return result;
 
}

RNode* insert(RNode* node, Figure* figure){
    if(node->isLeaf()){
        node->myFigures.push_back(new Figure(*figure));
        node->bound = MBB::merge(node->bound, figure->getBound());
        if(node->myFigures.size() == ORDER + 1)
            handleOverflow(node);
        if(node->father)
            std::cout << "indeed\n";
    }
    else{
        RNode* v = chooseSubtree(node, figure);
        insert(v, figure);
        node->bound = MBB::merge(node->bound, v->bound);
    }
    if(node->father)
        return node->father;
    return node;    
}

//gets the node that
RNode* RNode::chooseSubtree(Figure* f){

    std::cout << "choosing son \n";
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
        ++pos;
        
    }
    std::cout << "son choosed: " << pos << "\n"; 
    return regions[minpos];
}

template<class cnt>
void  RNode::minimumPerimeter(cnt &u, RNode* v, RNode* p){
    

    int m = u.size();

    cnt s1;
    cnt s2;

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
        
        std::cout << t1.Perimeter() << " " << t2.Perimeter() << " < " << m1.Perimeter() << " " << m2.Perimeter() << "\n";
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

    using T = typename cnt::value_type;
    
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

    MBB res = c.front()->getBound();
    for(auto region: c){
        res = MBB::merge(res, region->getBound());
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
    
    if(!this->father){
        std::cout << " creating new root";
        RNode* root = new RNode;
        root->regions.push_back(u);
        root->regions.push_back(v);
        u->father = v->father = root;
        root->bound = MBB::merge(u->bound, v->bound);
        father = root;
    }
    else{
        RNode* w = this->father;
        w->bound = MBB::merge(w->bound, v->bound);
        w->regions.push_back(v);
        u->father = v->father = w;
        if(w->regions.size() > ORDER){
            w->handleOverflow(f);
        }
    }
}

bool RNode::insert(Figure *f){
    std::cout << "insertando... \n";
    if(!this->isLeaf()){
        std::cout << "no soy hoja \n";
        RNode *node = this->chooseSubtree(f);
        return node->insert(f);
    }

    std::cout << "soy hoja\n";
    
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
    if(father){
        father->bound = MBB::merge(father->bound, bound);
    }
    //this->myFigure = f;

    return true;

}




RNode* Rtree::search(Figure* f){
    //TODO: search
    return root->search(f);
}

bool Rtree::insert(Figure *f){

    //insert
    root = ::insert(root, f);
    std::cout << root->father <<std::endl;
    std::cout << root->bound.Perimeter() <<std::endl;
    std::cout << "hijos: "<< root->regions.size() << " \n a\n";
    for(auto region: root->regions)
        std::cout<< region->myFigures.size() <<" ";
    std::cout << "a \n";
    return true;
}
