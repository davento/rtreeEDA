#ifndef RTREE_H
#define RTREE_H

#include "Boundings/Bound.h"
#include "Boundings/MBR.h"
#include "Nodes/Node.h"
#include "Nodes/InternalNode.h"
#include "Nodes/FigureNode.h"
#include "Figure.h"
#include <algorithm>
#include <queue>

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
        std::vector<FigureNode<T,ORDER>*> depthFirst(const Point&);
        void draw(SDL_Renderer* renderer) const;

        static void drawDepthFirst(SDL_Renderer* renderer, std::vector<FigureNode<T,ORDER>*>& vec){
            for(auto elem: vec)
                elem->draw(renderer, Color(255,0,255) );
        }
    private:
        Node<T,ORDER>* root;
        // void reinsert();
        Node<T,ORDER>* search(Node<T,ORDER>*, const Point&);
        void split(Node<T,ORDER>* original, Node<T,ORDER>* secondHalf);
        void minimumPerimeter(std::vector<Node<T,ORDER>*>& u, Node<T,ORDER>* v, Node<T,ORDER>* p, int axis);
        void handleOverflow(Node<T,ORDER>* overFlowed);

        Node<T,ORDER>* insert(Node<T,ORDER>* , Figure*);
        void addChildrenToFather(
            Node<T,ORDER>* root, Node<T,ORDER>* nodeOverflowed, Node<T,ORDER>* v
        );
        Node<T,ORDER>* chooseSubtree(Node<T,ORDER>* root, Figure* figure);
        static boundType mergeRegions(std::vector<Node<T,ORDER>*>& vec);
        static boundType mergeRegions(std::vector<Node<MBR,ORDER>*>& vec);
        static boundType makeNewCombineBound(boundType left, boundType right);
        static void mergeUp(Node<T,ORDER> *node);

        Node<T,ORDER>* findClosestNodeM(Node<T,ORDER>* father, Node<T,ORDER>* node);
        Node<T,ORDER>* closestNode(Node<T,ORDER>* father, Node<T,ORDER>* node);
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
        root->myBound = mergeRegions(root->children);
        if((root->children).size() == ORDER + 1)
            handleOverflow(root);
    }
    else{
        Node<T,ORDER>* v = chooseSubtree(root, figure);
        insert(v, figure);
        root->myBound = mergeRegions(root->children);
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
        overFather->myBound = mergeRegions(overFather->children);
    }
    else{
        Node<T,ORDER>* w = overFlowed->father;
        v->father = w;
        (w->children).push_back(v);
        w->myBound = mergeRegions(w->children);
        if((w->children).size() == ORDER + 1)
            handleOverflow(w);
    }
}



template <typename T, unsigned ORDER>
void Rtree<T,ORDER>::split(Node<T,ORDER>* original, Node<T,ORDER>* secondHalf){

 
    int axis = variance(original->children, original->myBound, X) < 
                variance(original->children, original->myBound, Y) ? X : Y;

      
    std::vector<Node<T,ORDER>*> regions = original->children;

    // //sort by x left
    sort(regions.begin(), regions.end(),
    [&](const Node<T,ORDER>* m1, const Node<T,ORDER>* m2){
        return m1->myBound.getCentroid()[axis] < m2->myBound.getCentroid()[axis];
    });

    
    minimumPerimeter(regions,original,secondHalf,axis);
}



template <typename T, unsigned ORDER>
void Rtree<T,ORDER>::minimumPerimeter(std::vector<Node<T,ORDER>*>& u, Node<T,ORDER>* v, Node<T,ORDER>* p, int axis){

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



        
        if(variance(s1,t1,axis) + variance(s2,t2,axis) < variance(v->children, m1, axis) + variance(p->children, m2, axis)){
            m1 = t1; m2 = t2;
            v->myBound = m1;
            p->myBound = m2;
            v->children = s1;
            p->children = s2;
        }
    }
    v->myBound = mergeRegions(v->children);
    p->myBound = mergeRegions(p->children);

}

