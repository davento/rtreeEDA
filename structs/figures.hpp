#ifndef _FIGURES_HPP_
#define _FIGURES_HPP_

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <vector>
#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>

struct Color{
    Uint8 RGB[3];
    size_t indexToChange=1;

    Color(int R, int G, int B){
        RGB[0] = R;
        RGB[1] = G;
        RGB[2] = B;
    }

    void changeColor(int value){
        RGB[indexToChange] += value;
        indexToChange = (indexToChange+1) % 3;
    }

    void print(){
        std::cout << "R: " << (int)RGB[0] << " G: " << (int)RGB[1] << " B: " << (int)RGB[2] << "\n";
    }
};

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

    Point(int pos_x, int pos_y): x(pos_x), y(pos_y){
        
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
       // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
       // SDL_RenderDrawPoint(renderer, x, y);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        int dim = 1;
        SDL_Rect dot{
            static_cast<int>(x - dim),
            static_cast<int>(y - dim),
            3*dim,
            3*dim
        };
        SDL_RenderFillRect(renderer, &dot);
    }   
};

struct MBB{

    //top left, bottom right
    Point topLeft, bottomRight;
    
    MBB(): topLeft({40000,40000}), bottomRight({0,0}){}


    int Perimeter() const {
        
        int l = this->bottomRight.x-this->topLeft.x;
        int w = this->bottomRight.y-this->topLeft.y;

        return std::abs(2*(l+w));
    }

    
    void draw(SDL_Renderer* renderer) const {
        if(topLeft.x == 40000 && topLeft.y == 40000)
            return;
        const int &lx = topLeft.x, &ty = topLeft.y, &rx = bottomRight.x, &by = bottomRight.y;
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderDrawLine(renderer, lx, ty, rx , ty);
        SDL_RenderDrawLine(renderer, lx, ty, lx , by);
        SDL_RenderDrawLine(renderer, lx, by, rx , by);
        SDL_RenderDrawLine(renderer, rx, ty, rx , by);
    }

    void draw(SDL_Renderer* renderer, Color color) const {
        if(topLeft.x == 40000 && topLeft.y == 40000)
            return;
        const int &lx = topLeft.x, &ty = topLeft.y, &rx = bottomRight.x, &by = bottomRight.y;
        SDL_SetRenderDrawColor(renderer, color.RGB[0], color.RGB[1], color.RGB[2], 255);
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

        Figure(){}; 

        MBB getBound() const { return bound; }
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
            if(!points.size())
                return;
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
