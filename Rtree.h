#ifndef Rtree_H
#define Rtree_H

#include "RtreeNode.h"
#include "Point.h"
#include "Figure.h"
#include <queue>
#include <SDL2/SDL.h>
#include <algorithm>


const int INF = 2e5;

class Rtree final{
    public:
        typedef RtreeNode Node;
        typedef FigureNode LeafNode;
        typedef MBC Bound;

        
        Rtree();
        Rtree(int dim);
        Node* search(const Point &);
        void insert(const Figure &);
        void remove(const Point &);
        void draw(SDL_Renderer* renderer) const;
        void print() const;
        
        std::vector<Figure*> depthFirst(const Point& p);

    private:

        Node* root;
        Node* search(Node*,const Point &);
        Node* chooseSubtree(Node*, const Figure &);

        Node* insert(Node*, const Figure &);
        void handleOverflow(std::vector<Node*>::iterator, Node*);
        void split(Node* original, Node* secondHalf);
        void bestSplit(std::vector<Node*>& u, Node* v, Node* p);

        
        void remove(Node*, const Point &);
        void reinsert();
        void dfs(std::vector<Figure> &s, Rtree::Node* u);

        

        template<typename TCmp>
        void k_depthFirst(std::priority_queue<Figure* , std::vector<Figure*>, TCmp> &p,const int &k,Rtree::Node* u);
        

};


template <typename T, typename C>
auto insert_ordered(std::vector<T>& vec, const T& element, C& callable) -> decltype(vec.begin()) {
    auto i = vec.begin();
    for(; i != vec.end(); ++i){
        if(callable(element, *i)){
            break;
        }
    }
    return vec.insert(i, element);
}


template <typename T, typename F, typename C>
auto get_first(std::vector<T>& vec, const F& to_find, C& callable) -> decltype(vec.begin()){
  return std::lower_bound(vec.begin(), vec.end(), to_find, callable);
}

const auto compare  = [](const RtreeNode* n1, const RtreeNode* n2){
    return n1->lhv() < n2->lhv();
};


const auto lessHilbert = [](const RtreeNode* n, const Point& q){
              return n->lhv() < q[Z];
};


#endif

