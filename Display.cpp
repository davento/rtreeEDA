#include "Display.h"

Display::Display(): isRunning(false), window(nullptr), renderer(nullptr), ss(nullptr){}

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
    
    ss = new Rtree();

    return true;
}

void Display::runLoop(){
    while(isRunning){
        // std::cout<<"input\n";
        processInputs();
        // std::cout<<"diplay\n";
        updateDisplay();
        // std::cout<<"out\n";s
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
                    if(!fig.addPoint(Point(x,y)) ){
                           ss->insert(fig);
                           fig.clear();
                    }
                }
                if(event.button.button == SDL_BUTTON_RIGHT){
                    ss->remove(Point(x,y));
                  
                }
            }
            else{
                int x, y;
                SDL_GetMouseState(&x, &y);
                Point po(x,y);
                // figures = r->depthFirst(po);
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
    // std::cout<<"out1\n";
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // std::cout<<"out2\n";
    SDL_RenderClear(renderer);
    fig.draw(renderer);

    // std::cout<<"out3\n";
    ss->draw(renderer);
    // std::cout<<"out4\n";
    SDL_RenderPresent(renderer);
}
