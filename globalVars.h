#include <SFML/Graphics.hpp>
#include <thread>
#include <queue> 
#include <condition_variable> 
#include <mutex> 

extern int      width;
extern int      height;
extern double   nx;
extern double   px;
extern double   ny;
extern double   py;
extern int      realLine;
extern long     z;
extern double   xoff;
extern double   yoff;
extern long     maxIteration;
extern int      iterat;

extern int      res;
extern bool     somethingChanged;
extern double*** data;
extern sf::Uint8* pictureData;
extern sf::Texture picture;
extern sf::Sprite sprite;
extern sf::Shader shader;

extern int formula;

extern bool stopThreads; 
extern std::vector<std::thread> threads; 
extern std::queue<std::tuple<int, int, int, int, bool, bool, int, double, double, double>> segmentQueue;
extern std::condition_variable queueCondition;
extern std::mutex queueMutex; 