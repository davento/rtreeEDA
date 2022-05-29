#ifndef _FIGURES_HPP_
#define _FIGURES_HPP_

#include <SDL2/SDL_render.h>
#include <vector>
#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>

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

    friend bool operator==(const Point& left, const Point& other){
        return left.x == other.x && left.y == other.y;
    }
    friend bool operator!=(const Point& left, const Point& other){
        return !(left == other);
    }

    double length(const Point& other){
        return std::sqrt(std::pow(other.x - x, 2) + std::pow(other.y - y, 2));
    }

    bool closeEnough(const Point& other){
        const int RADIO = 4;
        if(length(other) < RADIO*2)
            return true;
        return false;

    }
    void draw(SDL_Renderer* renderer) const {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawPoint(renderer, x, y);
    }   
};

struct MBB{

    //top left, bottom right
    Point topLeft, bottomRight;
    
    MBB(): topLeft({40000,40000}), bottomRight({0,0}){}
    void draw(SDL_Renderer* renderer) const {
        const int &lx = topLeft.x, &ty = topLeft.y, &rx = bottomRight.x, &by = bottomRight.y;
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderDrawLine(renderer, lx, ty, rx , ty);
        SDL_RenderDrawLine(renderer, lx, ty, lx , by);
        SDL_RenderDrawLine(renderer, lx, by, rx , by);
        SDL_RenderDrawLine(renderer, rx, ty, rx , by);
    }

    static MBB merge(const MBB& m1, const MBB& m2){
        MBB target;
        target.topLeft = Point::min(m1.topLeft, m2.topLeft);
        target.bottomRight = Point::max(m1.bottomRight, m2.bottomRight);
        return target;
    }

    void clear(){
        topLeft = {40000,40000};
        bottomRight = {0,0};
    }


};

struct Figure{

    private:
        
        std::vector<Point> points;
        MBB bound;

        friend class Rtree;

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
        
    void updateBound(const Point& newPoint){
        bound.topLeft = min(bound.topLeft, newPoint);
        bound.bottomRight = max(bound.bottomRight, newPoint);
    }
    
    public:

        Figure() = default; 

        bool addPoint(const Point& p){
            if(!points.size()){
                points.push_back(p);
                updateBound(p);
                return true;

            }
            if(points[0].closeEnough(p)){
                points.push_back(points[0]);
                return false;
            }
            else{
                points.push_back(p);
                updateBound(p);
                return true;
            }
            
        }
        void clear(){
            points.clear();
            bound.clear(); 
        }

        void draw(SDL_Renderer* renderer) const {
            for(const auto& point: points)
                point.draw(renderer);
            if(points.size() <= 1)
                return;
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            for(size_t i = 0; i != points.size() - 1; i++)
                SDL_RenderDrawLine(renderer, points[i].x, points[i].y, points[i+1].x, points[i+1].y);
            bound.draw(renderer);
        }

};

#endif
