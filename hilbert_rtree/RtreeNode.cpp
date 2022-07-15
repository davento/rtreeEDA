#include "RtreeNode.h"
#include "Rtree.h"


RtreeNode::RtreeNode(const MBC& other): bound(other), father(nullptr), hv(0){

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
    hv = this->children.back()->lhv();
}

void RtreeNode::draw(SDL_Renderer* renderer, Color color) const {

    bound.draw(renderer, color);
    color.changeColor(150);
    
    for(const auto& child: children){
        child->draw(renderer, color);
    }
}

int RtreeNode::lhv() const{
    return hv;
}

void FigureNode::draw(SDL_Renderer* renderer, Color color) const {
    bound.draw(renderer);
    f.draw(renderer);
}


FigureNode::FigureNode():RtreeNode(){
    
}

FigureNode::FigureNode(const Figure& p): RtreeNode(), f(p){
    bound = f.getBound();

}

void RtreeNode::print() const {
    std::cout << "Node: ";
    bound.topLeft.print(); bound.bottomRight.print();
    
    std::cout << "Bound: "; std::cout<<lhv()<<'\n';
    std::cout << "{" << std::endl; 
    for(const auto& child: children){
        child->print();
    }
    std::cout << "\n}" << std::endl;
}

int FigureNode::lhv() const{
    return bound.centroid[Z];
}

void FigureNode::print() const {
    std::cout<<"Figure Node:"<<bound.getCentroid()[Z]<<'\n'; ;
}
