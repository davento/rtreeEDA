#include "Display.h"
#include <SDL2/SDL.h>
#include <iostream>

Display::Display(): isRunning(false), window(nullptr), renderer(nullptr), r(nullptr){}

bool Display::initialize(double dim){

    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if(sdlResult != 0){
        SDL_Log("No se pudo inicializar SDL: %s", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow("Rtree", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dim, dim,0);
    if(!window){
        SDL_Log("Falla en la creación de la ventana: %s", SDL_GetError());
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    isRunning = true;
    
    r = new Rtree;

    return true;
}

void Display::runLoop(){
    while(isRunning){
        processInputs();
        updateDisplay();
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
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    if(event.button.button == SDL_BUTTON_LEFT){
                       if(!fig.addPoint({x,y})){
                           r->insert(&fig);
                           fig.clear();
                       }
                    }
                    if(event.button.button == SDL_BUTTON_RIGHT){
                        r->remove({x,y});
                        fig.clear();
                    }
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
    SDL_RenderPresent(renderer);
}
