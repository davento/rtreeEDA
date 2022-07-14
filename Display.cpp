#include "Display.h"
#include <algorithm>
#include <random>
#include <set>
std::random_device dev;
std::mt19937 gen(dev());
int random(int low, int high){
    //std::mt19937 gen(dev());
    std::uniform_int_distribution<int> dist(low, high);
    return dist(gen);
}

double random(double low, double high){

    std::uniform_real_distribution<double> dist(low, high);
    return dist(gen);
}


Point createRandomPoint(Point leading, int bound){
    return Point(random(leading.x, leading.x + bound), random(leading.y, leading.y + bound));
}

Point createRandomPoint(Point leading, int maxBound, int minBound){
    return Point(random(leading.x+minBound, leading.x+maxBound), random(leading.y+minBound, leading.y+maxBound));
}

Figure Display::generateRandomFigure(int size){
    bool flag = true;
    Figure result;
    while(flag){
    Figure resultwhile;
    double min_bounding_length = size*0.01;
    double bounding_length = size*0.02;

    int numberOfPoints = random(3,10); //get random number between 3 - 10

    Point leadingPoint = createRandomPoint(Point(0,0), size - bounding_length); // get a point that fits the window boundings
    
    //leadingPoint.print(); 
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
        resultwhile.addPoint(p);
    }
    resultwhile.addPoint(points[0]);
    MBC gb = resultwhile.getCopyBound();
    bool goodEnough = (gb.getBottomRight().x - gb.getTopLeft().x ) > min_bounding_length && (gb.getBottomRight().y - gb.getTopLeft().y ) > min_bounding_length;
    if(goodEnough){
        result = resultwhile;
        flag = false;
    }
    }

    return result;

}

struct Limits{
    std::size_t begin, end;
};

struct Region{
    Limits x, y;
};


std::vector<MBC> getMBCs(const std::vector<Figure*>& figures){
    std::vector<MBC> result;
    for(const auto& figure: figures){
        result.push_back(figure->getCopyBound());
    }
    return result;
}
std::vector<MBC> getMBCs(const std::vector<Figure>& figures){
    std::vector<MBC> result;
    for(const auto& figure: figures){
        result.push_back(figure.getCopyBound());
    }
    return result;
}

Region getLimits(const MBC& mbc, std::vector<double>& xValues, std::vector<double>& yValues){
    Region result;
    for(std::size_t i = 0; i != xValues.size(); ++i){
        if(xValues[i] == mbc.getTopLeft().x)
            result.x.begin = i;
        else if(xValues[i] == mbc.getBottomRight().x)
            result.x.end = i;
    }
    for(std::size_t i = 0; i != yValues.size(); ++i){
        if(yValues[i] == mbc.getTopLeft().y)
            result.y.begin = i;
        else if(yValues[i] == mbc.getBottomRight().y)
            result.y.end = i;
    }
    return result;
}
double getUnion_intersection(std::vector<MBC>& mbcs){
    double unionArea = 0, intersectionArea = 0;
    std::set<double> xValues, yValues;
    //xValues.insert(0); yValues.insert(0);
    for(const auto& mbc: mbcs){
        Point tl = mbc.getTopLeft();
        Point br = mbc.getBottomRight();
        xValues.insert(tl.x); xValues.insert(br.x);
        yValues.insert(tl.y); yValues.insert(br.y);
    }
    std::vector<double> xSegments(xValues.begin(), xValues.end()), ySegments(yValues.begin(), yValues.end());
    xValues.clear(); yValues.clear();
    std::vector<std::vector<std::size_t>> matrix (ySegments.size(), std::vector<std::size_t>(xSegments.size(), 0));
    for(const auto& mbc: mbcs){
        Region limits = getLimits(mbc, xSegments, ySegments);
        for(std::size_t i = limits.y.begin; i != limits.y.end; ++i){
            for(std::size_t j = limits.x.begin; j != limits.x.end; ++j){
                matrix[i][j] +=1;
            }
        }
    }
    for(std::size_t i = 0; i != matrix.size()-1; ++i){
        for(std::size_t j = 0; j != matrix[i].size()-1; ++j){
            if(matrix[i][j] > 0)
                unionArea += (xSegments[j+1] - xSegments[j])*(ySegments[i+1]-ySegments[i]);
            if(matrix[i][j] > 1)
                intersectionArea += (xSegments[j+1] - xSegments[j])*(ySegments[i+1]-ySegments[i]);
        }
    }
    return intersectionArea/unionArea;
}

void coeficienteSolapamiento(Rtree* r){
    std::vector<MBC> figuresMBC = getMBCs(r->getFigures());
    std::cout << getUnion_intersection(figuresMBC) << std::endl;
}
void coeficienteSolapamiento(std::vector<Figure>& figures){
    std::vector<MBC> figuresMBC = getMBCs(figures);
    std::cout << getUnion_intersection(figuresMBC) << std::endl;
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
         //std::cout<<"out\n";s
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
    static std::size_t i= 1;


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
                //int x, y;
                //SDL_GetMouseState(&x, &y);
                //Point po(x,y);
                //figures = ss->depthFirst(po);
                //std::cout << figures.size() << std::endl;
                quit = true;
            }
        }
    }
    /*figures.push_back(generateRandomFigure());
    std::cout<<i++<<",";
    coeficienteSolapamiento(figures);*/
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE])
        isRunning = false;
    else if(state[SDL_SCANCODE_Z]){
        std::cout<<"random inserted num: "<<i++<<'\n';
        ss->insert(generateRandomFigure(screenSize));
    }
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
        f.draw(renderer, Color(159,43,104));
    }
    //figures.clear();

    SDL_RenderPresent(renderer);
}
