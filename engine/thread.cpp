#include <iostream>
#include <mutex>
#include <queue>
#include <cmath>
#include <chrono>
#include <random>
//double conversionTreshold
int compute(double x, double y) {
        double xo = helperFunctions::mapValue(x, 0, 1920, (nx / z)+xoff, (px / z)+xoff);
        double yo = helperFunctions::mapValue(y, 0, 1080, (ny / z)+yoff, (py / z)+yoff);
        double r = xo;
        double i = yo;
    int c = 0;
    double a, b, d;        
    while (r * r + i * i < 4 && ++c < maxIteration) {
        a = r * r;
        b = 2 * r * i;
        d = i * i * -1;
        
        r = a + d + xo;
        i = b + yo;
                    // if(std::abs(prevX-r) < conversionTreshold ){
                    //    c = maxIteration;
                    //    break;
                    // }
        
                    // prevX  = r;
                    // prevY = i;
    }
    return c;
  
}

void rect(int x, int y, int dx, int dy, int value){
    for(int i =x; i < x+dx; i++){
        for(int j = y; j < y+dy; j++){
            data[i][j][0] = value;
        }
    }
}

void threadWorker(int startx, int starty, int endx, int endy) {
    while (true) {
        std::unique_lock<std::mutex> lock(queueMutex);

        

        queueCondition.wait(lock, [] { return !segmentQueue.empty() || stopThreads; });

        auto segment = segmentQueue.front();
        segmentQueue.pop();
        lock.unlock();

        int localStartx, localStarty, localEndx, localEndy;
        int res;
        bool simmetry;
        bool top;
        double conversionTreshold;
        double deltax, deltay;
        std::tie(localStartx, localStarty, localEndx, localEndy, simmetry, top, res, conversionTreshold, deltax, deltay) = segment;
        double xo = helperFunctions::mapValue(localStartx, 0, 1920, (nx / z)+xoff, (px / z)+xoff);
        double yo = helperFunctions::mapValue(localStarty, 0, 1080, (ny / z)+yoff, (py / z)+yoff);
     
        double iniy = yo;
      
        int c=0;
        int x = localStartx;
        int dx = localEndx-localStartx;
        int y = localStarty;
        int dy = localEndy-localStarty;
        //reference value

        int base = compute(x, y);
        bool edgesEqual = true;

         for (int i = y; i < y + dy; i++) {
            int edgeValue = compute(x + dx, i);
            if (edgeValue != base) {
                edgesEqual = false;
                break;
            }
        }

        for (int i = y; i < y + dy; i++) {
            int edgeValue = compute(x, i);
            if (edgeValue != base) {
                edgesEqual = false;
                break;
            }
        }

      
        if (edgesEqual) {
            for (int i = x; i < x + dx; i++) {
                int edgeValue = compute(i, y + dy);
                if (edgeValue != base) {
                    edgesEqual = false;
                    break;
                }
            }
        }
        if (edgesEqual) {
            for (int i = x; i < x + dx; i++) {
                int edgeValue = compute(i, y);
                if (edgeValue != base) {
                    edgesEqual = false;
                    break;
                }
            }
        }




        
        if (edgesEqual) {
            rect(localStartx,localStarty,localEndx-localStartx,localEndy-localStarty,base);
        }else{      
            std::random_device rd;
            std::mt19937 gen(rd()); 
            std::uniform_int_distribution<> dis(1, 255);

    
                    
            rect(x, y, dx, dy, dis(gen));
        //      for (int x = localStartx; x < localEndx; ++x) {
        //     for (int y = localStarty; y < localEndy; ++y) {
        //         c = compute(xo,yo,conversionTreshold);
        //         yo += deltay;

        //         if (c == maxIteration) {
        //             if(simmetry && top){
        //                 data[x][(int)(realLine*2-y)][0] = 0;
        //             }
        //             if(simmetry && !top){
        //                 data[x][realLine-(y-realLine)][0] = 0;
        //             }
        //             data[x][y][0] = 0;
        //             continue;
        //         }else{
        //             if(simmetry && top){
        //                 data[x][(int)(realLine*2-y)][0] = c;
        //             }
        //             if(simmetry && !top){
        //                 data[x][realLine-(y-realLine)][0] = c;
        //             }
        //             data[x][y][0] = c;
        //             continue;
        //         }            
        //     }
        //     yo = iniy;
        //     xo += deltax;
        // }
        }
     

    }
}
