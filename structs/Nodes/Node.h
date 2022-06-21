#ifndef NODE_H
#define NODE_H

#include "../Boundings/Bound.h"
#include "../Color.h"
#include <vector>
#include <iostream>

template <typename, unsigned>
class Rtree;

template <typename T, unsigned ORDER>
class Node{
    public:
        typedef T boundType;
        friend class Rtree<T,ORDER>;

        Node();
        virtual ~Node() = default;

        virtual bool isLeaf() const;
        virtual void draw(SDL_Renderer* renderer, Color color = Color(0,0,255)) const = 0;
        void mergeRegions();
  void print() const {
    std::cout << "----------\n";
    std::cout << "number of regions: "<< children.size() << "\n";
    std::cout << "perimeter: " << myBound.perimeter() << "\n";
    std::cout << "----------\n";
    if(children.size()){
      std::cout << "children: {\n";
      for(const auto &region: children)
        region->print();
      std::cout << "}\n";
    }
  }

    protected:
        Node<T,ORDER>* father;
        boundType myBound;
        static constexpr unsigned order = ORDER;
        std::vector<Node<T,ORDER>*> children;
};


template <typename T, unsigned ORDER>
Node<T,ORDER>::Node(): father(nullptr), myBound(){
    children.reserve(ORDER + 1);
}

template <typename T, unsigned ORDER>
bool Node<T,ORDER>::isLeaf() const{
    if(!children.empty() && children[0]->children.empty())
      return true;
    else if(children.empty())
      return true;
    return false;
}

template <typename T, unsigned ORDER>
void Node<T,ORDER>::mergeRegions(){
    for(const auto& child : children){
        myBound.merge(child->myBound);
    }
}


#endif
