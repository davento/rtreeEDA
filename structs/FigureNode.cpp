#include "FigureNode.h"

template <typename T, unsigned ORDER>
FigureNode<T, ORDER>::FigureNode(): Node<T,ORDER>(), myFigure(nullptr){

}

template <typename T, unsigned ORDER>
FigureNode<T, ORDER>::FigureNode(Figure* figure): Node<T,ORDER>(), myFigure(figure){

}

template <typename T, unsigned ORDER>
inline bool FigureNode<T,ORDER>::isLeaf() const {
    return true;
}

template <typename T, unsigned ORDER>
void FigureNode<T,ORDER>::draw(SDL_Renderer* renderer, Color color) const {
    myFigure->draw(renderer);
}
