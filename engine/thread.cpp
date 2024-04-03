#include <iostream>
#include <mutex>
#include <queue>
#include <cmath>
#include <chrono>
#include <random>
#include <immintrin.h>
#include <mpfr.h>

#include <mpfr.h>



//double conversionTreshold
int compute(int x, int y, double conversionTreshold) {
        if((int)x >=0 && (int)x <=width-1 && (int)y>=0 && (int)y<=height-1){
              
                        double prevX = 0;
                        double prevY = 0;

                        double xo = helperFunctions::mapValue(x, 0, width, (nx / z)+xoff, (px / z)+xoff);
                        double yo = helperFunctions::mapValue(y, 0, height, (ny / z)+yoff, (py / z)+yoff);
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
                        double xo = helperFunctions::mapValue(x, 0, width, (nx / z)+xoff, (px / z)+xoff);
                        double yo = helperFunctions::mapValue(y, 0, height, (ny / z)+yoff, (py / z)+yoff);
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
void computeQuad(int x, int y, int x2, int y2, int x3, int y3, int x4, int y4){
    __m256d const2 = _mm256_set1_pd(2.0);
    __m256d const4 = _mm256_set1_pd(4.0);
    __m256d constneg1 = _mm256_set1_pd(-1.0);
    __m256d constpos1 = _mm256_set1_pd(1.0);

    alignas(32) double xo[4], yo[4];
    alignas(32) double constreals[4], constimags[4];
    bool allZero = true;
    __m256d reals, imags, constrealsVec, constimagsVec, a, b, d, t, t2, t3, counter;

    __m256d mask;
     alignas(32) double maskArray[4];
    
            allZero = true;       
         
                xo[0] = helperFunctions::mapValue(x, 0, width, (nx / z) + xoff, (px / z) + xoff);
                yo[0] = helperFunctions::mapValue(y, 0, height, (ny / z) + yoff, (py / z) + yoff);

                xo[1] = helperFunctions::mapValue(x2, 0, width, (nx / z) + xoff, (px / z) + xoff);
                yo[1] = helperFunctions::mapValue(y2, 0, height, (ny / z) + yoff, (py / z) + yoff);

                xo[2] = helperFunctions::mapValue(x3, 0, width, (nx / z) + xoff, (px / z) + xoff);
                yo[2] = helperFunctions::mapValue(y3, 0, height, (ny / z) + yoff, (py / z) + yoff);

                   xo[3] = helperFunctions::mapValue(x4, 0, width, (nx / z) + xoff, (px / z) + xoff);
                yo[3] = helperFunctions::mapValue(y4, 0, height, (ny / z) + yoff, (py / z) + yoff);
                
                for(int i =0; i < 4; i++){
                constreals[i] = xo[i];
                constimags[i] = yo[i];
                }
            
            //  if(data[x][y][1] !=-9999){
            //     xo[0] = data[x][y][1];
            //     yo[0] = data[x][y][2];
            //     }
            
            //  if(data[x2][y2][1] !=-9999){
            //     xo[1] = data[x2][y2][1];
            //     yo[1] = data[x2][y2][2];
            //     }
            
            //  if(data[x3][y3][1] !=-9999){
            //     xo[2] = data[x3][y3][1];
            //     yo[2] = data[x3][y3][2];
            //     }
            
            //  if(data[x4][y4][1] !=-9999){
            //     xo[3] = data[x4][y4][1];
            //     yo[3] = data[x4][y4][2];
            //     }
            

             reals = _mm256_load_pd(xo);
             imags = _mm256_load_pd(yo);

             constrealsVec = _mm256_load_pd(constreals);
             constimagsVec = _mm256_load_pd(constimags);

            //
             counter = _mm256_setzero_pd();
            
           // counter = _mm256_set1_pd(splitIndex);
            
            //make mask 0b111111111......11111
             mask = _mm256_castsi256_pd(_mm256_set1_epi64x(-1));
            
            for (int c = 0; c < maxIteration; c += 1) {


            //_mm256_blendv_pd(reals, _mm256_setzero_pd(), mask); 
            //IF     nth bit in mask is 0 value from first vector is chosen ELSE second is chosen

                        //a = r*r
                t = _mm256_mul_pd(reals, reals);
                a = _mm256_blendv_pd(a,t, mask); 
                
                    //b=  i * i
                t = _mm256_mul_pd(const2, _mm256_mul_pd(reals, imags));
                b = _mm256_blendv_pd(a,t, mask); 

                    //d = i * i * -1
                t = _mm256_mul_pd(imags, _mm256_mul_pd(imags, constneg1));
                d = _mm256_blendv_pd(d, t, mask); 
        
                    // r = a + d + xo
                t = _mm256_add_pd(a, _mm256_add_pd(d, constrealsVec));
                reals = _mm256_blendv_pd(reals,t, mask); 

                    // i = b + yo
                t = _mm256_add_pd(b, constimagsVec);
                imags = _mm256_blendv_pd(imags, t, mask); 

                        //counter ++ for elements who did not escape
                t = _mm256_add_pd(counter, constpos1);
                counter = _mm256_blendv_pd(counter,t, mask);
                    //counter = _mm256_add_pd(counter,constpos1);
                            //t = r * r + i  * i
                t = _mm256_add_pd(_mm256_mul_pd(reals, reals), _mm256_mul_pd(imags, imags));

                // t < 4
                //first 64 bits will be 1 IF first element of t is not greather than 4, so rest can proceeed
                mask= _mm256_cmp_pd(t, const4, _CMP_NGT_UQ);
        
            
                _mm256_store_pd(maskArray, mask);
    
                allZero = true;
        for (int i = 0; i < 4; ++i) {
            if (maskArray[i] != 0) {
                allZero = false;
                //break;
            }
        }

      

    
                if (allZero) {
                break;
                }
            }

            // c    0       mit    0    255
            // x   xmin   xmax   ymin  ymax

            // ((X - X_min) / (X_max - X_min)) * (Y_max - Y_min) + Y_min;
            // ((c - 0)) / (mit - 0) * (255  - 0) + 0
            // (c) / (mit) * 255


            // t = _mm256_div_pd(counter, _mm256_set1_pd(maxIteration));

            // t2 = _mm256_mul_pd(_mm256_set1_pd(maxIteration), t);

            // t = _mm256_mul_pd(_mm256_set1_pd(255), t2);
            t = counter;
            
            alignas(32) double iterationCounts[4];
            _mm256_store_pd(iterationCounts, t);
            

        
           data[x][y][0] = iterationCounts[0];
           data[x2][y2][0] = iterationCounts[1];
           data[x3][y3][0] = iterationCounts[2];
           data[x4][y4][0] = iterationCounts[3];
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
          
                
                

                computeQuad(x  , y-1,
                            x+1, y-1, 
                            x-1, y-1,
                            x-1, y);

                computeQuad(x  , y+1,
                            x+1, y+1,
                            x-1, y+1,
                            x+1, y

                );
                // c =computeFromRefOrb(x  ,y-1, false);           
                // c =computeFromRefOrb(x+1,y-1, false);
                // c =computeFromRefOrb(x-1,y-1, false);
                // c =computeFromRefOrb(x-1,y  , false );

                
                // c =computeFromRefOrb(x  ,y+1, false);           
                // c =computeFromRefOrb(x+1,y+1, false);
                // c =computeFromRefOrb(x-1,y+1, false);
                // c =computeFromRefOrb(x+1,y  , false );

                //if (c == maxIteration) {
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
        double xo = helperFunctions::mapValue(localStartx, 0, width, (nx / z)+xoff, (px / z)+xoff);
        double yo = helperFunctions::mapValue(localStarty, 0, height, (ny / z)+yoff, (py / z)+yoff);
     
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
