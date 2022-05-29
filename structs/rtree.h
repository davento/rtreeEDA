#ifndef  _R_TREE_H_
#define  _R_TREE_H_


#include <vector>
#include "figures.h"

struct Bound{
  double topY, leftX;
  double height, width;
};


class Rtree{
  public:
    bool insert(figure);
  private:
    void split();
    Bound minimum;
    Rtree *first, *second, *third;
    std::vector<figure> figures;
};


#endif