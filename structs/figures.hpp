#ifndef _FIGURES_HPP_
#define _FIGURES_HPP_

#include <vector>


struct Point{
    int x, y;

    static inline Point max(const Point &m1, const Point &m2){
        return{
            std::max(m1.x, m2.x),
            std::max(m1.y, m2.y)
        };
    }

    static inline Point min(const Point &m1, const Point &m2){
        return{
            std::min(m1.x, m2.x),
            std::min(m1.y, m2.y)
        };
    }

};

struct MBB{

    //top left, bottom right
    Point topLeft, bottomRight;

    static void  merge(MBB& target, const MBB& source){
        target.topLeft = Point::min(target.topLeft, source.topLeft);
        target.bottomRight = Point::max(target.bottomRight, source.bottomRight);
    }

};

struct Figure{

    private:
        
        std::vector<Point> points;
        MBB bound;

        friend class Rtree;

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
