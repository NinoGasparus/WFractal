#include "keyEvents.h"
#include "../globalVars.h"

#include  <stdio.h>
#include  <iostream>
#include <fstream>
#include <string>
#include <cmath>

int stepFactor = 10;
void keyEvents::W(){
    somethingChanged = true;
    double deltaY = (ny / z) - (py / z);
    yoff += deltaY / stepFactor;
    printf("W\n");
}
void keyEvents::A(){
    somethingChanged = true;
    double deltaX = (nx / z) - (px / z);
    xoff += deltaX / stepFactor;
    printf("A\n");
}
void keyEvents::S(){
    double deltaY = (ny / z) - (py / z);
    yoff -= deltaY / stepFactor;
    somethingChanged = true;
    printf("S\n");
}
void keyEvents::D(){
    double deltaX = (nx / z) - (px / z);
    xoff -= deltaX / stepFactor;
    somethingChanged = true;
printf("D\n");
}
void keyEvents::F(){
    somethingChanged = true;
printf("F\n");
}

void keyEvents::Left(){
    splitfactor -= 0.05;
    if(splitfactor -0.05 <=0){
        printf("split min\n");
        splitfactor = 0;
    }
    splitIndex = (int)maxIteration*splitfactor;
    somethingChanged = true;
}
void keyEvents::Right(){

    splitfactor +=0.05;
    if(splitfactor +0.05>=1){
        printf("split MAx\n");
        splitfactor= 0.99;
    }
    splitIndex = (int)maxIteration*splitfactor;
    somethingChanged = true;
}

void keyEvents::J(){
    iterat++;
    maxIteration=std::pow(iterat,1/0.8);
    splitIndex = (int)maxIteration*splitfactor;
    somethingChanged = true;
    //resume = true;
    printf("J\n");
}
void keyEvents::K(){
    iterat--;
    maxIteration=std::pow(iterat,1/0.8);
    splitIndex = (int)maxIteration*splitfactor;
    somethingChanged = true;
    printf("K\n");
}
void keyEvents::Num0(){
    maxIteration =1000;
    splitIndex = maxIteration * splitfactor;
    xoff = 0;
	yoff = 0;
    
	z = 3;
    somethingChanged = true;
    printf("0\n");
}



void keyEvents::Space(){
    somethingChanged = true;
    printf("space\n");
}

void keyEvents::Shift(sf::Event ev){
    somethingChanged = true;
    switch(ev.key.code){
        case sf::Keyboard::Num1:
            formula = 1;
            break;
        case sf::Keyboard::Num2:
            formula = 2;
            break;
        case sf::Keyboard::Num3:
            formula = 3;
            break;
    }
    std::cout << ev.key.code <<std::endl;
}
void keyEvents::Ctrl(sf::Event ev) {
    std::ifstream file;
    std::string fragmentShaderCode;

    switch (ev.key.code) {
        case sf::Keyboard::Num1:
            file.open("shaders/basic.frag");
            break;
        case sf::Keyboard::Num2:
            file.open("shaders/blueBernstein.frag");
            break;
        case sf::Keyboard::Num3:
            file.open("shaders/darkRed.frag");
            break;
        case sf::Keyboard::Num4:
            file.open("shaders/green.frag");
            break;
        default:
            return; // Do nothing for other keys
    }

    if (file.is_open()) {
        fragmentShaderCode.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        shader.loadFromMemory(fragmentShaderCode, sf::Shader::Fragment);
        file.close();
    }
}


void keyEvents::Up(){
    maxRecursion++;
    std::cout <<maxRecursion <<std::endl;
    somethingChanged = true;
}
void keyEvents::Down(){
    maxRecursion--;
    std::cout <<maxRecursion << std::endl;
    somethingChanged = true;
}