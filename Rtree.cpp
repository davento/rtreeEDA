#include "Rtree.h"
#include "RtreeNode.h"

Rtree::Rtree(){
    root = new Node();
}
Rtree::Rtree(int dim){
    root = new Node(MBC(Point(0,0), Point(dim, dim)));
}

Rtree::Node* Rtree::search(const Point& p){
    return search(root, p);
}

void Rtree::insert(const Figure& f){
    root = insert(root, f);
    // std::cout<<"Finish insert\n";
    // std::cout<<"----------------------------------------------\n";
    // print();
}

void Rtree::remove(const Point& p){
    // remove(root, p);
}

void Rtree::draw(SDL_Renderer* renderer) const{
    // std::cout<<"A\n";
    //RGB -> BGR
    root->draw(renderer, Color(0,40,0));
}


Rtree::Node* Rtree::search(Node* node, const Point &p){
    
    if (node->isLeaf()) {

        return node;
    }

    for (auto r : node->children) {
        if (r->bound.inArea(p)){
            auto res = search(r, p);
            if(res != nullptr) return res;
        }
    }
    return nullptr;
}

Rtree::Node* Rtree::chooseSubtree(Node* node, const Figure &f){
        
    int minP = INF;
    Node* result = node;
    //choose region with minimum perimeter
    for(auto region : node->children) {
        Bound aux( f.getBound() );
        aux.merge(region->bound);
        
        int p = aux.perimeter();
        if(p <= minP) {
            minP = p;
            result = region;
        }
        
    }
    return result;
}

Rtree::Node* Rtree::insert(Node* node, const Figure &f){
    
    // std::cout<<"entering insert\n";
    // printf("inserting Point(%f,%f)\n", p.x,p.y);
    
    if(node->isLeaf()){
        // std::cout<<"inserting in leaf\n";
        
        Node* it = new LeafNode(f);
        (node->children).push_back(it);
        node->mergeBounds();
        if((node->children).size() == ORDER + 1){
            // std::cout<<"overflow\n";
            handleOverflow(node);
        }
    }
    else{
        Node* v = chooseSubtree(node, f);
        insert(v, f);
        node->mergeBounds();
    }
    if(node->father)
        return node->father;
    return node;
} 


void Rtree::handleOverflow(Node* overFlowed){
    
    Node* v = new Node();
    split(overFlowed, v);
    if(!overFlowed->father){
        //Create new father
        Node* overFather = new Node();
        //Add children to father
        overFlowed->father = v->father = overFather;
        (overFather->children).push_back(overFlowed);
        (overFather->children).push_back(v);
        overFather->mergeBounds();
    }
    else{
        Node* w = overFlowed->father;
        v->father = w;
        (w->children).push_back(v);
        w->mergeBounds();
        if((w->children).size() == ORDER + 1)
            handleOverflow(w);
    }
}

void Rtree::split(Node* original, Node* secondHalf){

    std::vector<Node*> regions = original->children;

    //sort by x left
    sort(regions.begin(), regions.end(),
    [](const Node* m1, const Node* m2){
        return m1->getBound().topLeft.x < m2->getBound().topLeft.x;
    });
    bestSplit(regions,original,secondHalf);

    //sort by x right
    sort(regions.begin(), regions.end(),
    [](const Node* m1, const Node* m2){
        return m1->getBound().bottomRight.x < m2->getBound().bottomRight.x;
    });
    bestSplit(regions,original,secondHalf);

    //sort by y left
    sort(regions.begin(), regions.end(),
    [](const Node* m1, const Node* m2){
        return m1->getBound().topLeft.y < m2->getBound().topLeft.y;
    });
    bestSplit(regions,original,secondHalf);

    //sort by y right
    sort(regions.begin(), regions.end(),
    [](const Node* m1, const Node* m2){
        return m1->getBound().bottomRight.y < m2->getBound().bottomRight.y;
    });
    bestSplit(regions,original,secondHalf);
}

void Rtree::bestSplit(std::vector<Node*>& u, Node* v, Node* p){

    int m = u.size();

    std::vector<Node*> s1;
    std::vector<Node*> s2;

    MBC m1 = v->bound;
    MBC m2 = v->bound;
    double minVariance = 2e5;
    for( int i = ceil(m * 0.4); i <= m - ceil(m * 0.4); i++){
        // s1 = first i regions (points)
        // s2 = the other i regions (points)
        s1 = {u.begin(), u.begin() + i};
        s2 = {u.begin()+i , u.end() };
        //get mbb and choose minimum
        MBC t1 = Node::mergeBounds(s1);
        MBC t2 = Node::mergeBounds(s2);
        
        if(t1.metric() + t2.metric() <  m1.perimeter() + m2.perimeter()){
            m1 = t1; m2 = t2;
            v->bound = m1;
            p->bound = m2;
            v->children = s1;
            p->children = s2;
            minVariance = t1.metric() + t2.metric();
        }
    }
    v->mergeBounds();
    p->mergeBounds();
}


void Rtree::print() const{
    root->print();
}

// void Rtree::remove(Node* node,const Point& p){

//    Node* leaf = search(p);
//    auto it = leaf->children.begin(); 

//    for(; it != leaf->children.end(); ++it)
//        if(Point::closeEnough((*it)->bound.getCentroid(), p,5))
//            break;

//    if(it == leaf->children.end()){
//         return ;
//    }
//    leaf->children.erase(it);
//    mergeUp(leaf);

//    if(leaf->children.size() <= ORDER/2){
//         std::cout<<"entering\n";
//        if(leaf->father){
//            Node* S = findClosestNodeM(leaf->father, leaf);
//            if(S) printf("closes node Point(%d,%d)\n", S->bound.getCentroid()[X], S->bound.getCentroid()[Y]);
//            if(S){
//                auto sit = S->children.begin();
//                decltype(sit) minDist = sit;
//                for(; sit != S->children.end(); ++sit)
//                    if(Point::distance( (*sit)->bound.getCentroid(),p) < Point::distance( (*minDist)->bound.getCentroid(), p) )
//                        minDist = sit;
               
//                insert(leaf, static_cast<LeafNode*>(*minDist)->getPoint());
//                S->children.erase(minDist);
//                mergeUp(leaf);
//                mergeUp(S);
               
//            }
//            else{
//                 std::cout<<"else\n";
//                S = closestNode(leaf->father, leaf);
//                for(auto point: leaf->children){
//                     insert(S, static_cast<LeafNode*>(point)->getPoint());
//                }
//                leaf->children.clear();
//                auto it = leaf->father->children.begin();
//                for(; it != leaf->father->children.end(); ++it){
//                     if(*it == leaf){
//                         break;
//                     }
//                }
//                leaf->father->children.erase(it);
               
//                mergeUp(S);
//            }
//        }
//    }
// }
