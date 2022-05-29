#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include "structs/rtree.h"

class Display{

    public:

        Display();
        bool initialize(double dim);
        void runLoop();
        void shutdown();

    private:

        void processInputs();
        void updateDisplay();
        void generateOutput(); 

        bool isRunning;
        SDL_Window* window;
        SDL_Renderer* renderer;
        Rtree* r;
        std::vector<Figure> figures;
        Figure fig;
};

#endif

