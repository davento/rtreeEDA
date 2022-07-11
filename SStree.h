#ifndef SSTREE_H
#define SSTREE_H

#include "SStreeNode.h"
#include "Point.h"
#include "Figure.h"
#include <SDL2/SDL.h>


const int INF = 2e5;

class SStree final{
    public:
        typedef RtreeNode Node;
        typedef FigureNode LeafNode;
        typedef MBC Bound;

        
        SStree();
        SStree(int dim);
        Node* search(const Point &);
        void insert(const Figure &);
        void remove(const Point &);
        void draw(SDL_Renderer* renderer) const;
        void print() const;
    private:

        Node* root;
        Node* search(Node*,const Point &);
        Node* chooseSubtree(Node*, const Figure &);

        Node* insert(Node*, const Figure &);
        void handleOverflow(Node*);
        void split(Node* original, Node* secondHalf);
        void bestSplit(std::vector<Node*>& u, Node* v, Node* p);

        
        // void remove(Node*, const Point &);
};

#endif

