#ifndef SSTREENODE_H
#define SSTREENODE_H

#define ORDER 3

#include "Point.h"
#include "MBC.h"
#include "Figure.h"

#include <vector>
#include <algorithm>

class RtreeNode{
    public:

        RtreeNode();
        RtreeNode(const MBC&);
        RtreeNode(const RtreeNode&);
        
        void mergeBounds();
        static MBC mergeBounds(std::vector<RtreeNode*> );
        MBC getBound() const {return bound;}
        

        virtual bool isLeaf(){
            if(!children.empty() && children[0]->children.empty())
                return true;
            else if(children.empty())
                return true;
            return false;
        } 
        
        virtual void draw(SDL_Renderer* renderer, Color color = Color(0,0,255)) const;
        virtual void print() const;
    protected:
        MBC bound;
        std::vector<RtreeNode*> children;
        RtreeNode* father;

        friend class SStree;
};

class FigureNode final : public RtreeNode{
    public:
        FigureNode();
        FigureNode(const Figure&);
        void draw(SDL_Renderer* renderer, Color color = Color(0,0,255)) const override;
        void print() const override;

    private:
        Figure f;
        virtual bool isLeaf(){return true;}
};

#endif
