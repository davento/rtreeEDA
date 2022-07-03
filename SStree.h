#ifndef SSTREE_H
#define SSTREE_H

#include "SStreeNode.h"
#include "Point.h"
#include <SDL2/SDL.h>

class SStree final{
    public:
        SStree();
        void insert(Point p);
        void remove(Point p);
        void draw(SDL_Renderer* renderer) const;
    private:
        SStreeNode root;
};

#endif

