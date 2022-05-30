#ifndef  _R_TREE_H_
#define  _R_TREE_H_


#include <vector>
#include <algorithm>
#include "figures.hpp"

#define ORDER 3

const int INF = 1e9+10;


struct RNode{
  
  std::vector<Figure*> myFigures;
  RNode* father;
  MBB bound;
    
  size_t cur_figs{};
  std::vector<RNode*> regions;
    
  
  static std::pair<RNode*, RNode*> split(RNode*);
  static MBB regionsMbb( std::vector<RNode*> regions);

  inline bool isLeaf() const {return regions.empty();}    



  RNode(){
    myFigures.reserve(ORDER + 1);
    regions.reserve(ORDER + 1);
  }

  void update(std::vector<RNode*> reg, MBB mb){
    regions = reg; bound = mb;
    cur_figs = regions.size();
  }

  RNode* search(Figure *);
  RNode* chooseSubtree(Figure *);
    
  bool insert(Figure *);
  void handleOverflow(Figure *f);
};





class Rtree{
  
  private:

    RNode* root;

  public:
    
    Rtree(){
      root =  new RNode;
    };
    

    RNode* search(Figure *);    
    bool insert(Figure *);




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
