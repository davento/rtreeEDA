#ifndef FIGURENODE_H
#define FIGURENODE_H

#include "Node.h"
#include "../Figure.h"

template <typename T, unsigned ORDER>
class FigureNode final: public Node<T,ORDER>{
    public:

        FigureNode();
        FigureNode(Figure*);
        ~FigureNode();
        bool isLeaf() const override;
        void draw(SDL_Renderer* renderer, Color color = Color(0,0,255)) const override;
        Figure* getFigure(){return myFigure;}
    private:
        Figure* myFigure;
};

template <typename T, unsigned ORDER>
FigureNode<T, ORDER>::FigureNode(): Node<T,ORDER>(), myFigure(nullptr){

}

template <typename T, unsigned ORDER>
FigureNode<T, ORDER>::FigureNode(Figure* figure): myFigure(new Figure(*figure)){
    this->myBound = *static_cast<T*>(myFigure->myBound);
}

template <typename T, unsigned ORDER>
FigureNode<T,ORDER>::~FigureNode(){
    delete myFigure;
}

template <typename T, unsigned ORDER>
bool FigureNode<T,ORDER>::isLeaf() const {
    return true;
}

template <typename T, unsigned ORDER>
void FigureNode<T,ORDER>::draw(SDL_Renderer* renderer, Color color) const {
    this->myBound.draw(renderer, color);
    myFigure->draw(renderer, color);
}


#endif

