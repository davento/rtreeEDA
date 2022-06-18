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
    if(!n)
        return ;
    auto fun = [&p](FigureNode<T,ORDER>* node){
        //TODO: CHECKEAR SI EXISTEN ESTOS METODOS Y LA FUNCION INAREA
        return inArea((node->getFigure())->getBound(), p);   
    };
    auto it = std::find_if(n->regions.begin(), n->regions.end(), fun);
    (n->regions).erase(it);
    //TODO: CREAR REGIONSB QUE ES REGIONSMBB SOLO QUE RENOMBRADO XQ AHORA PUEDEN
    //SER CIRCULOS
    n->myBound = Node<T,ORDER>::regionsB(n->regions);
    if(n->regions.size() >= std::ceil(ORDER/2) || n->father == nullptr) 
        return ;
    reinsert();
}

template <typename T, unsigned ORDER>
std::vector<Figure*> Rtree<T, ORDER>::depthFirst(const Point& p) const{
    constexpr int k = 5;
    auto func = [&p](Figure* f1, Figure* f2){
        
    }
}