template <typename T, unsigned ORDER>
typename Rtree<T,ORDER>::boundType  Rtree<T, ORDER>::mergeRegions(std::vector<Node<MBR,ORDER>*>& vec){
    boundType res = (vec.front())->myBound;
    for(const auto& node: vec){
        res.merge(node->myBound);
    }
    return res;
}

template <typename T, unsigned ORDER>
typename Rtree<T,ORDER>::boundType Rtree<T,ORDER>::mergeRegions(std::vector<Node<T,ORDER>*>& vec){
    boundType res(Point(0,0), Point(0,0));
    
    //get area & numerator
    double area = {};
    for(const auto& node : vec){
        auto otherB = node->myBound;
        area += node->myBound.area();
        res.getCentroid().x +=  otherB.getCentroid().x * otherB.area();
        res.getCentroid().y +=  otherB.getCentroid().y * otherB.area();
    }
    res.getCentroid().x /= area;
    res.getCentroid().y /= area;

    //get radious
    auto r = res.getRadious();
    for(const auto& node: vec){
        auto otherB = node->myBound;

        double centDist = res.getCentroid().distance(otherB.getCentroid());
        if(centDist < r){
            centDist += (centDist + otherB.getRadious() >= r)? otherB.getRadious(): 0;
        }
        else{
            centDist += otherB.getRadious();
        }
        r = std::max(
            r,
            centDist
        );
    }
    res.getRadious() = r;

    return res;
}


template <typename T, unsigned ORDER>
void Rtree<T,ORDER>::mergeUp(Node<T,ORDER> *node){


    while (node){ 
        node->myBound =  mergeRegions(node->children);
        node = node->father;
    }
}


