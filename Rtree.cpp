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
    std::cout<<"----------------------------------------------\n";
    print();
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
        return node;
    }

    //find first largest element
    auto it = get_first(node->children, p, lessHilbert);

    if(it == node->children.end()) return nullptr;
    return search(*it, p);
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



void Rtree::distribute(std::vector<Node*>& vec, iterator q, iterator s){
    
    std::vector<Node*> holder;
    for(auto it = q; it != (s+1); ++it){
        std::cout<<(*it)->children.size()<<'\n';
        for(auto child: (*it)->children){
            holder.push_back(child);    
        }
    }

    std::sort(holder.begin(), holder.end(), compare);

    int n = holder.size();
    int m = std::distance(q, s) + 1;

    auto it_h = holder.begin();
    printf("num of nodes: %d| num of cont: %d\n", n,m);
    for(auto it = q; it != s+1; it++){

        int to_insert =  ceil(n/ (m * 1.0));
        std::cout<<to_insert<<'\n';
        (*it)->children.clear();
        (*it)->children.insert((*it)->children.begin(), it_h , it_h + to_insert);
        (*it)->mergeBounds();

        it_h += to_insert;
        n -= to_insert;
        m--;
    }
    holder.clear();
}

void Rtree::handleOverflow(Node* overFlowed){
    


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
    //find node to the left where so can lease to
    auto rev_q = std::find_if(std::make_reverse_iterator(s), nodeFather->children.rend(), 
                            [] (Node* n){ return n->children.size() < ORDER;});


    if(rev_q != nodeFather->children.rend()){
        std::cout<<"lhv:"<<(*rev_q)->lhv()<<'\n';
        std::cout<<"lhv:"<<(*s)->lhv()<<'\n';
    } 
        
    
    // if found a node where you can lease to
    if(rev_q != nodeFather->children.rend()){
        //distribute among the nodes in (q,s)
        auto q =rev_q.base() -1;
        distribute(nodeFather->children, q, s);
    }
    else{
        //create a node s+1
        auto node =  new Node();

        node->father =  nodeFather;
        s = nodeFather->children.insert(s+1, node);
        
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
