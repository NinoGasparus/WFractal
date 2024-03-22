#include <SFML/Graphics.hpp>

#include "helperFunctions/helperFunctions.h"
#include "engine/threadManager.cpp"
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
int      res;
double *** data;
int main(){
    helperFunctions::displayUI();
    helperFunctions::initVars();
    helperFunctions::initDataArray();
    startThreads();


    sf::RenderWindow window(sf::VideoMode(1920, 1080), "WFractal");

	sf::Texture slika;
	slika.create(1920, 1080);

	sf::Uint8* data2 = new sf::Uint8[1080 * 1920 * 4];
		
    for (int i = 0; i < width; i += 1) {
        for ( int j = 0; j < height; j += 1) {
            int index = j * 1920 * 4 + i * 4;
            

            int delta = 0;
            
            delta = (int)helperFunctions::mapValue(data[i][j][0],0,maxIteration,0,255);

            data2[index]   = sf::Uint8(delta);
            data2[index+1] = sf::Uint8(delta);
            data2[index+2] = sf::Uint8(delta);
            data2[index+3] = sf::Uint8(255);
        }
    }
	
	slika.update(data2);
	sf::Sprite spr(slika);

    window.setSize(sf::Vector2u(1920,1080));
    window.setFramerateLimit(60); 

	int pw =width;
	int ph =height;
	bool somethinghappened = false;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch(event.type){
				case sf::Event::Closed:
					window.close();break;
    		}
        }

       	window.clear();
		window.draw(spr);
		sf::View view(sf::FloatRect(0, 0, 1920, 1080));
		window.setView(view);
		window.display();
	}

    return 0;
}