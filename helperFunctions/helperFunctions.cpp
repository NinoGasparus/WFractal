#include "helperFunctions.h"
#include "../globalVars.h"

#include <iostream>
#include <stdio.h>

double helperFunctions::mapValue(double X, double X_min, double X_max, double Y_min, double Y_max){
        return ((X - X_min) / (X_max - X_min)) * (Y_max - Y_min) + Y_min;
}


void helperFunctions::displayUI(){
        printf("=================================================\n");
        printf("                     WFractal                    \n");
        printf("\n\n\n\n\n");
        printf("Opis                                   Tipka\n");
        printf("================================================\n");
        printf("Za premikanje:                         WASD\n");
        printf("Za spremembo barvnega algoritma:       CTRL+1...9\n");
        printf("Za spremebo fraktala:                  SHIFT+1/2/3\n");
        printf("Zajem vidnega polja:                   F\n");
        printf("Povečaj/manjšaj iteracije:             J/K\n");
        printf("Ponastavi vidno polje:                 0\n");
        printf("Prosti pad:                            SPACE\n");
}

void helperFunctions::initVars(){
        yoff = 0;
        xoff = 0;
        width = 1920;
        height = 1080;
        nx = (-8);
        ny = (-4.5);
        px = (8);
        py = (4.5);

        z =3;
        maxIteration = 1000;

}

void helperFunctions::initDataArray(){
        data = new double**[width];
        for (int i = 0; i < width; ++i) {
                data[i] = new double*[height];
                for (int j = 0; j < height; ++j) {
                        data[i][j] = new double[3];
                }
        }
}