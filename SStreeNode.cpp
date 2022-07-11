#include "SStreeNode.h"
#include "SStree.h"

RtreeNode::RtreeNode(const MBC& other): bound(other), father(nullptr){

}

RtreeNode::RtreeNode(): bound(), father(nullptr){
    children.reserve(ORDER + 1);
}

RtreeNode::RtreeNode(const RtreeNode& other){
    
    children.reserve(ORDER + 1);
    father = other.father;
    children = other.children;
    mergeBounds();
}


MBC RtreeNode::mergeBounds(std::vector<RtreeNode*> bounds){
    
    MBC res = bounds.front()->getBound();
    for(auto region: bounds){
        res.merge(region->getBound());
    }
 
    return res;
}

void RtreeNode::mergeBounds(){
    bound  = mergeBounds(this->children);
}

void RtreeNode::draw(SDL_Renderer* renderer, Color color) const {
    // std::cout<<"A1\n";

    bound.draw(renderer, color);
    // std::cout<<"A1.4\n";

    color.changeColor(150);
    // std::cout<<"A2\n";
    for(const auto& child: children){
        child->draw(renderer, color);
    }
}

void FigureNode::draw(SDL_Renderer* renderer, Color color) const {
    
    color.changeColor(150);

    RtreeNode::draw(renderer, color);
       f.draw(renderer);
}


FigureNode::FigureNode():RtreeNode(){
    
}

FigureNode::FigureNode(const Figure& p): f(p), RtreeNode(){
    bound = f.getBound();

}

void RtreeNode::print() const {
    std::cout << "Node: ";
    bound.topLeft.print(); bound.bottomRight.print();
    
    // std::cout << "Bound: "; bound.getCentroid().print(); std::cout << " " << bound.getRadius() << std::endl;
    std::cout << "{" << std::endl; 
    for(const auto& child: children){
        child->print();
    }
    std::cout << "\n}" << std::endl;
}

void FigureNode::print() const {
    
}
