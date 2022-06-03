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

  void print() const {
    std::cout << "----------\n";
    std::cout << "number of regions: "<< regions.size() << "\n";
    std::cout << "number of figures: "<< myFigures.size() << "\n";
    std::cout << "perimeter: " << bound.Perimeter() << "\n";
    std::cout << "----------\n";
    if(regions.size()){
      std::cout << "children: {\n";
      for(const auto &region: regions)
        region->print();
      std::cout << "}\n";
    }
  }

  template<class cnt>
  static MBB regionsMbb( cnt c);
  
  inline bool isLeaf() const {return regions.empty();}    

  MBB getBound() const {return bound;}

  RNode(): father(nullptr){
    myFigures.reserve(ORDER + 1);
    regions.reserve(ORDER + 1);
  }

  void update(std::vector<RNode*> reg, MBB mb){
    regions = reg; bound = mb;
    cur_figs = regions.size();
  }

  void update(std::vector<Figure*> reg, MBB mb){
    myFigures = reg; bound = mb;
    cur_figs = myFigures.size();
  }


  RNode* search(Figure *){return nullptr;}

  
  void draw(SDL_Renderer* renderer, Color color) const {
      if(isLeaf()&& !myFigures.empty()){
        for(const auto& figure: myFigures)
          figure->draw(renderer);
      }
      bound.draw(renderer, color);
      color.changeColor(150);
      if(!regions.empty()){
        for(const auto& region: regions)
          region->draw(renderer, color);
      }
    }


};

class Rtree{
  
  private:

    RNode* root;
    RNode* search(RNode*, Point);

  public:
    
    Rtree(){
      root =  new RNode;
    };
    

    RNode* search(Point);    
    bool insert(Figure *);
    void remove(Point p);

    void draw(SDL_Renderer* renderer) const {
      Color color(0,40,0);
      root->draw(renderer, color);
    }

};


#endif
