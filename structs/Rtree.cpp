#include "Rtree.h"
#include "InternalNode.h"
#include "FigureNode.h"
#include "Color.h"
#include <algorithm>
#include <cmath>

template <typename T, unsigned ORDER>
Rtree<T, ORDER>::Rtree(): root(new InternalNode<T,ORDER>()){

}

template <typename T, unsigned ORDER>
Rtree<T, ORDER>::~Rtree(){
    delete root;
    root = nullptr;
}

template <typename T, unsigned ORDER>
InternalNode<T,ORDER>* Rtree<T,ORDER>::search(const Point& p){
    return search(root, p);
}

template <typename T, unsigned ORDER>
bool Rtree<T,ORDER>::insert(Figure* fig){
    root = insert(root, fig);
    return true;
}

template <typename T, unsigned ORDER>
void Rtree<T,ORDER>::remove(const Point& p){
    InternalNode<T, ORDER>* n = search(p);
    if(n == nullptr || !n->isLeaf() ) return ;
    
    auto fun = [&p](FigureNode<T,ORDER>* node){
        return  (node->getFigure()->getBound())->inArea(p);
    };
    auto it = std::find_if(n->regions.begin(), n->regions.end(), fun);
    (n->regions).erase(it);
    n->myBound = Node<T,ORDER>::regionsB(n->regions);
    if(n->regions.size() >= std::ceil(ORDER/2) || n->father == nullptr) 
        return ;

    //TODO: Reinsert
    // reinsert();
}

template <typename T, unsigned ORDER>
void Rtree<T,ORDER>::draw(SDL_Renderer* renderer) const{
    Color color(0,40,0);
    root->draw(renderer, color);
}

// template <typename T, unsigned ORDER>
// void Rtree<T,ORDER>::reinsert(SDL_Renderer* renderer) const{
//     Color color(0,40,0);
//     root->draw(renderer, color);
// }

template <typename T, unsigned ORDER>
InternalNode<T,ORDER>* search(InternalNode<T,ORDER>* node, const Point& p){

    if(node->isLeaf()){
        for(FigureNode<T,ORDER>* f: node->regions){
            
            if(f->getFigure()->getBound()->inArea(p)){
                return node;
            }
        }
    }

    for(InternalNode<T,ORDER>* r: node->regions){
        if(r->myBound->inArea(p)){
            InternalNode<T,ORDER>* res = search(r,p);
            if(res != nullptr) return res;
        }
    }
    return nullptr;
}


template<typename T, unsigned ORDER>
T Rtree<T,ORDER>::mergeRegions(std::vector<Node<T,ORDER>*>& nodes){
    
    T res  = nodes.front()->myBound;
    for(InternalNode<T,ORDER>* r: nodes){
        res->merge(r->myBound);
    }

    return res;
}

template <typename T, unsigned ORDER>
void Rtree<T,ORDER>::minimumPerimeter(std::vector<Node<T,ORDER>*>& u, InternalNode<T,ORDER>* v, InternalNode<T,ORDER>* p){
    
    int m = u.size();

    std::vector<Node<T,ORDER>*> s1;
    std::vector<Node<T,ORDER>*> s2;

    boundType m1 = v->bound;
    boundType m2 = v->bound;


    for( int i = ceil(m * 0.4); i <= m - ceil(m * 0.4); i++){
        // s1 = first i regions (points)
        // s2 = the other i regions (points)
        s1 = {u.begin(), u.begin() + i};
        s2 = {u.begin()+i , u.end() };
        
        //get mbb and choose minimum
        T t1 = mergeRegions(s1);
        T t2 = mergeRegions(s2);        
        
        if(t1.metric() + t2.metric() < m1.metric() + m2.metric()){
            m1 = t1; m2 = t2;

            *(v->myBound) = m1; v->regions = s1;
            *(p->myBound) = m2; p->regions = s2;
        }
    }
}


template <typename T, unsigned ORDER>
void addChildrenToFather(
    InternalNode<T,ORDER>* root, 
    InternalNode<T,ORDER>* nodeOverflowed, InternalNode<T,ORDER>* v){

    root->myBound = nodeOverflowed->myBound;
    nodeOverflowed->father = v->father = root;
    root->regions.push_back(nodeOverflowed);
    root->regions.push_back(v);
    root->myBound  = mergeRegions(root->regions);
}

template <typename T, unsigned ORDER>
void Rtree<T,ORDER>::handleOverflow(InternalNode<T,ORDER>* overFlowed){
    InternalNode<T,ORDER> * v= new InternalNode<T,ORDER> ();
    split(overFlowed, v);
    if(overFlowed->father){
        InternalNode<T,ORDER>* root = new InternalNode<T,ORDER>();
        //addChildrenToFather(root, nodeOverflowed, v);
    }
    else{
        InternalNode<T,ORDER> * w = overFlowed->father;
        // update MBR(u) in w or whatever that means
        v->father = w;
        w->regions.push_back(v);
        w->bound = mergeRegions(w->regions);
        if(w->regions.size() == ORDER + 1)
            handleOverflow(w);
    }
}
template <typename T, unsigned ORDER>
void Rtree<T,ORDER>::split(InternalNode<T,ORDER>* original, InternalNode<T,ORDER>* secondHalf){
        
    std::vector<Node<T,ORDER>*> regions = original->regions;

    //sort by x left
    sort(regions.begin(), regions.end(),
    [](const Node<T,ORDER>* m1, const Node<T,ORDER>* m2){
        return m1->getBound()->getTopLeft().x < m2->getBound()->getTopLeft().x;
    });
    minimumPerimeter(regions,original,secondHalf);
    
    //sort by x right  
    sort(regions.begin(), regions.end(),
    [](const Node<T,ORDER>* m1, const Node<T,ORDER>* m2){
        return m1->getBound()->getBottomRight().x < m2->getBound()->getBottomRight().x;
    });
    minimumPerimeter(regions,original,secondHalf);

    //same thing but with y left
    sort(regions.begin(), regions.end(),
    [](const Node<T,ORDER>* m1, const Node<T,ORDER>* m2){
        return m1->getBound()->getTopLeft().y < m2->getBound()->getTopLeft().y;
    });
    minimumPerimeter(regions,original,secondHalf);

    //same thing but with y right
    sort(regions.begin(), regions.end(),
    [](const Node<T,ORDER>* m1, const Node<T,ORDER>* m2){
        return m1->getBound()->getBottomRight().y < m2->getBound()->getBottomRight().y;
    });
    minimumPerimeter(regions,original,secondHalf);
}

template <typename T, unsigned ORDER>
InternalNode<T,ORDER>* Rtree<T,ORDER>::insert(InternalNode<T,ORDER>* node, Figure* figure){
    if(node->isLeaf()){
        node->regions.push_back(new FigureNode<T,ORDER>(figure));
        *(node->myBound) = mergeRegions(node->regions);
        if(node->regions.size() == ORDER + 1)
            handleOverflow(node);
    }
    else{
        InternalNode<T,ORDER>* v = chooseSubtree(node, figure);
        insert(v, figure);
        *(node->myBound) = mergeRegions(node->regions);
    }
    if(node->father)
        return node->father;
    return node;
}
