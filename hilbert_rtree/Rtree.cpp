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
    // std::cout<<"----------------------------------------------\n";
    // print();
}

void Rtree::remove(const Point& p){
    remove(root, p);
    // std::cout<<"----------------------------------------------\n";
    // if(root) print();
}

void Rtree::draw(SDL_Renderer* renderer) const{
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
        
    //chooses the first to the right (int h greates) with the greates  
    auto it = get_first(node->children, f.getCentroid(), lessHilbert);
    auto der = (it == node->children.end() ) ? node->children.back(): *it;

    return der;
}

Rtree::Node* Rtree::insert(Node* node, const Figure &f){
    

    
    //if leaf, insert figure, and check for overdlow
    if(node->isLeaf()){
        Node* it = new LeafNode(f);
        it->father = node;
        insert_ordered(node->children, it, compare);
        node->mergeBounds();
        if((node->children).size() == ORDER + 1){
            handleOverflow(node);
        }
    }
    else{
        //chooses subtree, inserts, and updates
        Node* v = chooseSubtree(node, f);
        insert(v, f);
        node->mergeBounds();
    }
    //returns node to update the root
    if(node->father)
        return node->father;
    return node;
} 



void Rtree::distribute(std::vector<Node*>& vec, iterator q, iterator s, int to_delete = 0){
    
    std::vector<Node*> holder;
    for(auto it = q; it != (s+1); ++it){
        for(auto child: (*it)->children){
            holder.push_back(child);    
        }
    }

    std::sort(holder.begin(), holder.end(), compare);

    int n = holder.size();
    int m = std::distance(q, s) + 1 - to_delete;

    auto it_h = holder.begin();
    for(auto it = q; it != s+1; it++){

        int to_insert =  ceil(n/ (m * 1.0));
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

        auto q =rev_q.base() -1;
        distribute(nodeFather->children, q, s);
        nodeFather->mergeBounds();

    }
    else{
        //create a node s+1
        auto node =  new Node();

        node->father =  nodeFather;
        s = nodeFather->children.insert(s + 1, node);

        //distribute between ( begin() , s+1)
        distribute(nodeFather->children, nodeFather->children.begin(), s);
        nodeFather->mergeBounds();
        if(nodeFather->children.size() == ORDER +1){
            handleOverflow(nodeFather);
        }
    }

}

void Rtree::split(Node* original, Node* secondHalf){

    //makes split of children  to distribute into original and secondHalf
    std::vector<Node*> regions = original->children;

    bestSplit(regions,original,secondHalf);
}

void Rtree::bestSplit(std::vector<Node*>& u, Node* v, Node* p){

    int m = u.size();

    // we just split in half
    std::vector<Node*> s1 = {u.begin(), u.begin() + ceil(m/2) };
    std::vector<Node*> s2 = {u.begin() + ceil(m/2) , u.end()};


    MBC m1 = Node::mergeBounds(s1);
    MBC m2 = Node::mergeBounds(s2);

    v->bound = m1;
    p->bound = m2;

    v->children = s1;
    p->children = s2;

    // updates father and bounds
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
    
    // if n is found, deletes the figure that contains p
    Node* n = search(p);
    if(n == nullptr || !n->isLeaf()) return ;

    auto fun = [&p](Node* node){
        return  (static_cast<LeafNode*>(node)->getBound().inArea(p));
    };
    auto it = std::find_if(n->children.begin(), n->children.end(), fun);
    if(it == n->children.end()) return ;

    (n->children).erase(it);
    
    // in case that n is root
    if(n->father == nullptr && n->children.size() == 0){
        n->bound = Bound();
        return ;
    }

    //update bound and values
    n->mergeBounds();
    update(n);

    //we check if we have underflow
    if(n->children.size() >= std::ceil(ORDER/2.0) || n->father == nullptr) 
        return ;
    
    // if we do, handle the cases for the distribution and merging of nodes
    handleUnderflow(n);
    update(n);
}

void Rtree::handleUnderflow(Node* underFlowed){

    // in the case that the node is the root
    // we insert manually the figures into the root
    if(underFlowed->father == nullptr){

        std::vector<Node*> holder;
        for(auto it : underFlowed->children){
            for(auto child: it->children){
                holder.push_back(child);    
            }
        }
        underFlowed->children.clear();
        std::sort(holder.begin(), holder.end(), compare);
        for(auto it: holder){
            it->father = underFlowed;
            underFlowed->children.push_back(it);
        }
        holder.clear();
        return ;
    }


    // we find the underflowed node position in its parent
    Node* nodeFather = underFlowed->father;

    auto s = std::find(nodeFather->children.begin(), nodeFather->children.end(), underFlowed);
    assert(s != nodeFather->children.end());
    
    // find the node where it can lease
    auto rev_q = std::find_if(std::make_reverse_iterator(s), nodeFather->children.rend(), 
                            [] (Node* n){ return n->children.size() > ceil(ORDER/2.0);});

    // if found a node who can borrow elements
    if(rev_q != nodeFather->children.rend()){
        //distribute among the nodes in (q,s)
        auto q =rev_q.base() -1;
        distribute(nodeFather->children, q, s);
    }
    else{
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


// we update the whole tree untile reaching root
void Rtree::update(Node* v) {
    v->mergeBounds();
    if(v != root) {
        update(v->father);
    }
}


// we get all the figures with a dfs traversal
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

// we get only k figures with dfs traversal
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


// Using DF to get the Knn
std::vector<Figure*> Rtree::depthFirst(const Point& p, int k ){

   std::vector<Figure*> res;

    auto func  = [&p](const Figure* f1, const Figure*f2){
        Bound m1 = f1->getBound();
        Bound m2 = f2->getBound();
        return MBC::distance(p, m1) < MBC::distance(p, m2);
    };


    std::priority_queue<Figure* , std::vector<Figure*>, decltype(func) > s(func);
    k_depthFirst(s,k,root);
    
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
