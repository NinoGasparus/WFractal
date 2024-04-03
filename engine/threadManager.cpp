#include "../globalVars.h"
#include "../helperFunctions/helperFunctions.h"
#include "thread.cpp"
#include <iostream>
#include <mutex>
#include <queue>
#include <chrono>
#include <cstring>
#include <cstdint>
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
    double xo = helperFunctions::mapValue(0, 0, width, (nx / z)+xoff, (px / z)+xoff);
    double yo = helperFunctions::mapValue(0, 0, height, (ny / z)+yoff, (py / z)+yoff);

    double xo1 = helperFunctions::mapValue(1, 0, width, (nx / z)+xoff, (px / z)+xoff);
    double yo1 = helperFunctions::mapValue(1, 0, height, (ny / z)+yoff, (py / z)+yoff);

    dx = std::abs(xo1-xo);
    dy = std::abs(yo1-yo);


    //reset all pixeldata
    for(int i =0; i < width; i++){
        for(int j =0; j < height; j++){
            //data[i][j][0] = -9999;
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
                        segmentQueue.push(std::make_tuple(startx, starty, endx, endy, false, false,1,conversionTreshold, dx, dy));
                    }
                    /*}else if(j*segmentHeight < realLine){
                        segmentQueue.push(std::make_tuple(startx,startx,endx, realLine, true, true, 1, conversionTreshold, dx, dy));
                    }*/
                    else{
                        segmentQueue.push(std::make_tuple(startx, starty, endx, endy, false, false,1,conversionTreshold, dx, dy));
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
                       segmentQueue.push(std::make_tuple(startx, starty, endx, endy, false,false, 1,conversionTreshold, dx, dy));
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


void hexaDecMap(uint16_t* data) {
    // Constants for the calculation
    const float ymin = 0.0f;
    const float ymax = 255.0f;
    const float xmin = 0.0f;
    const float xmax = static_cast<float>(maxIteration);

    // Load the constants into SIMD registers
    __m256 y_min = _mm256_set1_ps(ymin);
    __m256 y_max = _mm256_set1_ps(ymax);
    __m256 x_max = _mm256_set1_ps(xmax);

    // Load the input data
    __m256i input_data = _mm256_loadu_si256((__m256i*)data);

    // Convert input data to float
    __m256 x = _mm256_cvtepi32_ps(_mm256_unpacklo_epi16(input_data, _mm256_setzero_si256()));
    __m256 y = _mm256_cvtepi32_ps(_mm256_unpackhi_epi16(input_data, _mm256_setzero_si256()));

    // Compute the mapping
    __m256 mapped_data = _mm256_mul_ps(_mm256_div_ps(_mm256_sub_ps(x, _mm256_set1_ps(xmin)), _mm256_sub_ps(x_max, _mm256_set1_ps(xmin))),
                                        _mm256_sub_ps(y_max, y_min));

    // Convert the result back to integer
    __m256i result_data = _mm256_cvtps_epi32(mapped_data);

    // Store the result back to memory
    _mm256_storeu_si256((__m256i*)data, result_data);
}

void fetchData(){
    auto start = std::chrono::high_resolution_clock::now();
 //   std::unique_lock<std::mutex> lock(queueMutex);
   
	int delta = 0;
int index;
uint16_t* values = new uint16_t[16];
    for (int i = 0; i < width; i += 1) {
        for ( int j = 0; j < height; j += 1) {
            
        	//Navigation through the array with step of 4 on both i and j.
             
             
            // values[0] = data[i][j][0];
            // values[1] = data[i + 1][j][0];
            // values[2] = data[i + 2][j][0];
            // values[3] = data[i + 3][j][0];
            // values[4] = data[i + 4][j][0];
            // values[5] = data[i + 5][j][0];
            // values[6] = data[i + 6][j][0];
            // values[7] = data[i + 7][j][0];
            // values[8] = data[i + 8][j][0];
            // values[9] = data[i + 9][j][0];
            // values[10] = data[i + 10][j][0];
            // values[11] = data[i + 11][j][0];
            // values[12] = data[i + 12][j][0];
            // values[13] = data[i + 13][j][0];
            // values[14] = data[i + 14][j][0];
            // values[15] = data[i + 15][j][0];

            // hexaDecMap(values);
            
             delta = (int)helperFunctions::mapValue(data[i][j][0],0, maxIteration,0, 255);
             index = (j*4) * width + (i*4);    
                   pictureData[index]   = delta;
                   pictureData[index+1] = delta;
                   pictureData[index+2] = delta;
                   pictureData[index+3] = 255;
                
            index = (j*4) * width + (i*4);
            // // OPTIMIZE WITH AVX 
            // for(int tmp = 0; tmp < 16; tmp++){
                


            //     pictureData[index+4*tmp]   = values[tmp];
            //     pictureData[index+1+4*tmp] = values[tmp];
            //     pictureData[index+2+4*tmp] = values[tmp];
            //     pictureData[index+3+4*tmp] = 255;
                
            // }         
            

        }
    }
 //   lock.unlock();
    
	picture.update(pictureData);
	sprite.setTexture(picture,true);
    auto end = std::chrono::high_resolution_clock::now();
if(somethingChanged)
    std::cout << "took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms\n";
}