#include "Rtree.h"
#include "RtreeNode.h"
#include <assert.h>

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
    std::cout<<"----------------------------------------------\n";
   print();
}

void Rtree::remove(const Point& p){
    remove(root, p);
    std::cout<<"----------------------------------------------\n";
    print();
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
        
    //optimization made to lessen overlap
    auto it = get_first(node->children, f.getCentroid(), lessHilbert);
    auto der = (it == node->children.end() ) ? node->children.back(): *it;

    return der;
}

Rtree::Node* Rtree::insert(Node* node, const Figure &f){
    
    // std::cout<<"entering insert\n";
    // printf("inserting Point(%f,%f)\n", p.x,p.y);
    
    if(node->isLeaf()){
        // std::cout<<"inserting in leaf\n";
        
        Node* it = new LeafNode(f);
        it->father = node;
        insert_ordered(node->children, it, compare);
        node->mergeBounds();
        if((node->children).size() == ORDER + 1){
            // std::cout<<"overflow\n";
            handleOverflow(node);
        }
    }
    else{
        Node* v = chooseSubtree(node, f);
        printf("from [%d], Choose subtree: %d\n", f.lhv(), v->lhv());
        insert(v, f);
        node->mergeBounds();
    }
    if(node->father)
        return node->father;
    return node;
} 



void Rtree::distribute(std::vector<Node*>& vec, iterator q, iterator s, int to_delete = 0){
    
    std::vector<Node*> holder;
    for(auto it = q; it != (s+1); ++it){
        std::cout<<(*it)->children.size()<<'\n';
        for(auto child: (*it)->children){
            holder.push_back(child);    
        }
    }

    std::sort(holder.begin(), holder.end(), compare);

    int n = holder.size();
    int m = std::distance(q, s) + 1 - to_delete;

    auto it_h = holder.begin();
    printf("num of nodes: %d| num of cont: %d\n", n,m);
    for(auto it = q; it != s+1; it++){

        int to_insert =  ceil(n/ (m * 1.0));
        std::cout<<to_insert<<'\n';
        (*it)->children.clear();
        (*it)->children.insert((*it)->children.begin(), it_h , it_h + to_insert);
        
        std::for_each(it_h, it_h + to_insert, [&](Node* lamb_n){  
            lamb_n->father = (*it);
        } );

        (*it)->mergeBounds();

        it_h += to_insert;
        n -= to_insert;
        m--;

        if(n <= 0) break;
    }
    holder.clear();
}


void Rtree::propagateUpward(Node* node){

    while(node->father != nullptr){
        node->mergeBounds();
        node = node->father;
    }
}

void Rtree::handleOverflow(Node* overFlowed){
    
    std::cout<<"handle overflow\n";

    //if root
    if(!overFlowed->father){
        //make an split
        Node* v = new Node();
        split(overFlowed, v);
        Node* overFather = new Node();
        //Add children to father
        overFlowed->father = v->father = overFather;
        insert_ordered(overFather->children, overFlowed, compare);
        insert_ordered(overFather->children, v, compare);
        overFather->mergeBounds();
        return ;
    }

    Node* nodeFather = overFlowed->father;
    //find position of OverFlowed in father
    auto s = std::find(nodeFather->children.begin(), nodeFather->children.end(), overFlowed);

    assert(s != nodeFather->children.end());
    //find node to the left where so can lease to
    auto rev_q = std::find_if(std::make_reverse_iterator(s), nodeFather->children.rend(), 
                            [] (Node* n){ return n->children.size() < ORDER;});

    
    // if found a node where you can lease to
    if(rev_q != nodeFather->children.rend()){
        //distribute among the nodes in (q,s)
        std::cout<<"Found to lease\n";

        auto q =rev_q.base() -1;
        distribute(nodeFather->children, q, s);
    }
    else{
        //create a node s+1
        std::cout<<"Didn't find to lease\n";
        
        auto node =  new Node();

        node->father =  nodeFather;
        s = nodeFather->children.insert(s + 1, node);
        
        //distribute between ( begin() , s+1)
        distribute(nodeFather->children, nodeFather->children.begin(), s);

        if(nodeFather->children.size() == ORDER +1){
            handleOverflow(nodeFather);
            nodeFather->mergeBounds();
        }
    }

}

void Rtree::split(Node* original, Node* secondHalf){

    std::vector<Node*> regions = original->children;

    bestSplit(regions,original,secondHalf);
}

void Rtree::bestSplit(std::vector<Node*>& u, Node* v, Node* p){

    int m = u.size();

    std::vector<Node*> s1 = {u.begin(), u.begin() + ceil(m/2) };
    std::vector<Node*> s2 = {u.begin() + ceil(m/2) , u.end()};


    MBC m1 = Node::mergeBounds(s1);
    MBC m2 = Node::mergeBounds(s2);

    v->bound = m1;
    p->bound = m2;

    v->children = s1;
    p->children = s2;


    std::for_each(v->children.begin(), v->children.end(), [&](Node* lamb_n){  
            lamb_n->father = v;
    } );
    
    std::for_each(p->children.begin(), p->children.end(), [&](Node* lamb_n){  
            lamb_n->father = p;
    } );

    v->mergeBounds();
    p->mergeBounds();
}


void Rtree::print() const{
    root->print();
}

void Rtree::remove(Node* node,const Point& p){
    
    Node* n = search(p);
    if(n == nullptr || !n->isLeaf() ) return ;
    
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
    update(n);

    if(n->children.size() >= std::ceil(ORDER/2.0) || n->father == nullptr) 
        return ;
    
    handleUnderflow(n);
    update(n);
}

void Rtree::handleUnderflow(Node* underFlowed){

    std::cout<<"handleUnderflow\n";

    if(underFlowed->father == nullptr){
        
    }

    Node* nodeFather = underFlowed->father;

    auto s = std::find(nodeFather->children.begin(), nodeFather->children.end(), underFlowed);
    assert(s != nodeFather->children.end());
    
    auto rev_q = std::find_if(std::make_reverse_iterator(s), nodeFather->children.rend(), 
                            [] (Node* n){ return n->children.size() > ceil(ORDER/2.0);});

    // if found a node who can lend elements
    if(rev_q != nodeFather->children.rend()){
        std::cout<<"found lender\n";
        //distribute among the nodes in (q,s)
        auto q =rev_q.base() -1;
        distribute(nodeFather->children, q, s);
    }
    else{
        std::cout<<"didn't find lender\n";
        //merge from s to s-1 nodes

        
        //distribute between ( begin() , s)
        distribute(nodeFather->children, nodeFather->children.begin(), nodeFather->children.end() - 1, 1);
        
        nodeFather->children.erase(nodeFather->children.end() - 1);
        nodeFather->mergeBounds();


        if(nodeFather->children.size() < ceil(ORDER/2.0)){
            handleUnderflow(nodeFather);
            nodeFather->mergeBounds();
        }
    }
}

void Rtree::update(Node* v) {
    v->mergeBounds();
    if(v != root) {
        update(v->father);
    }
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
std::vector<Figure*> Rtree::getFigures(){
    std::vector<Figure*> result;
    const int k = 1215752192;
    Point p(0,0);
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
        result.push_back(f);
        s.pop();
    }

   
    return result;
}
