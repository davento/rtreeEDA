#ifndef  _R_TREE_H_
#define  _R_TREE_H_


#include <vector>
#include "figures.hpp"


class Rtree{
  
  public:
    
    bool insert(Figure);
  
  private:
    
    void split();
    
    MBB minimum;
    
    Rtree *first, *second, *third;
    
    Figure myFigure;

};


#endif
