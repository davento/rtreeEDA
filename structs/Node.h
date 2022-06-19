#ifndef NODE_H
#define NODE_H

#include "Bound.h"
#include "Color.h"

template <typename, unsigned>
class Rtree;

template <typename T, unsigned ORDER>
class Node{
    public:
        typedef T boundType;
        friend class Rtree<T,ORDER>;

        Node();
        virtual ~Node();

        virtual inline bool isLeaf() const = 0;
        //TODO: Agregar el color
        virtual void draw(SDL_Renderer* renderer, Color color) const = 0;
        //added for minimumPerimiter
        
    protected:
        Node* father;
        boundType* myBound;
        static constexpr unsigned order = ORDER;
};

#endif
