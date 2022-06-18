#include "Rtree.h"
#include "InternalNode.h"
#include "FigureNode.h"
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

