#ifndef FIGURENODE_H
#define FIGURENODE_H

#include "Node.h"
#include "Figure.h"

template <typename T, unsigned ORDER>
class FigureNode final: public Node<T,ORDER>{
    public:

        FigureNode();
        FigureNode(Figure*);
        inline bool isLeaf() const override;
        void draw(SDL_Renderer* renderer) const override;
        Figure* getFigure(){return myFigure;}
    private:
        Figure* myFigure;
};

#endif
