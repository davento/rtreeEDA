#include "SStree.h"

SStree::SStree(){
    root = nullptr;
}

SStree::Node* SStree::search(const Point& p){
    return search(root, p);
}

void SStree::insert(const Point& p){
    root = insert(root, p);
}

void SStree::remove(const Point& p){
    remove(root, p);
}

void SStree::draw(SDL_Renderer* renderer) const{
    root->draw(renderer);
}

SStree::Node* SStree::search(Node* node, const Point &p){
    
    Node* result = root;
    if(result->isLeaf()){
        return result;
    }
    
    while(!result->isLeaf()){
        Node* temp = result->children[0];
        for(const auto& node: result->children){
                if(Point::distance(node->bound.getCentroid(), p)< Point::distance(temp->bound.getCentroid(), p)  )
                    temp = node;
        }
        result = temp;
    }
    return result;
}

SStree::Node* SStree::chooseSubtree(Node* node, const Point &p){
    
    double minMetric = 1e9+10;
    Node* result = nullptr;
    for(const auto& child: node->children){
        // boundType aux = Rtree<T,ORDER>::makeNewCombineBound(*(figure->getBound()), node->myBound);
        // double p = aux.metric();
        double temp = Point::distance(p, child->bound.getCentroid());
        if(temp <= minMetric){
            minMetric = temp;
            result = child;
        }
    }
    return result;
}

SStree::Node* SStree::insert(Node* node, const Point &p){
    
    if(root->isLeaf()){
        Node* it = new LeafNode(p);
        (root->children).push_back(it);
        root->mergeBounds();

        if((root->children).size() == ORDER + 1)
            handleOverflow(root);
    }
    else{
        Node* v = chooseSubtree(root, p);
        insert(v, p);
        root->mergeBounds();
    }
    if(root->father)
        return root->father;
    return root;
} 


void SStree::handleOverflow(Node* overFlowed){
    
    Node* v = new SStreeNode();
    split(overFlowed, v);
    if(!overFlowed->father){
        //Create new father
        Node* overFather = new SStreeNode();
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

void SStree::split(Node* original, Node* secondHalf){

    int axis = variance(original->children, original->bound, X) < 
                variance(original->children, original->bound, Y) ? X : Y;

      
    std::vector<Node*> regions = original->children;

    // //sort by x left
    std::sort(regions.begin(), regions.end(),
    [&](Node* m1,  Node* m2){
        return m1->bound.getCentroid()[axis] < m2->bound.getCentroid()[axis];
    });
    
    bestSplit(regions,original,secondHalf,axis);
}

void SStree::bestSplit(std::vector<Node*>& u, Node* v, Node* p, int axis){

    int m = u.size();

    std::vector<Node*> s1;
    std::vector<Node*> s2;

    MBC m1 = v->bound;
    MBC m2 = v->bound;


    for( int i = ceil(m * 0.4); i <= m - ceil(m * 0.4); i++){
        // s1 = first i regions (points)
        // s2 = the other i regions (points)
        s1 = {u.begin(), u.begin() + i};
        s2 = {u.begin()+i , u.end() };
        //get mbb and choose minimum
        MBC t1 = mergeBounds(s1);
        MBC t2 = mergeBounds(s2);

        
        if(variance(s1,t1,axis) + variance(s2,t2,axis) < variance(v->children, m1, axis) + variance(p->children, m2, axis)){
            m1 = t1; m2 = t2;
            v->bound = m1;
            p->bound = m2;
            v->children = s1;
            p->children = s2;
        }
    }
    v->mergeBounds();
    p->mergeBounds();
}

