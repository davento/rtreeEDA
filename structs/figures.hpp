#ifndef _FIGURES_HPP_
#define _FIGURES_HPP_

#include <vector>


struct Point{
    int x, y;

};

struct MBB{

    //top left, bottom right
    Point topLeft, bottomRight;

};

struct Figure{

    private:
        
        std::vector<Point> points;
        MBB bound;

    public:

        Figure(Point& p){
            // TODO: sdl input, overload == and != for the point struct
            
            points.push_back(p);
            bound.bottomRight.x = bound.topLeft.x = p.x;
            bound.bottomRight.y = bound.topLeft.y = p.y;
            Point other;
            
            //get other point from sdl input
            
            while(other != p){

                points.push_back(other);
                
                bound.topLeft.x = std::min(bound.topLeft.x, other.x);
                bound.bottomRight.x = std::max(bound.bottomRight.x, other.x);
                bound.topLeft.y = std::min(bound.topLeft.y, other.y);
                bound.bottomRight.y = std::max(bound.bottomRight.y, other.y);

                // get other point in sdl

            }
        }

};

#endif
