#ifndef _FIGURES_HPP_
#define _FIGURES_HPP_

#include <vector>


struct Point{
    int x,y;
};

struct MBB{

    //top left, bottom right
    Point top_l, bottom_r;

};

struct figures{

    private:
        std::vector<Point> points;

    public:

        MBB getMbb(){
            
            MBB mbb;

            Point tl;
            Point br;

            for(auto p: points){
                tl.x = std::min(tl.x, p.x);
                tl.y = std::min(tl.y, p.y);

                br.x = std::max(br.x, p.x);
                br.y = std::max(br.y, p.y);
            }

        }

};

#endif