#ifndef SSTREE_H
#define SSTREE_H

#include "SStreeNode.h"
#include "Point.h"
#include <SDL2/SDL.h>

class SStree final{
    public:
        typedef SStreeNode Node;
        typedef PointsNode LeafNode;
        
        SStree();
        Node* search(const Point &);
        void insert(const Point &);
        void remove(const Point &);
        void draw(SDL_Renderer* renderer) const;
    private:

        int ORDER;        

        Node* root;
        Node* search(Node*,const Point &);
        Node* chooseSubtree(Node*, const Point &);

        Node* insert(Node*, const Point &);
        void handleOverflow(Node*);
        void split(Node* original, Node* secondHalf);
        void bestSplit(std::vector<Node*>& u, Node* v, Node* p, int axis);

        
        void remove(Node*, const Point &);
};

#endif

