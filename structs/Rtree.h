#ifndef RTREE_H
#define RTREE_H

#include "Boundings/Bound.h"
#include "Nodes/Node.h"
#include "Nodes/InternalNode.h"
#include "Nodes/FigureNode.h"
#include "Figure.h"
#include <algorithm>

template<typename T , unsigned ORDER>
class Rtree{
    public:
        typedef T boundType;
        Rtree();
        ~Rtree();
        //TODO: chequear si podemos agregar const al retorno
        Node<T,ORDER>* search(const Point&);
        bool insert(Figure*);
        void remove(const Point&);
        // std::vector<Figure*> depthFirst(const Point&);
        void draw(SDL_Renderer* renderer) const;

    private:
        Node<T,ORDER>* root;
        // void reinsert();
        Node<T,ORDER>* search(Node<T,ORDER>*, const Point&);
        void split(Node<T,ORDER>* original, Node<T,ORDER>* secondHalf);
        void minimumPerimeter(std::vector<Node<T,ORDER>*>& u, Node<T,ORDER>* v, Node<T,ORDER>* p);
        void handleOverflow(Node<T,ORDER>* overFlowed);

        Node<T,ORDER>* insert(Node<T,ORDER>* , Figure*);
        void addChildrenToFather(
            Node<T,ORDER>* root, Node<T,ORDER>* nodeOverflowed, Node<T,ORDER>* v
        );
        Node<T,ORDER>* chooseSubtree(Node<T,ORDER>* root, Figure* figure);
        static boundType mergeRegions(std::vector<Node<T,ORDER>*>& vec);
        static boundType makeNewCombineBound(boundType left, boundType right);
};

template <typename T, unsigned ORDER>
Rtree<T, ORDER>::Rtree(): root(new InternalNode<T,ORDER>()){

}

template <typename T, unsigned ORDER>
Rtree<T, ORDER>::~Rtree(){
    delete root;
    root = nullptr;
}

template <typename T, unsigned ORDER>
bool Rtree<T, ORDER>::insert(Figure* figure){
    root = insert(root, figure);
    return true;
}

template <typename T, unsigned ORDER>
Node<T,ORDER>* Rtree<T,ORDER>::insert(Node<T,ORDER>* root, Figure* figure){
    if(root->isLeaf()){
        FigureNode<T,ORDER>* it = new FigureNode<T,ORDER>(figure);
        Node<T,ORDER>* item = it;
        (root->children).push_back(item);
        root->mergeRegions();
        if((root->children).size() == ORDER + 1)
            handleOverflow(root);
    }
    else{
        Node<T,ORDER>* v = chooseSubtree(root, figure);
        insert(v, figure);
        root->mergeRegions();
    }
    if(root->father)
        return root->father;
    return root;
}

template <typename T, unsigned ORDER>
void Rtree<T,ORDER>::handleOverflow(Node<T,ORDER>* overFlowed){
    Node<T,ORDER>* v = new InternalNode<T,ORDER>();
    split(overFlowed, v);
    if(!overFlowed->father){
        //Create new father
        Node<T,ORDER>* overFather = new InternalNode<T,ORDER>();
        //Add children to father
        overFlowed->father = v->father = overFather;
        (overFather->children).push_back(overFlowed);
        (overFather->children).push_back(v);
        overFather->mergeRegions();
    }
    else{
        Node<T,ORDER>* w = overFlowed->father;
        v->father = w;
        (w->children).push_back(v);
        w->mergeRegions();
        if((w->children).size() == ORDER + 1)
            handleOverflow(w);
    }
}

template <typename T, unsigned ORDER>
void Rtree<T,ORDER>::split(Node<T,ORDER>* original, Node<T,ORDER>* secondHalf){
    std::vector<Node<T,ORDER>*> regions = original->children;

    //sort by x left
    sort(regions.begin(), regions.end(),
    [](const Node<T,ORDER>* m1, const Node<T,ORDER>* m2){
        return m1->myBound.getTopLeft().x < m2->myBound.getTopLeft().x;
    });
    minimumPerimeter(regions,original,secondHalf);
    
    //sort by x right  
    sort(regions.begin(), regions.end(),
    [](const Node<T,ORDER>* m1, const Node<T,ORDER>* m2){
        return m1->myBound.getBottomRight().x < m2->myBound.getBottomRight().x;
    });
    minimumPerimeter(regions,original,secondHalf);

    //same thing but with y left
    sort(regions.begin(), regions.end(),
    [](const Node<T,ORDER>* m1, const Node<T,ORDER>* m2){
        return m1->myBound.getTopLeft().y < m2->myBound.getTopLeft().y;
    });
    minimumPerimeter(regions,original,secondHalf);

    //same thing but with y right
    sort(regions.begin(), regions.end(),
    [](const Node<T,ORDER>* m1, const Node<T,ORDER>* m2){
        return m1->myBound.getBottomRight().y < m2->myBound.getBottomRight().y;
    });
    minimumPerimeter(regions,original,secondHalf);
}

template <typename T, unsigned ORDER>
void Rtree<T,ORDER>::minimumPerimeter(std::vector<Node<T,ORDER>*>& u, Node<T,ORDER>* v, Node<T,ORDER>* p){
    int m = u.size();

    std::vector<Node<T,ORDER>*> s1;
    std::vector<Node<T,ORDER>*> s2;

    boundType m1 = v->myBound;
    boundType m2 = v->myBound;


    for( int i = ceil(m * 0.4); i <= m - ceil(m * 0.4); i++){
        // s1 = first i regions (points)
        // s2 = the other i regions (points)
        s1 = {u.begin(), u.begin() + i};
        s2 = {u.begin()+i , u.end() };
        //get mbb and choose minimum
        boundType t1 = Rtree<T,ORDER>::mergeRegions(s1);
        boundType t2 = Rtree<T,ORDER>::mergeRegions(s2);        
        
        if(t1.metric() + t2.metric() < m1.metric() + m2.metric()){
            m1 = t1; m2 = t2;
            v->myBound = m1;
            p->myBound = m2;
            v->children = s1;
            p->children = s2;
        }
    }
    v->mergeRegions();
    p->mergeRegions();

}

template <typename T, unsigned ORDER>
typename Rtree<T,ORDER>::boundType Rtree<T,ORDER>::mergeRegions(std::vector<Node<T,ORDER>*>& vec){
    boundType res = (vec.front())->myBound;
    for(const auto& node: vec){
        res.merge(node->myBound);
    }
    return res;
}

template <typename T, unsigned ORDER>
Node<T,ORDER>* Rtree<T,ORDER>::chooseSubtree(Node<T,ORDER>* node, Figure* figure){
    double minMetric = 1e9+10;
    Node<T,ORDER>* result = node;
    for(const auto& child: node->children){
        boundType aux = Rtree<T,ORDER>::makeNewCombineBound(*(figure->getBound()), node->myBound);
        double p = aux.metric();
        if(p <= minMetric){
            minMetric = p;
            result = child;
        }
    }
    return result;
   
}

template <typename T, unsigned ORDER>
T Rtree<T,ORDER>::makeNewCombineBound(boundType left, boundType right){
    boundType result(left.getTopLeft(), left.getBottomRight());
    result.merge(right);
    return result;
}

template <typename T, unsigned ORDER>
void Rtree<T,ORDER>::draw(SDL_Renderer *renderer) const{
    root->draw(renderer, Color(0,40,0));
}

#endif
