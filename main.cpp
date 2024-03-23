#include <SFML/Graphics.hpp>

#include "helperFunctions/helperFunctions.h"
#include "engine/threadManager.cpp"
#include "keyEvents/keyEvents.h"
#include "globalVars.h"


#include <iostream>
#include <stdio.h>

int      width;
int      height;
double   nx;
double   px;
double   ny;
double   py;
long     z;
double   xoff;
double   yoff;
long     maxIteration;
int 	 iterat;
int      res;
double *** data;
bool 	 somethingChanged;
sf::Uint8* pictureData;
sf::Texture picture;
sf::Sprite sprite;
sf::Shader shader;
int main(){
    helperFunctions::displayUI();
    helperFunctions::initVars();
    helperFunctions::initDataArray();


	

    startThreads();
	//called once to compute the first frame. 
	reDraw();

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "WFractal");
	sf::View view(sf::FloatRect(0, 0, 1920, 1080));
    window.setSize(sf::Vector2u(width,height));
    window.setFramerateLimit(60); 
	window.setView(view);
	
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch(event.type){
				case sf::Event::Closed:
					window.close();break;

				case  sf::Event::KeyPressed:						
					switch(event.key.code){
						//Basic single key instructions
						case sf::Keyboard::W:
							keyEvents::W();break;
						case sf::Keyboard::A:
							keyEvents::A();break;
						case sf::Keyboard::S:
							keyEvents::S();break;
						case sf::Keyboard::D:
							keyEvents::D();break;
						case sf::Keyboard::F:
							keyEvents::F();break;
						case sf::Keyboard::J:
							keyEvents::J();break;
						case sf::Keyboard::K:
							keyEvents::K();break;
						case sf::Keyboard::Num0:
							keyEvents::Num0();break;
						case sf::Keyboard::Space:
							keyEvents::Space();break;
						//Multikey combinations

						case sf::Keyboard::Num1:
						case sf::Keyboard::Num2:
						case sf::Keyboard::Num3:
							//In case that Shift+1/2/3 was pressed
							if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
								keyEvents::Shift(event);
								break;
							}
						case sf::Keyboard::Num4:
						case sf::Keyboard::Num5:
						case sf::Keyboard::Num6:
						case sf::Keyboard::Num7:
						case sf::Keyboard::Num8:
						case sf::Keyboard::Num9:
							//In case that Ctrl+ 1-9 was pressed
							if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
								keyEvents::Ctrl(event);
								break;
							}
							break;
					}

				break;
    		}
        }
		if(somethingChanged){
			reDraw();
			somethingChanged = false;
		}
       	window.clear();
		window.draw(sprite, &shader);
		
		window.display();
	}

    return 0;
}