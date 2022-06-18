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