template <typename T, unsigned ORDER>
Node<T,ORDER>* Rtree<T,ORDER>::chooseSubtree(Node<T,ORDER>* node, Figure* figure){
    double minMetric = 1e9+10;
    Node<T,ORDER>* result = nullptr;
    for(const auto& child: node->children){
        // boundType aux = Rtree<T,ORDER>::makeNewCombineBound(*(figure->getBound()), node->myBound);
        // double p = aux.metric();
        double p = static_cast<MBC*> (figure->getBound())->getCentroid().
                    distance(child->myBound.getCentroid());
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
    root->draw(renderer, Color(40,0,0));
}


template<typename A, typename B, unsigned O>
void k_depthFirst(A &s, const size_t k, Node<B,O>* u, const Point &p){


    auto ep = u->myBound;
    
    Point mp = ep.getCentroid();
    double rp_max = ep.getRadious();
    //definig it as the minimum distance from u centroid to its children centroid
    double rp_min  = rp_max;
    for(auto f: u->children){
        rp_min = std::min(rp_min,
        f->myBound.getCentroid().distance(mp)) + f->myBound.getRadious();
    }
    // std::cout<<1<<'\n';
    Point pe = (s.size() == 0) ? Point() : s.top()->myBound.getCentroid();
    // std::cout<<2<<'\n'; 
    double dk = (s.size() < k)  ?  2e5 : p.distance(pe);
    
    // std::cout<<"a\n";
    if(u->isLeaf()){
        for(auto f: u->children){
            if(s.size() < k){
                s.push(static_cast<FigureNode<B,O>*>(f));
                continue;
            }

            Point& o = f->myBound.getCentroid();

            if(dk + rp_max < p.distance(mp) or
                dk + p.distance(mp) <  o.distance(mp)) continue;
            
            s.push(static_cast<FigureNode<B,O>*>(f));
            s.pop();
        }
        return ;
    }
    // std::cout<<"b\n";
    for(auto r: u->children){
        Point& o = r->myBound.getCentroid();
        if(dk + o.distance(mp) < p.distance(mp) or
            dk + p.distance(mp) < rp_min or
            (p.distance(mp) + rp_max < dk and k == 1) ){
            continue;   
        }
        k_depthFirst(s,k,r,p);
    }
}

template <typename T, unsigned ORDER>
std::vector<FigureNode<T,ORDER>*>  Rtree<T,ORDER>::depthFirst(const Point& p){

    const int  k =3;

    typedef FigureNode<T,ORDER> FN;

    auto func  = [p](const FN* f1, const FN*f2){
        T m1 = f1->myBound;
        T m2 = f2->myBound;
        return m1.getCentroid().distance(p) < m2.getCentroid().distance(p);
    };
    
    std::vector<FN*> result;

    std::priority_queue<FN*, std::vector<FN*>, decltype(func) > s(func);

    if(root->children.empty()) 
        return result;
    
    k_depthFirst(s,k,root,p);

     while(!s.empty()){
         result.push_back(s.top());
         s.pop();
     }
     return result;
    
}

template <typename T, unsigned ORDER>
Node<T, ORDER>* Rtree<T,ORDER>::search(const Point& p){
    Node<T,ORDER>* result = root;
    if(result->isLeaf()){
        return result;
    }
    else{
        while(!result->isLeaf()){
            Node<T,ORDER>* temp = result->children[0];
            for(const auto& node: result->children){
                    if(node->myBound.getCentroid().distance(p) < temp->myBound.getCentroid().distance(p))
                        temp = node;
            }
            result = temp;
        }
        return result;
    }
}

template <typename T, unsigned ORDER>
Node<T,ORDER>* Rtree<T,ORDER>::findClosestNodeM(Node<T,ORDER>* father, Node<T,ORDER>* node){
    Node<T,ORDER>* result = nullptr;
    for(const auto& child: father->children){
        if(!result && child->children.size() > ORDER/2)
            result = child;
        else if(result && result->myBound.getCentroid().distance(node->myBound.getCentroid()) > child->myBound.getCentroid().distance(node->myBound.getCentroid()) && child->children.size() > ORDER/2)
            result = child;
    }
    return result;
}

template <typename T, unsigned ORDER>
Node<T,ORDER>* Rtree<T,ORDER>::closestNode(Node<T,ORDER>* father, Node<T,ORDER>* node){
    Node<T,ORDER>* result = nullptr;
    for(const auto & child: father->children){
        if(!result)
            result = child;
        else if(result->myBound.getCentroid().distance(node->myBound.getCentroid()) > child->myBound.getCentroid().distance(node->myBound.getCentroid()))
            result = child;
    }
    return result;
}

template <typename T, unsigned ORDER>
void Rtree<T,ORDER>::remove(const Point& p){
   Node<T,ORDER>* leaf = search(p);
   
   auto it = leaf->children.begin(); 
   for(; it != leaf->children.end(); ++it)
       if((*it)->myBound.inArea(p))
           break;

   if(it == leaf->children.end()){
        return ;
   }
   leaf->children.erase(it);
    mergeUp(leaf);

   if(leaf->children.size() <= ORDER/2){

       if(leaf->father){
           Node<T,ORDER>* S = findClosestNodeM(leaf->father, leaf);
           if(S){
               auto sit = S->children.begin();
               decltype(sit) minDist = sit;
               for(; sit != S->children.end(); ++sit)
                   if((*sit)->myBound.getCentroid().distance(p) < (*minDist)->myBound.getCentroid().distance(p))
                       minDist = sit;
               insert(S, static_cast<FigureNode<T,ORDER>*>(*minDist)->myFigure);
               S->children.erase(minDist);
               mergeUp(S);
           }
           else{

               S = closestNode(leaf->father, leaf);
               for(auto figure: leaf->children){
                   insert(S, static_cast<FigureNode<T,ORDER>*>(figure)->myFigure);
               }
               leaf->children.clear();
               auto it = leaf->father->children.begin();
               for(; it != leaf->father->children.end(); ++it){
                    if(*it == leaf){
                        break;
                    }
               }
               leaf->father->children.erase(it);
               mergeUp(leaf);
           }
       }
   }
}
//123456789012345678901
////123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901
//123456789012345678901

#endif
