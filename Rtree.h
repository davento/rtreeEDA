#ifndef Rtree_H
#define Rtree_H

#include "RtreeNode.h"
#include "Point.h"
#include "Figure.h"
#include <queue>
#include <SDL2/SDL.h>


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
        std::vector<Figure*> getFigures();
    private:

        Node* root;
        Node* search(Node*,const Point &);
        Node* chooseSubtree(Node*, const Figure &);

        Node* insert(Node*, const Figure &);
        void handleOverflow(Node*);
        void split(Node* original, Node* secondHalf);
        void bestSplit(std::vector<Node*>& u, Node* v, Node* p);

        
        void remove(Node*, const Point &);
        void reinsert();
        void updateTree(Node* node);
        void propagateUpwards(Node* node);
        
        void dfs(std::vector<Figure> &s, Rtree::Node* u);

        

        template<typename TCmp>
        void k_depthFirst(std::priority_queue<Figure* , std::vector<Figure*>, TCmp> &p,const int &k,Rtree::Node* u);

        void clear(Node*);
        

};

#endif

