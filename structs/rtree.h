#ifndef  _R_TREE_H_
#define  _R_TREE_H_


#include <vector>
#include "figures.hpp"

#define ORDER 3

class Rtree{
  
  private:

    Figure myFigure;
    MBB minimum;
    
    size_t cur_figs{};
    Rtree* node[ORDER];
    
    void split();

  public:
    
    Rtree() = default;
    Rtree(Figure);

    Rtree* search(const Figure&);
    
    bool insert(Figure);
    
};


#endif
