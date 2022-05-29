#ifndef  _R_TREE_H_
#define  _R_TREE_H_


#include <vector>
#include "figures.hpp"

#define ORDER 3

const int INF = 1e9+10;







class Rtree{
  
  private:


    Figure* myFigure;
    Rtree* father;
    MBB bound;
    
    size_t cur_figs{};
    Rtree* regions[ORDER];
    
    void split();

  public:
    
    Rtree() = default;
    Rtree(Figure*);

    Rtree* search(Figure *);
    Rtree* chooseSubtree(Figure *);
    
    bool insert(Figure *);
    inline bool isLeaf();
    
};


#endif
