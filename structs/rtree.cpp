#include "rtree.h"


void addChildrenToFather(RNode* root, RNode* nodeOverflowed, RNode* v){
    root->bound = nodeOverflowed->bound;
    nodeOverflowed->father = v->father = root;
    root->regions.push_back(nodeOverflowed);
    root->regions.push_back(v);
    root->bound = RNode::regionsMbb(root->regions);
}

template<class cnt>
void minimumPerimeter(cnt &u, RNode* v, RNode* p){

    int m = u.size();
    //using T = typename cnt::value_type;

    cnt s1;
    cnt s2;

    MBB m1 = v->bound;
    MBB m2 = v->bound;
    
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

    minimumPerimeter(figures,originalAndFirstHalf,secondHalf);
    
    //sort by x right
    sort(figures.begin(), figures.end(),
    [](const Figure* m1, const Figure* m2){
        return m1->getBound().bottomRight.x < m2->getBound().bottomRight.x;
    });
    minimumPerimeter(figures,originalAndFirstHalf,secondHalf);

    //same thing but with y left
    sort(figures.begin(), figures.end(),
    [](const Figure* m1, const Figure* m2){
        return m1->getBound().topLeft.y < m2->getBound().topLeft.y;
    });
    minimumPerimeter(figures,originalAndFirstHalf,secondHalf);

    //same thing but with y right
    sort(figures.begin(), figures.end(),
    [](const Figure* m1, const Figure* m2){
        return m1->getBound().bottomRight.y < m2->getBound().bottomRight.y;
    });
    minimumPerimeter(figures,originalAndFirstHalf,secondHalf);

}

void splitInternal(RNode* originalAndFirstHalf, RNode* secondHalf){
    
    std::vector<RNode*> regions = originalAndFirstHalf->regions;

    //sort by x left
    sort(regions.begin(), regions.end(),
    [](const RNode* m1, const RNode* m2){
        return m1->getBound().topLeft.x < m2->getBound().topLeft.x;
    });
    minimumPerimeter(regions,originalAndFirstHalf,secondHalf);
    
    //sort by x right
    sort(regions.begin(), regions.end(),
    [](const RNode* m1, const RNode* m2){
        return m1->getBound().bottomRight.x < m2->getBound().bottomRight.x;
    });
    minimumPerimeter(regions,originalAndFirstHalf,secondHalf);

    //same thing but with y left
    sort(regions.begin(), regions.end(),
    [](const RNode* m1, const RNode* m2){
        return m1->getBound().topLeft.y < m2->getBound().topLeft.y;
    });
    minimumPerimeter(regions,originalAndFirstHalf,secondHalf);

    //same thing but with y right
    sort(regions.begin(), regions.end(),
    [](const RNode* m1, const RNode* m2){
        return m1->getBound().bottomRight.y < m2->getBound().bottomRight.y;
    });
    minimumPerimeter(regions,originalAndFirstHalf,secondHalf);


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
    }
    else{
        RNode* w = nodeOverflowed->father;
        // update MBR(u) in w or whatever that means
        v->father = w;
        w->regions.push_back(v);
        w->bound = RNode::regionsMbb(w->regions);
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
        std::cout << minP << " vs. " << p << "\n";
        if(p <= minP) {
            std::cout << "gana: "<< p << "\n";
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
    }
    else{
        RNode* v = chooseSubtree(node, figure);
        insert(v, figure);
        node->bound = RNode::regionsMbb(node->regions);
    }
    if(node->father)
        return node->father;
    return node;    
}

template<class cnt>
MBB RNode::regionsMbb(cnt c){

    MBB res = c.front()->getBound();
    for(auto region: c){
        res = MBB::merge(res, region->getBound());
    }
    return res;
}

bool inArea(MBB b, Point p) {
    if(
        (p.x > b.topLeft.x && p.x < b.bottomRight.x) &&
        (p.y > b.topLeft.y && p.y < b.bottomRight.y)
    )
        return true;
    return false;
}

RNode* Rtree::search(RNode* n, Point p) {
    if (n->isLeaf()) {
        for(auto f : n->myFigures) {
            if (inArea(f->bound, p))
                return n;
        }
        return nullptr;
    }

    for (auto r : n->regions) {
        if (inArea(r->bound, p))
            return search(r, p);
    }
    return nullptr;
}

RNode* Rtree::search(Point p) {
    return search(root, p);
}

bool Rtree::insert(Figure *f){

    //insert
    root = ::insert(root, f);
    std::cout << "root: \n";
    root->print();
    return true;
}

void Rtree::erase(Point p) {
    // locate node with the figure
    RNode* n = search(p);
    
    // if not found, do nothing
    if (n == nullptr) return;

    // remove figure from figures list
    auto fun = [&p](Figure* f){
        return inArea(f->getBound(), p);
    };
    auto it = std::find_if(n->myFigures.begin(), n->myFigures.end(), fun);

    n->myFigures.erase(it);


    // if there aren't orphans, we are done
    if (n->myFigures.size() >= ceil(ORDER/2)) return;

    // else deal with orphans (recursively):
    // - send to nearest node
    // - split said node if necessary
    // - resize tree accordingly

}