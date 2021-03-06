#ifndef RTREENODE_H
#define RTREENODE_H

#define ORDER 3

#include "Point.h"
#include "Figure.h"

#include <vector>
#include <algorithm>
#include <queue>
#include <functional>


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
        virtual int lhv() const;

    protected:

        typedef std::function<bool (RtreeNode*, RtreeNode*) > comptype;
        typedef std::priority_queue<RtreeNode* , std::vector<RtreeNode*>, comptype> HibertVec;

        MBC bound;
        std::vector<RtreeNode*> children;
        RtreeNode* father;
        int hv;

        friend class Rtree;
};

class FigureNode final : public RtreeNode{
    public:
        FigureNode();
        FigureNode(const Figure&);

        void draw(SDL_Renderer* renderer, Color color = Color(0,0,255)) const override;
        void print() const override;
        virtual int lhv() const override;

        Figure& getFigure() {return f;}

    private:
        Figure f;
        virtual bool isLeaf(){return true;}
};

#endif

