#include "SStreeNode.h"
#include "SStree.h"

SStreeNode::SStreeNode(const MBC& other): bound(other), father(nullptr){

}

SStreeNode::SStreeNode(): bound(), father(nullptr){
    children.reserve(ORDER + 1);
}

SStreeNode::SStreeNode(const SStreeNode& other){
    
    children.reserve(ORDER + 1);
    father = other.father;
    children = other.children;
    mergeBounds();
}


MBC SStreeNode::mergeBounds(std::vector<SStreeNode*> bounds){
    
    double area = {};
    MBC res(Point(0,0), Point(0,0));
    for(const auto& node : bounds){
        auto otherB = node->getBound();
        area += node->getBound().area();
        res.getCentroid().x +=  otherB.getCentroid().x * otherB.area();
        res.getCentroid().y +=  otherB.getCentroid().y * otherB.area();
    }
    res.getCentroid().x /= area;
    res.getCentroid().y /= area;

    //get radious
    double r = 0;
    for(const auto& node: bounds){
        auto otherB = node->getBound();

        double centDist = Point::distance(res.getCentroid(), otherB.getCentroid());

        r = std::max(
            r,
            centDist
        );
    }
    res.getRadius() = r;
    return res;
}

void SStreeNode::mergeBounds(){
    bound  = mergeBounds(this->children);
}

void SStreeNode::draw(SDL_Renderer* renderer, Color color) const {
    bound.draw(renderer, color);
    color.changeColor(150);
    for(const auto& child: children){
        child->draw(renderer, color);
    }
}

void PointsNode::draw(SDL_Renderer* renderer, Color color) const {
    SStreeNode::draw(renderer, color);
    for(const auto& point: points)
        point.draw(renderer);
}


PointsNode::PointsNode():SStreeNode(){
    points.reserve(ORDER+1);
}

PointsNode::PointsNode(const Point& p):SStreeNode(){
    points.reserve(ORDER+1);
    points.push_back(p);
}

