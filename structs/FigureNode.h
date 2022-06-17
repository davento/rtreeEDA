#ifndef FIGURENODE_H
#define FIGURENODE_H

#include "Node.h"
#include "Figure.h"

template <typename T, unsigned ORDER>
class FigureNode final: public Node<T,ORDER>{
    public:
        FigureNode();
    private:
        Figure* myFigure;
};

#endif
