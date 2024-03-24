#include "../globalVars.h"
#include "../helperFunctions/helperFunctions.h"
#include "thread.cpp"
#include <iostream>
#include <mutex>
#include <queue>
#include <chrono>
#include <cstring>

void startThreads(int threadCount,int numSegments){
    //numSegments is idealy 8 (while 9 is faster it creates more overhead with non perfectlly sized chunks)
    //
    // since 1920/8 = 240 (whole)    240 / 3 = 80 (whole) there is no need to care about the decimal portions
    //                            segment width^                   ^width of the chunk processed by the AVX512 computer
    //
    //8 is ideal for 1080 and 4K. At 1440p it breaks at 2560/8 = 320 / 3 = 106.6.. otherwise its perfect 
    
    //Create threads
    for (int i = 0; i < threadCount; ++i) {  
			
        threads.emplace_back(threadWorker, 0, 0, 0, 0, i);
        
    }   
}









void killThreads(){
   stopThreads = true;
   queueCondition.notify_all(); 

   std::this_thread::sleep_for(std::chrono::milliseconds(100));
   for (auto& worker : threads) {
        if (worker.joinable()) {
            worker.join();
        }
    }
   
}


void reDraw(int numSegments){
    
    int segmentWidth = width / numSegments;
    int segmentHeight = height / numSegments;

    //if negative Y and positive Y have different signs and mandelbrot is begin computed following will be true -> simmetry can be utilised
    double img1 = ny/z+yoff;
	double img2 = py/z+yoff;
    bool simViable =  (img1 < 0 && img2 > 0) && formula == 1;
    double conversionTreshold = std::abs(nx/z-px/z)/(width+height*2);
    double dx, dy;
    double xo = helperFunctions::mapValue(0, 0, 1920, (nx / z)+xoff, (px / z)+xoff);
    double yo = helperFunctions::mapValue(0, 0, 1080, (ny / z)+yoff, (py / z)+yoff);

    double xo1 = helperFunctions::mapValue(1, 0, 1920, (nx / z)+xoff, (px / z)+xoff);
    double yo1 = helperFunctions::mapValue(1, 0, 1080, (ny / z)+yoff, (py / z)+yoff);

    dx = std::abs(xo1-xo);
    dy = std::abs(yo1-yo);


    //reset all pixeldata
    for(int i =0; i < width; i++){
        for(int j =0; j < height; j++){
            data[i][j][0] = -9999;
            data[i][j][1] = -9999;
            data[i][j][2] = -9999;
            data[i][j][3] = -9999;

        }
    }


    if(simViable && true){
        
        realLine = (int)helperFunctions::mapValue(0,img1, img2, 0, height);
        //following will be true If real number line corsses the screen closer to its top, and false if bottom
        bool topORbottom = std::abs(realLine-0) < std::abs(realLine-height) ? true : false;

        if(topORbottom){ //if closer to top of screen   
            
            for (int i = 0; i < numSegments; ++i) {
                for (int j = 0; j < numSegments; ++j) {
                    int startx= i * segmentWidth;
                    int starty = j * segmentHeight;
                    int endx = (i + 1) * segmentWidth;
                    int endy = (j + 1) * segmentHeight;

                    //if end of current segment is lesser than realLine that means it can be completeley mirrored
                    if((j+1) * segmentHeight <= realLine){
                        segmentQueue.push(std::make_tuple(startx, starty, endx, endy, true, true,1,conversionTreshold, dx, dy));
                    }
                    /*}else if(j*segmentHeight < realLine){
                        segmentQueue.push(std::make_tuple(startx,startx,endx, realLine, true, true, 1, conversionTreshold, dx, dy));
                    }*/
                    else{
                        segmentQueue.push(std::make_tuple(startx, starty, endx, endy, false, true,1,conversionTreshold, dx, dy));
                    }
                }
            }
        }else{
            for (int i = 0; i < numSegments; ++i) {
                for (int j = 0; j < numSegments; ++j) {
                    int startx= i * segmentWidth;
                    int starty = j * segmentHeight;
                    int endx = (i + 1) * segmentWidth;
                    int endy = (j + 1) * segmentHeight;
                    if(j*segmentHeight >= realLine){
                       segmentQueue.push(std::make_tuple(startx, starty, endx, endy, true,false, 1,conversionTreshold, dx, dy));
                    }else{
                       segmentQueue.push(std::make_tuple(startx, starty, endx, endy, false,false,1,conversionTreshold, dx, dy));
                    }
                }
            }
        }
    }else{
        //Create and store segments
        //Segment - A chunk of the set that will be worked on by the thread at point in time.
        for (int i = 0; i < numSegments; ++i) {
            for (int j = 0; j < numSegments; ++j) {
                int startx= i * segmentWidth;
                int starty = j * segmentHeight;
                int endx = (i + 1) * segmentWidth;
                int endy = (j + 1) * segmentHeight;

                segmentQueue.push(std::make_tuple(startx, starty, endx, endy, false,false,1,conversionTreshold, dx, dy));
            }
        }
    }



    //Wake threads- Start computation
    completeThreads = 0;
    queueCondition.notify_all();
}

void fetchData(){
  
    std::unique_lock<std::mutex> lock(queueMutex);
   
	

    for (int i = 0; i < width; i += 1) {
        for ( int j = 0; j < height; j += 1) {
			//Navigation through the array with step of 4 on both i and j.
            int index = (j * 4) * width + (i * 4);
            

            int delta = 0;
            // OPTIMIZE WITH AVX 
            delta = (int)helperFunctions::mapValue(data[i][j][0],0,maxIteration,0,255);
           
            pictureData[index]   = delta;
            pictureData[index+1] = delta;
            pictureData[index+2] = delta;
            pictureData[index+3] = 255;
        }
    }
    lock.unlock();
    
	picture.update(pictureData);
	sprite.setTexture(picture,true);

}