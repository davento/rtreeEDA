#ifndef  _R_TREE_H_
#define  _R_TREE_H_


#include <vector>
#include <algorithm>
#include "figures.hpp"

#define ORDER 3

const int INF = 1e9+10;







class Rtree{
  
  private:


    std::vector<Figure*> myFigures;
    Rtree* father;
    MBB bound;
    
    size_t cur_figs{};
    std::vector<Rtree*> regions;
    
    static std::pair<Rtree*, Rtree*> split(Rtree*);
    void handleOverflow(Figure *f);

  public:
    
    Rtree():father(nullptr){
      myFigures.reserve(ORDER + 1);
      regions.reserve(ORDER + 1);
    };
    
    Rtree(std::vector<Rtree*> reg, MBB mb):
    bound(mb), regions(reg.begin(), reg.end()){
      cur_figs = regions.size();
    }

    // this is only done if node is a 'figure node'
    Rtree(Figure*);
    

    Rtree* search(Figure *);
    Rtree* chooseSubtree(Figure *);
    
    bool insert(Figure *);
    inline bool isLeaf() const {
      /*if(regions.size())
        return this->regions[0]->myFigure != nullptr;
      else
        return true;*/
      return regions.empty();
    }    
    void update(std::vector<Rtree*> reg, MBB mb){
      regions = reg;
      bound = mb;
      cur_figs = regions.size();
    }

    static MBB regionsMbb( std::vector<Rtree*> regions){
        MBB res;
        
        for(auto region: regions){
            res = MBB::merge(res, region->getBound());
        }
        return res;
    }


    MBB getBound(){ return bound;}

    void draw(SDL_Renderer* renderer) const {
      
      if(isLeaf()&& !myFigures.empty()){
        for(const auto& figure: myFigures)
          figure->draw(renderer);
      }
      bound.draw(renderer);
      if(!regions.empty()){
        for(const auto& region: regions)
          region->draw(renderer);
      }
    }
};


#endif
