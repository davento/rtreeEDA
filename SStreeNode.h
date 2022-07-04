#ifndef SSTREENODE_H
#define SSTREENODE_H

#define ORDER 3

#include "Point.h"
#include "MBC.h"

#include <vector>
#include <algorithm>

class SStreeNode{
    public:

        SStreeNode();
        SStreeNode(const MBC&);
        SStreeNode(const SStreeNode&);
        
        void mergeBounds();
        static MBC mergeBounds(std::vector<SStreeNode*> );
        MBC getBound() const {return bound;}
        

        virtual bool isLeaf(){
            if(!children.empty() && children[0]->children.empty())
                return true;
            else if(children.empty())
                return true;
            return false;
        } 
        
        virtual void draw(SDL_Renderer* renderer, Color color = Color(0,0,255)) const;
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
        void draw(SDL_Renderer* renderer, Color color = Color(0,0,255)) const override;
        
    private:
        std::vector<Point> points;
        virtual bool isLeaf(){return true;}
};

#endif

