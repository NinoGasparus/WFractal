#include <SFML/Graphics.hpp>

#include "helperFunctions/helperFunctions.h"
#include "engine/threadManager.cpp"
#include "keyEvents/keyEvents.h"
#include "globalVars.h"


#include <iostream>
#include <stdio.h>
#include <thread>
#include <queue> 
#include <condition_variable> 
#include <mutex> 
#include <chrono>

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
int 	realLine;
bool 	top;
bool 	 somethingChanged;
sf::Uint8* pictureData;
sf::Texture picture;
sf::Sprite sprite;
sf::Shader shader;

int formula;
bool stopThreads;
std::vector<std::thread> threads; 
std::queue<std::tuple<int, int, int, int, bool,bool, int, double, double , double>> segmentQueue;
std::condition_variable queueCondition;
std::mutex queueMutex; 

int main(){
    helperFunctions::displayUI();
    helperFunctions::initVars();
    helperFunctions::initDataArray();

	//first parameter is number of  threads, second is amount of splits horizontally and veritcally, 8 is ideal.
	startThreads(8,8);
	//called once to compute the first frame. 
	reDraw(8);

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "WFractal");
	sf::View view(sf::FloatRect(0, 0, 1920, 1080));
    window.setSize(sf::Vector2u(width,height));
    window.setFramerateLimit(60); 
	window.setView(view);
	
	auto prevFrameTime = std::chrono::high_resolution_clock::now();
	auto curFrameTime = std::chrono::high_resolution_clock::now();
	while (window.isOpen())
	{
		curFrameTime = std::chrono::high_resolution_clock::now();
		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(curFrameTime-prevFrameTime).count() << std::endl;
		prevFrameTime = std::chrono::high_resolution_clock::now();
		sf::Event event;
		//if image was not completeley done last frame it gets the new data
		fetchData();
		while (window.pollEvent(event))
		{	
					
			switch(event.type){
				case sf::Event::Closed:
					killThreads();
					window.close();
					goto end;

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
						
						case sf::Keyboard::Q:
							killThreads();
							window.close();
							break;
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
				
				case sf::Event::MouseWheelScrolled:	
						if (event.mouseWheelScroll.delta > 0){
						z++;
						somethingChanged = true;
						}
						else if (event.mouseWheelScroll.delta < 0){
						z--;
						somethingChanged = true;
						}
						std::cout << z << std::endl;
					break;

    		}
        }
		if(somethingChanged){
		for(int i =0; i < width; i++){
			for(int j =0; j < height; j++){
				
				data[i][j][0] = 0;
				data[i][j][1] = 0;
				data[i][j][2] = 0;
			}
		}
			reDraw(8);
			somethingChanged = false;
		}
       	window.clear();
		window.draw(sprite, &shader);
		
		window.display();
	}
	end:
    return 0;
}