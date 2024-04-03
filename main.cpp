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
#include <atomic>
#include <string>
#include <cstdint>

int      width;
int      height;
double   nx;
double   px;
double   ny;
double   py;
long     z;
double   xoff;
double   yoff;
uint16_t     maxIteration;
int 	 iterat;
int      res;
double *** data;
int 	realLine;
bool 	top;
long curFrame;
bool resume;
int completeThreads;
bool 	 somethingChanged;
sf::Uint8* pictureData;
sf::Texture picture;
sf::Sprite sprite;
sf::Shader shader;
int maxRecursion;
int splitIndex;
int formula;
float splitfactor;
bool stopThreads;
std::vector<std::thread> threads; 
std::queue<std::tuple<int, int, int, int, bool,bool, int, double, double , double>> segmentQueue;
std::condition_variable queueCondition;
std::mutex queueMutex; 





void consoleThread() {
    std::string input;
    while (true) {
        std::getline(std::cin, input);
		try{
			switch(input[0]){
				case '+':input.erase(input.begin());z+= std::stod(input);break;
				case '-':input.erase(input.begin());z-= std::stod(input);break;
				case 'x':input.erase(input.begin());xoff = std::stod(input);break;
				case 'y':input.erase(input.begin());yoff = std::stod(input);break;
				default:
				z = std::stod(input);
			}
			somethingChanged = true;
		}catch(std::exception& e){
			
		}
    }
}



int main(){
	int zoomFactor= 0;
    helperFunctions::displayUI();
    helperFunctions::initVars();
    helperFunctions::initDataArray();

	//first parameter is number of  threads, second is amount of splits horizontally and veritcally, 8 is ideal.
	int tcf = 8;
	startThreads(tcf,8);
	//called once to compute the first frame. 
	reDraw(8);

    sf::RenderWindow window(sf::VideoMode(width, height), "WFractal");
	sf::View view(sf::FloatRect(0, 0, width, height));
    window.setSize(sf::Vector2u(width,height));
    window.setFramerateLimit(60); 
	window.setView(view);
	
	auto prevFrameTime = std::chrono::high_resolution_clock::now();
	auto curFrameTime = std::chrono::high_resolution_clock::now();
	
	sf::Vector2i prevMousePOsition = {0,0};

		bool mouseDragging = false;
		int mouseDragStartx = 0;
		int mouseDragStarty = 0;


//start listener
	std::atomic<bool> runningFlag(true);
	std::thread consoleThread2(consoleThread);

	while (window.isOpen())
	{
		if(prevMousePOsition != sf::Mouse::getPosition(window)){
			if(mouseDragging){
						
						sf::Vector2i m = sf::Mouse::getPosition(window);

						int deltax = m.x - mouseDragStartx;
						int deltay = m.y - mouseDragStarty;

						double panx = std::abs((nx/z)-(px/z)) /width;
						double pany = std::abs((ny/z)-(py/z))/height;						

						xoff -= deltax * panx;
						yoff -= deltay * pany;

						mouseDragStartx  = m.x;
						mouseDragStarty  = m.y;
						std::cout <<"mx:" << m.x << " my:" << m.y << '\n';
						somethingChanged = true;
					}	
		}
		if(completeThreads == tcf){
			curFrame++;
			//std::cout<<curFrame <<  std::endl;
			std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-curFrameTime).count() << std::endl;
			completeThreads = 0;
			//resume = false;

		}else{
			// curFrameTime = std::chrono::high_resolution_clock::now();
			// std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(curFrameTime-prevFrameTime).count() << std::endl;
			// prevFrameTime = std::chrono::high_resolution_clock::now();
		}
		
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

						case sf::Keyboard::Up:
							keyEvents::Up();
							break;
						case sf::Keyboard::Down:
							keyEvents::Down();
							break;
						case sf::Keyboard::Left:
							keyEvents::Left();
							break;
						case sf::Keyboard::Right:
							keyEvents::Right();
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
							zoomFactor++;
						z+=zoomFactor;
						somethingChanged = true;
						}
						else if (event.mouseWheelScroll.delta < 0){
						z-=zoomFactor;
						zoomFactor--;
						somethingChanged = true;
						}
					break;
				case sf::Event::MouseButtonPressed:
					printf("mousedown\n");
					mouseDragging = true;
					mouseDragStartx = sf::Mouse::getPosition(window).x;
					mouseDragStarty = sf::Mouse::getPosition(window).y;
					break;
				
				case sf::Event::MouseButtonReleased:
					mouseDragging = false;
					printf("mouseUp\n");
					break;

				case sf::Event::MouseMoved:
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
			curFrameTime = std::chrono::high_resolution_clock::now();
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