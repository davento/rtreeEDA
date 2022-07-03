#ifndef SSTREENODE_H
#define SSTREENODE_H

#include "Point.h"
#include "SStree.h"
#include "MBC.h"
#include <vector>

class SStreeNode{
    public:

        SStreeNode();
        SStreeNode(const SStreeNode&) = default;
        
        MBC getBound() const {return bound;}
        MBC mergeBounds();
        virtual bool isLeaf(){return false;} 
        
        void draw(SDL_Renderer* renderer);
        
    protected:
        MBC bound;
        std::vector<SStreeNode*> children;
        SStreeNode* father;
        friend class SStree;
};

class PointsNode final : public SStreeNode{
    public:
    
        PointsNode(const Point&);

    private:
        std::vector<Point> points;
        virtual bool isLeaf(){return true;}
};

#endif

