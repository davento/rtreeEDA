#include "SStreeNode.h"





SStreeNode::SStreeNode(): father(nullptr), bound(){
    children.reserve(ORDER + 1);
}

SStreeNode::SStreeNode(const SStreeNode& other){
    
    children.reserve(ORDER + 1);
    father = other.father;
    children = other.children;
    mergeBounds();
}

void SStreeNode::mergeBounds(){
    if(children.size() == 0) return ;

    MBC res = (children.front())->bound;
    for(const auto& node: children){
        res.merge(node->bound);
    }
}

void SStreeNode::draw(SDL_Renderer* renderer){
    //TODO
}

PointsNode::PointsNode():SStreeNode(){
    points.reserve(ORDER+1);
}

PointsNode::PointsNode(const Point& p):SStreeNode(){
    points.reserve(ORDER+1);
    points.push_back(p);
}
