#ifndef INTERNALNODE_H
#define INTERNALNODE_H

#include "Node.h"
#include <vector>

template <typename T, unsigned ORDER>
class InternalNode final: public Node<T,ORDER>{
    public:
        InternalNode();
        ~InternalNode();
        void draw(SDL_Renderer* renderer, Color color) const override;
};

template <typename T, unsigned ORDER>
InternalNode<T,ORDER>::InternalNode(): Node<T,ORDER>(){

}

template <typename T, unsigned ORDER>
InternalNode<T,ORDER>::~InternalNode(){
    for(auto& child: this->children)
        delete child;
}

template <typename T, unsigned ORDER>
void InternalNode<T,ORDER>::draw(SDL_Renderer* renderer, Color color) const {
    this->myBound.draw(renderer, color);
    color.changeColor(150);
    for(const auto& child: this->children){
        child->draw(renderer, color);
    }
}



#endif

