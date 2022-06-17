#ifndef RTREE_H
#define RTREE_H

#include "Node.h"
#include "Figure.h"

template<typename T, unsigned ORDER>
class Rtree{
    public:
        typedef T boundType;
        Rtree();
        ~Rtree();
        //TODO: chequear si podemos agregar const al retorno
        Node<T,ORDER>* search(const Point&);
        bool insert(Figure*);
        void remove(const Point&);
        std::vector<Figure*> depthFirst(const Point&);
        void draw(SDL_Renderer* renderer) const;
        
    private:
        Node<T,ORDER>* root;
        void reinsert();
        Node<T,ORDER>* search(Node<T,ORDER>*, const Point&);
};

#endif
