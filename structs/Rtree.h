#ifndef RTREE_H
#define RTREE_H

#include "Node.h"
#include "Figure.h"
#include "InternalNode.h"

template<typename T , unsigned ORDER>
class Rtree{
    public:
        typedef T boundType;
        Rtree();
        ~Rtree();
        //TODO: chequear si podemos agregar const al retorno
        InternalNode<T,ORDER>* search(const Point&);
        bool insert(Figure*);
        void remove(const Point&);
        // std::vector<Figure*> depthFirst(const Point&);
        void draw(SDL_Renderer* renderer) const;
        
    private:
        InternalNode<T,ORDER>* root;
        // void reinsert();
        InternalNode<T,ORDER>* search(InternalNode<T,ORDER>*, const Point&);
        void split(InternalNode<T,ORDER>* original, InternalNode<T,ORDER>* secondHalf);
        void minimumPerimeter(std::vector<Node<T,ORDER>*>& u, InternalNode<T,ORDER>* v, InternalNode<T,ORDER>* p);
        void handleOverflow(InternalNode<T,ORDER>* overFlowed);

        InternalNode<T,ORDER>* insert(InternalNode<T,ORDER>* , Figure*);
        //maybe some error with  declaration
        T mergeRegions(std::vector<Node<T,ORDER>*>&);
        void addChildrenToFather(
            InternalNode<T,ORDER>* root, InternalNode<T,ORDER>* nodeOverflowed, InternalNode<T,ORDER>* v
        );
};

#endif
