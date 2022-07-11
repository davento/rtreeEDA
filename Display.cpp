#include "Display.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <random>
std::random_device dev;
int random(int low, int high){
    std::uniform_int_distribution<int> dist(low, high);
    return dist(dev);
}

Display::Display(): isRunning(false), window(nullptr), renderer(nullptr), r(nullptr){}

bool Display::initialize(double dim){
    screenSize = dim;
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

Point createRandomPoint(Point leading, int bound){
    return {random(leading.x, leading.x + bound), random(leading.y, leading.y + bound)};
}

Figure Display::generateRandomFigure(){
    Figure result;
    int bounding_length = screenSize*0.03;

    int numberOfPoints = random(3,10); //get random number between 3 - 10

    Point leadingPoint = createRandomPoint({0,0}, screenSize - bounding_length); // get a point that fits the window boundings
    
    std::vector <Point> points;
    double middleX = 0, middleY = 0;

    for(int i = 0; i < numberOfPoints; ++i){
        Point newPoint = createRandomPoint(leadingPoint, bounding_length);
        middleX += newPoint.x;
        middleY += newPoint.y;
        points.push_back(newPoint);
        
    }
    middleX /= numberOfPoints*1.0;
    middleY /= numberOfPoints*1.0;

    Point middlePoint(middleX, middleY);

    auto sortByAngle = [middlePoint](const Point& a, const Point& b) -> bool {
        Point A = a - middlePoint;
        Point B = b - middlePoint;
        return std::acos(A.x/A.length())*A.y/std::abs(A.y) < std::acos(B.x/B.length())*B.y/std::abs(B.y);

    };
    std::sort(points.begin(), points.end(), sortByAngle );
    for(const auto& p: points){
        result.addPoint(p);
    }
    result.addPoint(points[0]);
    return result;

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
                    /*   if(!fig.addPoint({x,y})){
                           r->insert(&fig);
                           fig.clear();
                       }*/
                     Figure temp = generateRandomFigure();
                    r->insert(&temp);

                    }
                                       if(event.button.button == SDL_BUTTON_RIGHT){
                        r->remove({x,y});
                        fig.clear();
                    }
                    break;
                }
            default:
                {
                    int x, y;
                    
                    SDL_GetMouseState(&x, &y);
                    Point po(x,y);
                    r->depthFirst(&po);
                    break;
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
    r->drawDepthFirst(renderer);
    SDL_RenderPresent(renderer);
}
