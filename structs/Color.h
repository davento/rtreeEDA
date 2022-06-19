#ifndef COLOR_H
#define COLOR_H
#include <iostream>
#include <SDL2/SDL_stdinc.h>
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


#endif

