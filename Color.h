#ifndef COLOR_H
#define COLOR_H
#include <cstdint>
#include <iostream>

struct Color final{
    std::uint8_t RGB[3];
    size_t indexToChange=1;

    Color(unsigned char R, unsigned char G, unsigned char B): RGB{R,G,B}{
    }

    void changeColor(int value){
        RGB[indexToChange] += value;
        indexToChange = (indexToChange+1) % 3;
    }

    void print() const {
        std::cout << "R: " << (int)RGB[0] << " G: " << (int)RGB[1] << " B: " << (int)RGB[2] << "\n";
    }
};


#endif

