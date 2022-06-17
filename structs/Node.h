#ifndef NODE_H
#define NODE_H

#include "Bound.h"

template <typename T, unsigned ORDER>
class Node{
    public:
        typedef T boundType;

        Node(Bound*);
        virtual ~Node();

        virtual inline bool isLeaf() const = 0;
        //TODO: Agregar el color
        virtual void draw(SDL_Renderer* renderer) const = 0;
    protected:
        Node* father;
        Bound* myBound;
        static constexpr unsigned order = ORDER;
};

#endif
