#include <iostream>
#include <mutex>
#include <queue>
#include <cmath>
#include <chrono>
#include <random>
//double conversionTreshold
int compute(int x, int y, double conversionTreshold) {
        if((int)x >=0 && (int)x <=width-1 && (int)y>=0 && (int)y<=height-1){
              
                        double prevX = 0;
                        double prevY = 0;

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
                            if(std::abs(prevX-r) < conversionTreshold ){
                                c = maxIteration;
                                break;
                            }
                            
                            prevX  = r;
                            prevY = i;
                        }
                        if(c ==maxIteration){
                            data[x][y][0] = 0;
                            return 0;
                        }
                        data[(int)x][(int)y][0] = c;
                        return c;
        }else{
          return 0;
        }
      
  
}



int computeFromRefOrb(int x, int y, bool center){

        if((int)x >=0 && (int)x <=width-1 && (int)y>=0 && (int)y<=height-1){
                        double xo = helperFunctions::mapValue(x, 0, 1920, (nx / z)+xoff, (px / z)+xoff);
                        double yo = helperFunctions::mapValue(y, 0, 1080, (ny / z)+yoff, (py / z)+yoff);
                        double r = xo;
                        double i = yo;
                        int c = 0;
                        double a, b, d;        
                        if(data[x][y][1] !=-9999){
                            r = data[x][y][1];
                            i = data[x][y][2];
                            c = splitIndex;
                        }
                        while (r * r + i * i < 4 && ++c < maxIteration) {
                            a = r * r;
                            b = 2 * r * i;
                            d = i * i * -1;
                            
                            r = a + d + xo;
                            i = b + yo;
                            if(c == splitIndex && !center){
                                 try {
                                data[x - 1][y + 1][1] = r;
                                data[x - 1][y + 1][2] = i;
                                data[x][y + 1][1] = r;
                                data[x][y + 1][2] = i;
                                data[x + 1][y + 1][1] = r;
                                data[x + 1][y + 1][2] = i;
                                data[x - 1][y][1] = r;
                                data[x - 1][y][2] = i;
                                data[x][y][1] = r;
                                data[x][y][2] = i;
                                data[x + 1][y][1] = r;
                                data[x + 1][y][2] = i;
                                data[x - 1][y - 1][1] = r;
                                data[x - 1][y - 1][2] = i;
                                data[x][y - 1][1] = r;
                                data[x][y - 1][2] = i;
                                data[x + 1][y - 1][1] = r;
                                data[x + 1][y - 1][2] = i;

                            } catch (std::exception& e) {
                            }

                            }
                        }
                        if(c ==maxIteration){
                            data[x][y][0] = 0;
                            return 0;
                        }
                        data[(int)x][(int)y][0] = c;
                        return c;
                        return c;
        }else{
          return 0;
        }
      
}
void rect(int x, int y, int dx, int dy, int value){
    for(int i =x; i < x+dx; i++){
        for(int j = y; j < y+dy; j++){
            data[i][j][0] = value;
        }
    }
}

void fragmentCompute(int localStartx, int localStarty, int localEndx, int localEndy, int depth, int maxDepth, double conversionTreshold){
        
        int base = compute(localStartx, localStarty, conversionTreshold);
        bool edgesEqual = true;



        //left and right edge
         for (int i = localStarty; i < localEndy; i++) {
            int edgeValue = compute(localEndx, i, conversionTreshold);
            
            if (edgeValue != base) {
                edgesEqual = false;
                break;
            }
            edgeValue = compute(localStartx, i, conversionTreshold);
            if (edgeValue != base) {
                edgesEqual = false;
                break;
            }
        }

      //top and bottom
        if (edgesEqual) {
            for (int i = localStartx; i < localEndx; i++) {
                int edgeValue = compute(i, localEndy, conversionTreshold);
                if (edgeValue != base) {
                    edgesEqual = false;
                    break;
                }
                edgeValue = compute(i, localStarty, conversionTreshold);
                if (edgeValue != base) {
                    edgesEqual = false;
                    break;
                }
            }
          
        }
        
        if (edgesEqual) {
            rect(localStartx,localStarty,localEndx-localStartx,localEndy-localStarty,base);
        }else if(depth < maxDepth){
                int newDepth = depth + 1;

            int halfWidth =(int)(localEndx-localStartx)/2;
            int halfHeight = (int)(localEndy-localStarty)/2;
            fragmentCompute(localStartx, localStarty, localStartx+halfWidth, localStarty + halfHeight, newDepth, maxDepth, conversionTreshold);
            fragmentCompute(localStartx+halfWidth, localStarty, localEndx, localStarty+halfHeight,     newDepth, maxDepth, conversionTreshold);
            fragmentCompute(localStartx, localStarty+ halfHeight,localStartx+halfWidth,localEndy,      newDepth, maxDepth, conversionTreshold);
            fragmentCompute(localStartx+halfWidth, localStarty+halfHeight, localEndx, localEndy,       newDepth, maxDepth, conversionTreshold);                




        
        }else{      
             for (int x = localStartx+1; x < localEndx; x+=3) {
                for (int y = localStarty+1; y < localEndy; y+=3) {
                    // int c = compute(x,y, conversionTreshold);
                int c = computeFromRefOrb(x,y, true);
          
                 c =  computeFromRefOrb(x+1,y, false );
                c = computeFromRefOrb(x-1,y, false );
                    
                c =    computeFromRefOrb(x  ,y-1, false);           
                c =    computeFromRefOrb(x+1,y-1, false);
                    c =computeFromRefOrb(x-1,y-1, false);
                    
                   c = computeFromRefOrb(x,y+1, false);           
                    c =computeFromRefOrb(x+1,y+1, false);
                    c =computeFromRefOrb(x-1,y+1, false);

                // if (c == maxIteration) {
                //     // if(simmetry && top){
                //     //     data[x][(int)(realLine*2-y)][0] = 0;
                //     // }
                //     // if(simmetry && !top){
                //     //     data[x][realLine-(y-realLine)][0] = 0;
                //     // }
                //     data[x][y][0] = 0;
                //     continue;
                // }else{
                //     // if(simmetry && top){
                //     //     data[x][(int)(realLine*2-y)][0] = c;
                //     // }
                //     // if(simmetry && !top){
                //     //     data[x][realLine-(y-realLine)][0] = c;
                //     // }
                //     data[x][y][0] = c;
                //     continue;
                // }            
            }
            // yo = iniy;
            // xo += deltax;
        }

        }
     

}

void threadWorker(int startx, int starty, int endx, int endy, int tid) {
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
        

        fragmentCompute(localStartx, localStarty, localEndx, localEndy, 0, maxRecursion, conversionTreshold);
        
        lock.lock();

        if(segmentQueue.empty()){
          //  std::cout <<"thread : " << tid <<" done" << '\n';
            completeThreads++;
        }
        lock.unlock();
    }
}
