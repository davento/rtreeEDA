#include "Display.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <iostream>
#include "structs/Figure.h"

Display::Display(): isRunning(false), window(nullptr), renderer(nullptr), r(nullptr), fig(new MBC()){}

bool Display::initialize(double dim){

    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if(sdlResult != 0){
        SDL_Log("No se pudo inicializar SDL: %s", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow("Rtree", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,0);
    if(!window){
        SDL_Log("Falla en la creación de la ventana: %s", SDL_GetError());
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    isRunning = true;
    
    r = new Rtree<MBC,3>;

    return true;

}

void Display::runLoop(){
    while(isRunning){
        printf("input\n");
        processInputs();
        printf("Display\n");
        updateDisplay();
        printf("Output\n");
        generateOutput();
    }
}

void Display::shutdown(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Display::processInputs(){
    SDL_Event event;
    bool quit = false;
    std::cout<<"A\n";


    while (!quit){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                quit = true;
                isRunning = false;
            }
            
            else if(event.type == SDL_MOUSEBUTTONDOWN){
                int x, y;
            
                SDL_GetMouseState(&x, &y);
                if(event.button.button == SDL_BUTTON_LEFT){
                    if(!fig.addPoint(Point(x,y))){
                       r->insert(&fig);
                       fig.clear();
                    }
                }
                if(event.button.button == SDL_BUTTON_RIGHT){
                    r->remove(Point(x,y));
                    fig.clear();
                }
            }
            else{
                int x, y;
                std::cout << "default" << std::endl; 
                SDL_GetMouseState(&x, &y);
                Point po(x,y);
                figures = r->depthFirst(po);
                //std::cout << figures.size() << std::endl;
                quit = true;
            }
        }
    }
    
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE])
        isRunning = false;
}

void Display::updateDisplay(){

}

void Display::generateOutput(){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    fig.draw(renderer);
    //for(const auto& figure: figures)
    //    figure.draw(renderer);
    r->draw(renderer);
    r->drawDepthFirst(renderer, figures);
    SDL_RenderPresent(renderer);
}
