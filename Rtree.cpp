#include "Rtree.h"
#include "RtreeNode.h"

Rtree::Rtree(){
    root = new Node();
}
Rtree::Rtree(int dim){
    root = new Node(MBC(Point(0,0), Point(dim, dim)));
}

Rtree::Node* Rtree::search(const Point& p){
    return search(root, p);
}

void Rtree::insert(const Figure& f){
    root = insert(root, f);
    // std::cout<<"Finish insert\n";
    // std::cout<<"----------------------------------------------\n";
    // print();
}

void Rtree::remove(const Point& p){
    remove(root, p);
}

void Rtree::draw(SDL_Renderer* renderer) const{
    // std::cout<<"A\n";
    //RGB -> BGR
    root->draw(renderer, Color(0,40,0));
}


Rtree::Node* Rtree::search(Node* node, const Point &p){
    
    if (node->isLeaf()) {
        if(node->father == nullptr) return node;

        for(auto r: node->children){
            auto f = static_cast<LeafNode*>(r);

            if(f->bound.inArea(p)) return node;
        }

        return nullptr;
    }

    for (auto r : node->children) {
        if (r->bound.inArea(p)){
            auto res = search(r, p);
            if(res != nullptr) return res;
        }
    }
    return nullptr;
}

Rtree::Node* Rtree::chooseSubtree(Node* node, const Figure &f){
        
    int minP = INF;
    Node* result = node;
    //choose region with minimum perimeter
    for(auto region : node->children) {
        Bound aux( f.getBound() );
        aux.merge(region->bound);
        
        int p = aux.perimeter();
        if(p <= minP) {
            minP = p;
            result = region;
        }
        
    }
    return result;
}

Rtree::Node* Rtree::insert(Node* node, const Figure &f){
    
    // std::cout<<"entering insert\n";
    // printf("inserting Point(%f,%f)\n", p.x,p.y);
    
    if(node->isLeaf()){
        // std::cout<<"inserting in leaf\n";
        
        Node* it = new LeafNode(f);
        (node->children).push_back(it);
        node->mergeBounds();
        if((node->children).size() == ORDER + 1){
            // std::cout<<"overflow\n";
            handleOverflow(node);
        }
    }
    else{
        Node* v = chooseSubtree(node, f);
        insert(v, f);
        node->mergeBounds();
    }
    if(node->father)
        return node->father;
    return node;
} 


void Rtree::handleOverflow(Node* overFlowed){
    
    Node* v = new Node();
    split(overFlowed, v);
    if(!overFlowed->father){
        //Create new father
        Node* overFather = new Node();
        //Add children to father
        overFlowed->father = v->father = overFather;
        (overFather->children).push_back(overFlowed);
        (overFather->children).push_back(v);
        overFather->mergeBounds();
    }
    else{
        Node* w = overFlowed->father;
        v->father = w;
        (w->children).push_back(v);
        w->mergeBounds();
        if((w->children).size() == ORDER + 1)
            handleOverflow(w);
    }
}

void Rtree::split(Node* original, Node* secondHalf){

    std::vector<Node*> regions = original->children;

    //sort by x left
    sort(regions.begin(), regions.end(),
    [](const Node* m1, const Node* m2){
        return m1->getBound().topLeft.x < m2->getBound().topLeft.x;
    });
    bestSplit(regions,original,secondHalf);

    //sort by x right
    sort(regions.begin(), regions.end(),
    [](const Node* m1, const Node* m2){
        return m1->getBound().bottomRight.x < m2->getBound().bottomRight.x;
    });
    bestSplit(regions,original,secondHalf);

    //sort by y left
    sort(regions.begin(), regions.end(),
    [](const Node* m1, const Node* m2){
        return m1->getBound().topLeft.y < m2->getBound().topLeft.y;
    });
    bestSplit(regions,original,secondHalf);

    //sort by y right
    sort(regions.begin(), regions.end(),
    [](const Node* m1, const Node* m2){
        return m1->getBound().bottomRight.y < m2->getBound().bottomRight.y;
    });
    bestSplit(regions,original,secondHalf);
}

void Rtree::bestSplit(std::vector<Node*>& u, Node* v, Node* p){

    int m = u.size();

    std::vector<Node*> s1;
    std::vector<Node*> s2;

    MBC m1 = v->bound;
    MBC m2 = v->bound;
    // double minVariance = 2e5;
    for( int i = ceil(m * 0.4); i <= m - ceil(m * 0.4); i++){
        // s1 = first i regions (points)
        // s2 = the other i regions (points)
        s1 = {u.begin(), u.begin() + i};
        s2 = {u.begin()+i , u.end() };
        //get mbb and choose minimum
        MBC t1 = Node::mergeBounds(s1);
        MBC t2 = Node::mergeBounds(s2);
        
        if(t1.metric() + t2.metric() <  m1.perimeter() + m2.perimeter()){
            m1 = t1; m2 = t2;
            v->bound = m1;
            p->bound = m2;
            v->children = s1;
            p->children = s2;
        }
    }
    v->mergeBounds();
    p->mergeBounds();
}


void Rtree::print() const{
    root->print();
}

void Rtree::remove(Node* node,const Point& p){
    
    Node* n = search(p);
    if(n == nullptr || !n->isLeaf() ){
        std::cout<<"Not found\n";
        return ;
    } 
    else{
        std::cout<<"Found\n";
    }
    
    auto fun = [&p](Node* node){
        return  (static_cast<LeafNode*>(node)->getBound().inArea(p));
    };
    
    auto it = std::find_if(n->children.begin(), n->children.end(), fun);
    if(it == n->children.end()) return ;

    (n->children).erase(it);

    if(n->father == nullptr && n->children.size() == 0){
        n->bound   = Bound();
        return ;
    }

    n->mergeBounds();
    if(n->children.size() >= std::ceil(ORDER/2.0) || n->father == nullptr) 
        return ;
    
    reinsert();
}


void Rtree::dfs(std::vector<Figure> &s, Rtree::Node* u){

    if(u->isLeaf()){
        for(auto f: u->children){
            s.push_back(static_cast<LeafNode*>(f)->getFigure());
        }
        return ;
    }

    for(auto r: u->children){
        dfs(s,r);
    }
}

void Rtree::reinsert(){

    std::vector<Figure> s;

    dfs(s,this->root);
    
    this->root = new Node;

    for(auto fig: s){
        insert(fig);
    }
    
}


template<typename TCmp>
void Rtree::k_depthFirst(std::priority_queue<Figure* , std::vector<Figure*>, TCmp> &p,
                const int &k,Rtree::Node* u){


    if(u->isLeaf()){
        for(auto f: u->children){
            p.push(&static_cast<Rtree::LeafNode*>(f)->getFigure());
            if(p.size() > (unsigned) k) p.pop();
        }
        return ;
    }

    for(auto r: u->children){
        k_depthFirst(p,k,r);
    }
}

std::vector<Figure*> Rtree::depthFirst(const Point& p){


    std::vector<Figure*> res;

    const int  k =3;

    auto func  = [&p](const Figure* f1, const Figure*f2){
        Bound m1 = f1->getBound();
        Bound m2 = f2->getBound();
        return MBC::distance(p, m1) < MBC::distance(p, m2);
    };


    std::priority_queue<Figure* , std::vector<Figure*>, decltype(func) > s(func);
    k_depthFirst(s,k,root);
    
    // df.clear();
    while(!s.empty()){
        auto f = s.top();
        res.push_back(f);
        s.pop();
    }

    return res;
}
