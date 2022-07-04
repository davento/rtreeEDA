#ifndef SSTREENODE_H
#define SSTREENODE_H

#define ORDER 3

#include "Point.h"
#include "SStree.h"
#include "MBC.h"

#include <vector>
#include <algorithm>

class SStreeNode{
    public:

        SStreeNode();
        SStreeNode(const SStreeNode&) = default;
        
        void mergeBounds();
        static MBC mergeBounds(std::vector<SStreeNode*> );
        MBC getBound() const {return bound;}
        

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
        PointsNode();
        PointsNode(const Point&);
        
    private:
        std::vector<Point> points;
        virtual bool isLeaf(){return true;}
};

#endif

