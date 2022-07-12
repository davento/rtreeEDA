#include "Display.h"
#include <algorithm>
#include <random>
std::random_device dev;
int random(int low, int high){
    std::uniform_int_distribution<int> dist(low, high);
    return dist(dev);
}


Point createRandomPoint(Point leading, int bound){
    return Point(random(leading.x, leading.x + bound), random(leading.y, leading.y + bound));
}

Figure Display::generateRandomFigure(){
    
    Figure result;
    int bounding_length = screenSize*0.03;
    std::cout << bounding_length << std::endl;
    int numberOfPoints = random(3,10); //get random number between 3 - 10

    Point leadingPoint = createRandomPoint(Point(0,0), screenSize - bounding_length); // get a point that fits the window boundings
    leadingPoint.print(); 
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


Display::Display(): isRunning(false), window(nullptr), renderer(nullptr), ss(nullptr){}

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
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    isRunning = true;
    
    screenSize = dim;
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
                    /*if(!fig.addPoint(Point(x,y)) ){
                           ss->insert(fig);
                           fig.clear();
                    }*/
                   ss->insert(generateRandomFigure());
                }
                if(event.button.button == SDL_BUTTON_RIGHT){
                    ss->remove(Point(x,y));
                  
                }
            }
            else{
                int x, y;
                SDL_GetMouseState(&x, &y);
                Point po(x,y);
                figures = ss->depthFirst(po);
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
    
    // std::cout<<figures.size()<<'\n';
    for(auto f: figures){
        f->draw(renderer, Color(159,43,104));
    }
    figures.clear();

    SDL_RenderPresent(renderer);
}
