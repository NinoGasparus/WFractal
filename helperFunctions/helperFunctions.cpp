#include "helperFunctions.h"
#include "../globalVars.h"
#include <SFML/Graphics.hpp>

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>

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
        realLine = 540; //hardcoded since default will do 
        z =3;
        iterat  = 10;
        maxIteration = iterat*iterat;        
        maxRecursion = 4;
        curFrame =0;
        completeThreads= 0;
        somethingChanged = false;
        formula = 1;
        pictureData = new sf::Uint8[1080 * 1920 * 4];
        picture.create(1920, 1080);
        sprite.setTexture(picture,true);
        splitIndex = 90;
        splitfactor = 0.97;
        curFrame = 0;

        std::ifstream file("shaders/blueBernstein.frag");
        std::string fragmentShaderCode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        shader.loadFromMemory(fragmentShaderCode, sf::Shader::Fragment);

        stopThreads = false;
}

void helperFunctions::initDataArray(){
        data = new double**[width];
        for (int i = 0; i < width; ++i) {
                data[i] = new double*[height];
                for (int j = 0; j < height; ++j) {
                        data[i][j] = new double[4];
                }
        }
}