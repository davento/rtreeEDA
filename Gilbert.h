#ifndef GILBERT_H
#define GILBERT_H


#include "Display.h"
#include "Point.h"


#define SIDEDIM 1024


void rot(int n, int *x, int *y, int rx, int ry) {
    if (ry == 0) {
        if (rx == 1) {
            *x = n-1 - *x;
            *y = n-1 - *y;
        }

        //Swap x and y
        int t  = *x;
        *x = *y;
        *y = t;
    }
}


int xy2d ( const Point& p) {
    int n = SIDEDIM;
    int rx, ry, s, d=0;
    int x = p[X];
    int y = p[Y];
    for (s=n/2; s>0; s/=2) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(n, &x, &y, rx, ry);
    }
    return d;
}



#endif