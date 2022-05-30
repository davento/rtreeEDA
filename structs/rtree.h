#ifndef  _R_TREE_H_
#define  _R_TREE_H_


#include <vector>
#include <algorithm>
#include "figures.hpp"

#define ORDER 3

const int INF = 1e9+10;







class Rtree{
  
  private:


    Figure* myFigure;
    Rtree* father;
    MBB bound;
    
    size_t cur_figs{};
    std::vector<Rtree*> regions;
    
    static std::pair<Rtree*, Rtree*> split(Rtree*);
    void handleOverflow(Figure *f);

  public:
    
    Rtree(){
      regions.reserve(ORDER + 1);
    };
    
    Rtree(std::vector<Rtree*> reg, MBB mb): regions(reg.begin(), reg.end()),
    bound(mb){
      cur_figs = regions.size();
    }

    // this is only done if node is a 'figure node'
    Rtree(Figure*);
    

    Rtree* search(Figure *);
    Rtree* chooseSubtree(Figure *);
    
    bool insert(Figure *);
    inline bool isLeaf();

    Rtree update(std::vector<Rtree*> reg, MBB mb){
      regions = reg;
      bound = mb;
      cur_figs = regions.size();
    }

    MBB getBound(){ return bound;}
};


#endif
