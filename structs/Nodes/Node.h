#ifndef NODE_H
#define NODE_H

#include "../Boundings/Bound.h"
#include "../Boundings/MBC.h"

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

        template<typename A, typename B, unsigned O>
        friend void k_depthFirst(A &s, const size_t k, Node<B,O>* u, const Point &p);

        template<unsigned O>
        friend double var(Node<T,O> *node, int axis);



        Node();
        virtual ~Node() = default;

        virtual bool isLeaf() const;
        virtual void draw(SDL_Renderer* renderer, Color color = Color(0,0,255)) const = 0;
        void mergeRegions();
        T getBound() {return myBound;};

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

template <unsigned O>
double variance(std::vector<Node<MBC,O> *> s, const MBC& m, int axis){

    if(s.empty()) return 2e5;
    
    int u = m.getCentroid()[axis];
    double res = 0;

    for(auto f: s){
        int x = f->getBound().getCentroid()[axis];
        
        res += (x-u) * (x-u);
    }
    
    res/= s.size();
    return res;
}



#endif
