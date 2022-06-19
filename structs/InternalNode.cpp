#include "InternalNode.h"

template <typename T, unsigned ORDER>
InternalNode<T,ORDER>::InternalNode(){
    regions.reserve(ORDER + 1);
}


template <typename T, unsigned ORDER>
inline bool InternalNode<T,ORDER>::isLeaf() const{
   if(!regions.empty())
       return regions[0]->isLeaf();
   else 
       return true;
}

template <typename T, unsigned ORDER>
void InternalNode<T,ORDER>::draw(SDL_Renderer* renderer, Color color) const { 
    Node<T,ORDER>::myBound->draw(renderer, color);
    color.changeColor(150);
    for(const auto& region: regions){
        region->draw(renderer);
    }
}
