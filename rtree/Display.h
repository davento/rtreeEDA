#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include "Rtree.h"
#include "Figure.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <iostream>

class Display{

    public:

        Display();
        bool initialize(double dim);
        void runLoop();
        void shutdown();
        static Figure generateRandomFigure(int );

    private:

        void processInputs();
        void updateDisplay();
        void generateOutput(); 
        bool isRunning;
        SDL_Window* window;
        SDL_Renderer* renderer;
        Rtree* ss;
        Figure fig;
        std::vector<Figure> figures;
        std::vector<Figure*> depthFirst;
        int screenSize;

};


int random(int low, int high);

#endif

